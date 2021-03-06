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


#include "gf.h"

const GF_t gf_antilog[256] = { 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, };

const GF_t gf_log[256] = { 175, 255, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175, };

const GF_t gf_pow2[256] = { 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, };


// Funcao: GF_add(a,b)
// Descricao: Retorna a soma a+b
GF_t GF_add(GF_t a, GF_t b)
{
	return b ^ a;
}


// Funcao: GF_mul(a, b)
// Descricao: Retorna a multiplicacao de a*b
GF_t GF_mul(GF_t a, GF_t b)
{
	//Otimizado com tabelas log e antilog
	uint32_t temp;
	if (a == 0 || b == 0)
	{
		return 0;
	}
	temp = gf_log[a] + gf_log[b];
	temp = (temp >> 8) + (temp & 0xFF);
	return gf_antilog[temp];
}


// Funcao: GF_pow
// Descricao: Retorna b^e
GF_t GF_pow(GF_t b, GF_t e)
{
	uint32_t a;
	if (e == 0 || b == 1)
	{
		return 1;
	}
	else if (b == 0)
	{
		return 0;
	}
	a = e * gf_log[b];
	a = (a >> 16) + (a & 0xFFFF);
	a = (a >> 8) + (a & 0xFF);
	
	if ((a < (2 * 255)) && (a > 255)) 
		a = a - 255;
	else if (a > (2 * 255)) 
		a = a - (2 * 255);
	return gf_antilog[a];
}

uint32_t mod255(uint32_t a) {
	a = (a >> 16) + (a & 0xFFFF);
	a = (a >> 8) + (a & 0xFF);
	if (a < 255) return a;
	if (a < (2 * 255)) return a - 255;
	return a - (2 * 255);
}


// Funcao: GF_inv(a)
// Descricao: Retorna a inversa de 'a' (a^-1) 
GF_t GF_inv(GF_t a)
{
	if (a == 0)
	{
		return 0;
	}
	return gf_antilog[(255 - gf_log[a])];
}

// Funcao: GF_rootadd(*p, a, sz_p) add_root
// Descricao: Retorna na forma de ponteiro a convolucao do polinomio 'p' com o polinomio de grau 1 com raiz 'a'.
void GF_rootadd(GF_t *p, GF_t a, GF_t sz_p)
{
	GF_t idx = 0;
	GF_t dummy[2] = { 0 };

	dummy[0] = p[0];
	dummy[1] = p[0];
	p[sz_p] = 0;
	for (idx = 1; idx < (sz_p + 1); idx++)
	{
		dummy[0] = dummy[1];
		dummy[1] = p[idx];
		p[idx] = GF_add(p[idx], GF_mul(dummy[0], a));
	}
}

// Funcao: GF_mod(*a, *b, sz_a, sz_b)
// Descricao: Retorna o modulo da divisao de 'a' por 'b' na forma de ponteiro.
void GF_mod(GF_t *a, const GF_t *b, uint16_t sz_a, uint16_t sz_b)
{
	GF_t dummy = 0;
	GF_t idx = 0;

	// loop
	while (sz_a >= sz_b)
	{
		dummy = GF_mul(a[0], GF_inv(b[0]));
		for (idx = 0; idx < sz_b; idx++)
		{
			a[idx] = GF_add(a[idx], GF_mul(b[idx], dummy));
		}
		// shift a
		for (idx = 0; idx < sz_a; idx++)
		{
			a[idx] = a[idx + 1];
		}
		sz_a = sz_a - 1;
		a[sz_a] = 0;
	}
}