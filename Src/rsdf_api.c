//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: rsdf_api.c
//
// DESCRIPTION: API that encode data that will be send over
// ereasure channels. It combines the Reed Solomon and Digital 
// Fountain techniques to ensure a reliable communication
// during telemetry.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************


/* ----- INCLUDES ----- */
#include "rsdf_api.h"

/* ----- LOCAL VARS ----- */
//-- FREERTOS VARS/DEFS
#define NUM_TASKS 5
#define FREERTOS_IDLE_CALLBACK

// Handles
TaskHandle_t taskHandles[NUM_TASKS];

#define tMain			0
#define tStoreData		1
#define tPureEnc		2
#define tDFcwEnc		3
#define tSendData		4


// TaskNotify
#define nNEW_HDATA		0x02
#define nSTO_RDY		0x04
#define nNEW_BLOCK		0x08
#define nFREE_BAND		0x10
#define nNEW_CODEWORD	0x20
#define nDF_CALCULED	0x40


//-- VARS
uint8_t *ptr_buff_in;
static uint32_t n_block = 0; //number of received blocks
uint8_t run_flag = 0x00;

uint8_t buff_out_pure[OUT_SZ] = { 0 };
uint8_t buff_out_dfcw[OUT_SZ] = { 0 };
uint16_t buff_sto_sz = 0;

StreamBufferHandle_t xPureStream; // alloc stream buffer
static uint8_t ucPureStreamStorage[OUT_SZ * 3];
StaticStreamBuffer_t xPureStreamStruct;

StreamBufferHandle_t xDFStream; // alloc stream buffer
static uint8_t ucDFStreamStorage[OUT_SZ * 3];
StaticStreamBuffer_t xDFStreamStruct;

// MONITORING VARS
uint32_t num_msg_stored = 0;
uint32_t num_msg_send = 0;
uint32_t num_dfcw_send = 1;
volatile uint8_t stream_block_flag = 0x00;

// PROTOTYPES
static void vMain(void *pvParameters);
static void vStoreData(void *pvParameters);
static void vPureEnc(void *pvParameters);
static void vDFcwEnc(void *pvParameters);
static void vSendData(void *pvParameters);

/* ----- FUNCTIONS ----- */
void initRSF(uint8_t reset_mem)
{
	initStorage(reset_mem);
	initRSenc(RS_N, RS_K);
	xPureStream = xStreamBufferGenericCreateStatic(OUT_SZ * 3, 16, pdFALSE, ucPureStreamStorage, &xPureStreamStruct);
	xDFStream = xStreamBufferGenericCreateStatic(OUT_SZ * 3, 16, pdFALSE, ucPureStreamStorage, &xPureStreamStruct);

	xTaskCreate(vMain, "MAIN_TASK", configMINIMAL_STACK_SIZE, NULL, 4, &taskHandles[tMain]);
	
	vTaskStartScheduler();
}

void storeTrigger(uint8_t* ptr_data, uint16_t data_sz)
{
	ptr_buff_in = ptr_data;
	buff_sto_sz = data_sz;
	xTaskNotify(taskHandles[tMain], nSTO_RDY, eSetValueWithOverwrite); 
	
}



static void vMain(void *pvParameters)
{
	unsigned long ulNotifiedMain =  0;
	unsigned buff_in_cnt = 0;
	
	
	// Create Tasks
	xTaskCreate(vStoreData, "STO_DATA", configMINIMAL_STACK_SIZE, NULL, 3, &taskHandles[tStoreData]);
	xTaskCreate(vPureEnc, "PURE_ENC", ((uint16_t) 1536), NULL, 1, &taskHandles[tPureEnc]);
	xTaskCreate(vDFcwEnc, "DFCW_ENC", ((uint16_t) 1536), NULL, 0, &taskHandles[tDFcwEnc]);
	xTaskCreate(vSendData, "TX_DATA", configMINIMAL_STACK_SIZE, NULL, 2, &taskHandles[tSendData]);
	
	// ((uint16_t) 1536)
	for(;;)
	{
		if (xTaskNotifyWait(0, 0, &ulNotifiedMain, portMAX_DELAY) == pdTRUE)
		{	
			//Check if sto_rdy
			if((ulNotifiedMain & nSTO_RDY) != 0x00)
			{
				ulNotifiedMain -= nSTO_RDY;
				vTaskResume(taskHandles[tStoreData]);
				//vTaskSuspend(taskHandles[tSendData]);
			}
			//Generete Pure Msg
			if((ulNotifiedMain & nNEW_BLOCK) != 0x00)
			{
				ulNotifiedMain -= nNEW_BLOCK;
				vTaskResume(taskHandles[tPureEnc]);
			}
			//Generate DF CW
			if((ulNotifiedMain & nFREE_BAND) != 0x00)
			{
				ulNotifiedMain -= nFREE_BAND;
				vTaskResume(taskHandles[tDFcwEnc]);
			}
			if ((ulNotifiedMain & nNEW_CODEWORD) != 0x00)
			{
				ulNotifiedMain = -nNEW_CODEWORD;
				vTaskResume(taskHandles[tSendData]);
			}
		}
	}
}


static void vStoreData(void *pvParameters)
{
	TickType_t xLastWakeTime;
	vTaskSuspend(NULL);
	for (;;)
	{
		/*xLastWakeTime = xTaskGetTickCount();
		while (stream_block_flag == 0xFF)
			vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
			
		stream_block_flag = 0xFF;*/
		
		save_data(((uint32_t *)ptr_buff_in), buff_sto_sz);
		
		//stream_block_flag = 0x00;
		if(getStorageCount() >= ((n_block + 1)*BLOCK_SZ))
		{
			n_block += 1;
			num_msg_stored += 1;
			xTaskNotify(taskHandles[tMain], nNEW_BLOCK, eSetValueWithOverwrite);
		}
		vTaskSuspend(NULL);
	}
}

uint8_t pure_run = 0x00;

static void vPureEnc(void *pvParameters)
{
	size_t xBytesSent;
	uint8_t *ptr_data;
	vTaskSuspend(NULL);
	TickType_t xLastWakeTime;
	for (;;)
	{
		pure_run = 0xFF;
			
		while (num_msg_stored > num_msg_send)
		{
			ptr_data = load_data(num_msg_send);
			encode_rs(ptr_data, &buff_out_pure[HEADER_SZ], RS_N, RS_K, BLOCK_SZ);
		
			insertheader(buff_out_pure, 0x00, num_msg_send);
		
			xBytesSent = xStreamBufferSend(xPureStream, (void *) buff_out_pure, OUT_SZ, 0);
			stream_block_flag = 0x00;
		
			if (xBytesSent == OUT_SZ)
			{
				num_msg_send += 1;
			}
		
			xTaskNotify(taskHandles[tMain], nNEW_CODEWORD, eSetValueWithOverwrite);
		}
		pure_run = 0x00;
		vTaskSuspend(NULL);
	}
	
}


static void vDFcwEnc(void *pvParameters)
{
	uint8_t cw[BLOCK_SZ] = { 0 };
	uint32_t seed = 0;
	uint8_t *ptr_data;
	uint32_t n_sto_msg = 0;
	size_t xBytesSent;
	TickType_t xLastWakeTime;
	vTaskSuspend(NULL);
	for (;;)
	{
		run_flag = 0xFF;
		if ((xStreamBufferBytesAvailable(xDFStream) <= OUT_SZ))
		{
			seed = num_dfcw_send;
			n_sto_msg = num_msg_stored;
			if (num_msg_stored != 1)
			{
				encode_df(cw, n_sto_msg, BLOCK_SZ, seed);
		
				encode_rs(cw, &buff_out_dfcw[HEADER_SZ], RS_N, RS_K, BLOCK_SZ);
			}
			else
			{
				ptr_data = load_data(0);
				encode_rs(ptr_data, &buff_out_dfcw[HEADER_SZ], RS_N, RS_K, BLOCK_SZ);
			}
		
			insertheader(buff_out_dfcw, seed, n_sto_msg);
			
			xLastWakeTime = xTaskGetTickCount();
			while (stream_block_flag == 0xFF)
				vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
			
			stream_block_flag = 0xFF;
			
			xBytesSent = xStreamBufferSend(xDFStream, buff_out_dfcw, OUT_SZ, 0);
			stream_block_flag = 0x00;
			
			if (xBytesSent == OUT_SZ)
			{
				num_dfcw_send += 1;
				if ((num_dfcw_send > 0xFF) || (num_dfcw_send == 0x00))
					num_dfcw_send = 1;
			}
			
		}
		run_flag = 0x00;
		xTaskNotify(taskHandles[tMain], nNEW_CODEWORD, eSetValueWithOverwrite);
			
		vTaskSuspend(NULL);
	}
	
}


uint8_t data_out[BYTES_PER_TRANSFER] = { 0 };
uint16_t streamsz = 0;

static void vSendData(void *pvParameters)
{
	size_t xReceivedBytes = 0;
	TickType_t xLastWakeTime, xTickSatu;
	uint16_t ii;
	uint16_t bytes_used = 0;
	uint32_t purestream_sz, dfstream_sz;
	vTaskSuspend(NULL);
	xLastWakeTime = xTaskGetTickCount();

	
	for (;;)
	{
		
		purestream_sz = xStreamBufferBytesAvailable(xPureStream);
		dfstream_sz = xStreamBufferBytesAvailable(xDFStream);
			
		if ((purestream_sz <= OUT_SZ) && (run_flag == 0x00) && (dfstream_sz <= OUT_SZ))
		{
			xTaskNotify(taskHandles[tMain], nFREE_BAND, eSetValueWithOverwrite);
		}

		if ((purestream_sz == 0) && (run_flag != 0x00))
		{
			vTaskSuspend(NULL);
		}
		else
		{	
			if (purestream_sz != 0)
				xReceivedBytes = xStreamBufferReceive(xPureStream, data_out, BYTES_PER_TRANSFER, pdMS_TO_TICKS(100));
			else
				xReceivedBytes = xStreamBufferReceive(xDFStream, data_out, BYTES_PER_TRANSFER, pdMS_TO_TICKS(100));
			
			
			if (xReceivedBytes != 0)
			{
				sendData(data_out, xReceivedBytes);
				xTickSatu = pdMS_TO_TICKS(xReceivedBytes * WAIT_TIME_MS) + 1;
				vTaskDelayUntil(&xLastWakeTime, xTickSatu);
			}
		}
	}
}
		




/* ----- FREERTOS REQUIREMENT FOR STATIC MEMORY USE ----- */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
	StackType_t **ppxIdleTaskStackBuffer,
	uint32_t *pulIdleTaskStackSize)
{
	// If the buffers to be provided to the Idle task are declared inside this
	//function then they must be declared static - otherwise they will be allocated on
	//the stack and so not exists after this function exits. 
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

	// Pass out a pointer to the StaticTask_t structure in which the Idle task's
	//state will be stored. 
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	// Pass out the array that will be used as the Idle task's stack. 
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	// Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	//Note that, as the array is necessarily of type StackType_t,
	//configMINIMAL_STACK_SIZE is specified in words, not bytes. 
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


