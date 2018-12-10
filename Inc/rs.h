//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: rs.c
//
// DESCRIPTION: Calculate the RS codeword given the data with
// size K. Returns the codeword with size N. Supports only 8
// bits symbols.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************

#ifndef RS_H
#define RS_H

#include "gf.h"

void initRSenc(uint16_t p_n, uint16_t p_k);

void rsenc(GF_t *p_msg, uint16_t p_n, uint16_t p_k, GF_t *p_r);

#endif // RS_H