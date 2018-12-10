//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: df.h
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


#include <stdint.h>

void dfrandpattern(uint8_t *pPattern, uint16_t nmsg, uint32_t seed);
void dfgencw(uint16_t nmsg, uint16_t nszmsg, uint8_t *pPattern, uint8_t *cw);