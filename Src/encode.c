//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: encode.c
//
// DESCRIPTION: Encode the data using the RS and DF algorithms.
// Also, has the function to insert the 6 bytes header in the 
// beginning of the message.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************


#include "encode.h"

//6bytes header
void insertheader(uint8_t *buff, uint16_t seed, uint16_t msg_count)
{
	buff[0] = 0x13;
	buff[1] = 0x37;
	buff[2] = 0x13;
	buff[3] = ((uint8_t) seed);
	buff[4] = ((uint8_t)(msg_count >> 8));
	buff[5] = ((uint8_t) msg_count);
}

void encode_rs(uint8_t *p_msg, uint8_t *p_buff_out, uint16_t p_n, uint16_t p_k, uint16_t blocksz)
{
	uint16_t ii = 0, jj = 0, ii2 = 0;
	uint8_t e_msg[255];
	
	for (ii = 0; ii < (blocksz/p_k); ii++)
	{
		rsenc(&p_msg[ii*p_k], p_n, p_k, e_msg);
		
		//interleaving
		ii2 = ii;
		for (jj = 0; jj < p_n; jj++)
		{
			p_buff_out[ii2] = e_msg[jj];
			ii2 = ii2 + (blocksz / p_k);
		}
	}
}

void encode_df(uint8_t *cw, uint16_t nmsgs, uint16_t msg_sz, uint32_t seed)
{
	uint8_t pattern[1000] = { 0 };
	uint16_t ii = 0;
	
	dfrandpattern(pattern, nmsgs, seed);
	
	//clear cw
	for(ii = 0 ; ii < msg_sz ; ii++)
	{
		cw[ii] = 0;
	}
	
	dfgencw(nmsgs, msg_sz, pattern, cw);
	
}