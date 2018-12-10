//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: df.c
//
// DESCRIPTION: Calculates the patterns responsible to determine
// which blocks will set the codeword using a pseudo number 
// generator (xoroshiro128**). Calculates the DF codeword to be
// send over the channel.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************


#include "df.h"
#include "xoroshiro128.h"
#include "storage.h"

//Funcao: dfgencw(numero de msgs, tamanho da msg, *padrao, *codeword)
//Descricao: Gera a palavra codigo baseado nas mensagens e padrao enviado, retorna no ultimo ponteiro a codeword
void dfgencw(uint16_t nmsg, uint16_t nszmsg, uint8_t *pPattern, uint8_t *cw)
{
	uint16_t i, j;
	uint8_t *ptr_msg;
	for (i = 0; i < nmsg; i++)
	{
		if (pPattern[i])
		{
			ptr_msg = load_data(i);
			for (j = 0; j  < nszmsg; j++)
			{
				cw[j] = cw[j] ^ *((ptr_msg) + j);
			}
		}
	}
}

//Funcao: dfrandpattern(*padrao, numero de mensagem)
//Descricao: retorna um padrao de bits 0/1 no ponteiro pPadrao
void dfrandpattern(uint8_t *pPattern, uint16_t nmsg, uint32_t seed)
{
	uint16_t i, cc;
	uint16_t sum = 0;
	seedx(seed);
	
	cc = (seed & 0x000F);
	
	//5 nexts to well mix the pseudo rng
	next();
	next();
	next();
	next();
	next();
	for (i = 0; i < cc; i++)
	{
		next();
	}
	
	while (sum == 0 | sum == 1)
	{
		sum = 0;
		for (i = 0; i < nmsg; i++)
		{
			pPattern[i] = next() & 0x00000001;
			sum = sum + pPattern[i];
		}
	}
}
