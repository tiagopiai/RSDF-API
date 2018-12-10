//**********************************************************************************************************************
// Reed Solomon Digital Fountain API
// FILE: storage.c
//
// DESCRIPTION: Here must be writen the code to deal with the
// memory. Initializa, write, read and erase.
//
// AUTHOR: Tiago Guido Piai
// Email: tiago.piai@hotmail.com
// 
//**********************************************************************************************************************

#include "storage.h"
#include "rsDf_api.h"


// Codigo para incializacao das variaveis a serem utilizadas
void initStorage(uint8_t reset_mem)
{
	// Escrita do codigo
	



	// Fim do codigo
	if (reset_mem == 1)
		erase_data();
}

// Funcao que salva os data_sz bytes apontados por *data. Adicionar código para incrementar a variável de contagem do número de bytes armazenados.
uint8_t save_data(uint32_t *data, uint32_t data_sz)
{
	
	
}

// Funcao que retorna o numero de bytes armazenados na memória.
uint32_t getStorageCount()
{
}

// Dado o número da mensagem, tal funcao deve retornar um ponteiro para a mensagem indicada. A mensagem deve ser do tamanho indicado pelo BLOCK_SZ configurado
uint8_t * load_data(uint16_t n_msg)
{
}

// Funcao que apaga a memoria inteira
void erase_data()
{
}


