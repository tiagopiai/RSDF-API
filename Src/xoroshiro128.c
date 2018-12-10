/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoroshiro128.h"


static inline uint32_t rotl(const uint32_t x, int pk) 
{
	return (x << pk) | (x >> (32 - pk));
}


static uint32_t s[4];

void seedx(uint32_t pSeed)
{
	int i = 0;

	s[0] = 0;
	s[1] = 0;
	s[2] = 0;
	s[3] = pSeed;
}

void get_seed(uint32_t seed[])
{
	seed[0] = s[0];
	seed[1] = s[1];
	seed[2] = s[2];
	seed[3] = s[3];
}

uint32_t next(void) 
{
	const uint32_t result_starstar = rotl(s[0] * 5, 7) * 9;

	const uint32_t t = s[1] << 9;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 11);

	return result_starstar;
}

