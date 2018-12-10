//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: rsdf_api.h
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


#ifndef RSF_API_H
#define RSF_API_H


/* ----- INCLUDES ----- */
#include "FreeRTOS.h"
#include "storage.h"
#include "task.h"
#include "stream_buffer.h"
#include "semphr.h"
#include "timers.h"
#include "string.h"
#include "encode.h"
#include "unpack.h"

/* ----- VARIABLES ----- */

//-- RS SPECS
#define RS_N 255
#define RS_K 193

//-- DF SPECS
#define BLOCK_SZ 3474 //BYTES 

//-- TX PACK SPECS
#define HEADER_SZ 6 //BYTES
#define OUT_SZ (HEADER_SZ + BLOCK_SZ*RS_N/RS_K)
#define TX_BAND		7680 //Bps
#define BYTES_PER_TRANSFER 256
#define WAIT_TIME_MS 1000/TX_BAND


// PROTOTYPES
void initRSF(uint8_t reset_mem);
void storeTrigger(uint8_t* ptr_data, uint16_t data_sz);

extern void sendData(uint8_t *ptr_data, uint32_t n_bytes);

// VALIDATE CHOICES
#if BLOCK_SZ % RS_K != 0
#error BLOCK_SZ deve ser multiplo de RS_K
#endif

#if RS_K > RS_N
#error RS_K deve ser menor que RS_N
#endif

#endif // RSF_API_H