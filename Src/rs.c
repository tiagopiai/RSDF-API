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


#include "rs.h"


static uint16_t rsk, rsn;
static GF_t G_enc[255];

void initRSenc(uint16_t p_n, uint16_t p_k)
{
	uint16_t i = 0;
	
	rsk = p_k;
	rsn = p_n;
	
	G_enc[0] = 1;
	for (i = 0; i < rsn - rsk; i++)
	{
		GF_rootadd(G_enc, gf_pow2[(i + 1)], (i + 1));
	}
	
}

void rsenc(GF_t *p_msg, uint16_t rsn, uint16_t rsk, GF_t *p_r)
{
	GF_t loc_msg[255] = { 0 };
	uint16_t i = 0;
	//Gera polinomio encoder e copia msg
	
	for(i = 0 ; i < rsk ; i++)
	{
		loc_msg[i] = p_msg[i];
		p_r[i] = p_msg[i];
	}
	GF_mod(loc_msg, G_enc, rsn, (rsn - rsk + 1));
	for (i = 0; i <= (rsn - rsk); i++)
	{
		p_r[rsk + i] = loc_msg[i];
	}
	
}