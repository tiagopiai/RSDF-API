//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: encode.h
//
// DESCRIPTION: Encode the data using the RS and DF algorithms.
// Also, has the function to insert the 6 bytes header in the 
// beginning of the message.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************


#ifndef ENCODE_H
#define ENCODE_H
#include "rs.h"
#include "df.h"
#include "FreeRTOS.h"

void insertheader(uint8_t *buff, uint16_t seed, uint16_t msg_count);

void encode_rs(uint8_t *p_msg, uint8_t *p_buff_out, uint16_t p_n, uint16_t p_k, uint16_t blocksz);

void encode_df(uint8_t *cw, uint16_t nmsgs, uint16_t msg_sz, uint32_t seed);

#endif // ENCODE_H