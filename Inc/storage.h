//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: storage.h
//
// DESCRIPTION: Here must be writen the code to deal with the
// memory. Initializa, write, read and erase.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************


#ifndef STORAGE_H
#define STORAGE_H

#include "flash.h"


void initStorage(uint8_t reset_mem);

uint8_t save_data(uint32_t *data, uint32_t data_sz);

uint32_t getStorageCount();

uint8_t * load_data(uint16_t n_msg);

void erase_data();

#endif // STORAGE_H