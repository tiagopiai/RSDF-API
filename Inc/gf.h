//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: gf.c
//
// DESCRIPTION: Contains the functions necessary to calculate 
// the Galois Field Arithmetics. Used in Reed Solomon encoding.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************

#ifndef GF_H
#define GF_H

#include <stdint.h>

#define pol 0x1D
//#define k 123

typedef uint8_t GF_t;

extern const GF_t gf_antilog[256];
extern const GF_t gf_log[256];
extern const GF_t gf_pow2[256];

GF_t GF_add(GF_t a, GF_t b);
GF_t GF_mul(GF_t a, GF_t b);
GF_t GF_pow(GF_t b, GF_t e);
GF_t GF_inv(GF_t a);
void GF_rootadd(GF_t *p, GF_t a, GF_t sz_p);
void GF_mod(GF_t *a, const GF_t *b, uint16_t sz_a, uint16_t sz_b);
uint32_t mod255(uint32_t a);
#endif // GF_H