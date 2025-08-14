#ifndef PADMA_CH_OPS_H
#define PADMA_CH_OPS_H

/////////////// NO ARM
//////////////  Проверка на нуди

// #include <immintrin.h>

#include "types.h"
#include <omp.h>
#include <math.h>

// static	inline	void	px_normalize_rgb_avx	(	float*	arr,	pint_t	size	);	// NoR
// static	inline	void	px_srgb_lrgb_avx		(	float* 	arr,	pint_t	size	);  // NoR


///////////////////////
//
//	Convert
//

void	ch_normalize_rgb		(	double*	arr,	pint_t	size	);
void	ch_srgb_lrgb 			(	double*	arr,	pint_t	size	);
void	ch_argb_lrgb			(	double*	arr,	pint_t	size	);

//////////////////////
//
//	Realization
//

void ch_normalize_rgb(double* arr, pint_t size) {
	#pragma omp parallel for proc_bind(spread)
	for (pint_t i = 0; i < size; i++) {
		*(arr + i) = *(arr + i) / 255.0;	
	}
}

void ch_srgb_lrgb(double* arr, pint_t size) {
	#pragma omp parallel for proc_bind(spread)
	for (pint_t i = 0; i < size; i++) {
		if (*(arr + i) > 0.04045) {
	        *(arr + i) =  pow( ( *(arr + i) + 0.055) / 1.055, 2.4);
	    }
	    *(arr + i) = *(arr + i) / 12.92;
	}
}

void ch_srgb

#endif
