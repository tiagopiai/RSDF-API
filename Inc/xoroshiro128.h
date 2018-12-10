#include <stdint.h>

static inline uint32_t rotl(const uint32_t x, int pk);

uint32_t next(void);

void seedx(uint32_t pSeed);

void get_seed(uint32_t seed[]);
