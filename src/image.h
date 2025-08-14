#ifndef PADMA_IMAGE_H
#define PADMA_IMAGE_H

#include "types.h"
#include "memo.h"

#include <stdint.h>
#if defined(_WIN32) || defined(_WIN64)
#include <intrin.h>
#else
#include <cpuid.h>
#endif

/////////////// NO ARM

////////////////////////////////
//
//	CPU Support	
//

static	simd_mode	USAGE_SIMD;
static	char		FMA_ENABLED;		

///////////////////////////
//
//	CPUID
//

static	inline 	void		cpuid				(	int regs[],	int leaf, int subleaf	);
static 	inline	uint64_t	xgetbv				(	void								);
void						image_module_init	(	void								);

///////////////////////////////
//
//	Image
//

image_t*	open_img_path	(	char* 			path	);  // +	CHECKED O2
image_t*	open_img_mem	(	void					);	// NoR
image_t*	cpy_img			(	const image_t*	image	);  // +
void		close_img		(	image_t**		image	);  // +

/////////////////////////////
//
//	Realization
//

///  CPU Support  ///

static inline void cpuid(int regs[], int leaf, int subleaf) {
	#if defined(_WIN32)
	__cpuidex(regs, leaf, subleaf);
	#else
	__cpuid_count(leaf, subleaf, regs[0], regs[1], regs[2], regs[3]);
	#endif
}

static inline uint64_t xgetbv(void) {
	#if defined(_WIN32)
	return _xgetbv(0);
	#else
	uint32_t eax, edx;
	__asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(0));
	return ((uint64_t)edx << 32) | eax;
	#endif
}

void image_module_init(void) {
	char	features_support[6] = {0};
	int info[4];
	uint32_t max_leaf;

	cpuid(info, 0, 0);
	max_leaf = info[0];

	cpuid(info, 1, 0);

	features_support[1] = (info[3] & (1 << 25)) != 0; // SSE
	features_support[2] = (info[3] & (1 << 26)) != 0; // SSE 2
	features_support[3] = (info[2] & (1 << 19)) != 0; // SSE_4_1 
	FMA_ENABLED = (info[2] & (1 << 12)) != 0; // FMA
	

	int avx_support = (info[2] & (1 << 28)) != 0; 
	int osxsave     = (info[2] & (1 << 27)) != 0; 

	if (avx_support && osxsave) {
		uint64_t xcr0 = xgetbv();
		if ((xcr0 & 6) == 6) {
			features_support[4] = 1;  // AVX
		}
	}

	if (max_leaf >= 7) {
		cpuid(info, 7, 0);
		if (features_support[4]) {
			features_support[5] = ((info)[1] & (1 << 5)) != 0;  // AVX 2
		}
	}

	for (int i = 5; i > 0; i--) {
		if (features_support[i]) {
			USAGE_SIMD = i;
			return;
		}
	}
}

///  Image  ///

image_t* cpy_img(const image_t* image) {
	image_t* img = init_img(image->width, image->height);
	if (!img) return NULL;

	double* comp_1 = NULL;
	double* comp_2 = NULL;
	double* comp_3 = NULL;

	plint_t size = image->width * image->height;

    comp_1 = (double*)malloc(size * sizeof(double));
    comp_2 = (double*)malloc(size * sizeof(double));
    comp_3 = (double*)malloc(size * sizeof(double));

   	if (!comp_1) { free_img(img);  return NULL; }
	if (!comp_2) { free_img(img);  return NULL; }
	if (!comp_3) { free_img(img);  return NULL; }

	#pragma omp parallel for proc_bind(spread)
	for ( plint_t iter; iter < size; iter++ ) {
		comp_1[iter] = image->data->comp_1[iter];
		comp_2[iter] = image->data->comp_2[iter];
		comp_3[iter] = image->data->comp_3[iter];
	}

	img->data->comp_1 = comp_1;
	img->data->comp_2 = comp_2;
	img->data->comp_3 = comp_3;

    return img;
}

void close_img(image_t** image) {
	free_img(*image);
	*image = NULL;
}

#endif
