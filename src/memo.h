#ifndef PADMA_MEMO_H
#define PADMA_MEMO_H

#include "types.h"
#include "stddef.h"

/////////////// NO ARM

#if defined(_WIN32)  || defined(_WIN64)
	#include <malloc.h>
	#define ALIGNED_ALLOC(size, align) _aligned_malloc(size, align)
	#define ALIGNED_FREE(ptr) _aligned_free(ptr)
#else
	#include <stdlib.h>
	#define ALIGNED_ALLOC(size, align) ({ void* p = NULL; posix_memalign(&p, align, size) == 0 ? p : NULL; })
 	#define ALIGNED_FREE(ptr) free(ptr) 
#endif

#define IS_ALIGNED(ptr, align) (((uintptr_t)(ptr) % (align)) == 0)

////////////////////////////////
//
//	Allocation functions
//

void*	avx_alloc	(	size_t	size 	);		// Aligned
void*	sse_alloc	(	size_t 	size 	);		// Aligned

void*	zero_alloc	(	size_t	size,	pint_t	align 	);	// NoR

////////////////////////////////
//
//	Init functions
//

image_data_t*	init_img_data	(	plint_t size	);
image_t*		init_img  		(	pint_t	width,		pint_t	height	);

///////////////////////////////
//
//	Free
//

void	free_img_data	(	image_data_t*	img_data	);
void	free_img 		(	image_t*		img			);

///////////////////////////////
//
//	Realization
//

///  Alloc  ///

void* avx_alloc(size_t size) {
	void* tmp = ALIGNED_ALLOC(size, 32);
	return tmp;
}

void* sse_alloc(size_t size) {
	void* tmp = ALIGNED_ALLOC(size, 16);
	return tmp;
}

///  Free  ///

void free_img_data(image_data_t* img_data) {
	free(img_data->comp_1);
	free(img_data->comp_2);
	free(img_data->comp_3);
	free(img_data);
}

void free_img(image_t* img) {
	free_img_data(img->data);
	free(img);
}

///  Init  ///

image_data_t* init_img_data(plint_t size) {	// CHECKED O2
	image_data_t* data = (image_data_t*)malloc(sizeof(image_data_t));
	if (!data) return NULL;
	
	data->comp_1 = (double*)malloc(size * sizeof(double));
	data->comp_2 = (double*)malloc(size * sizeof(double));
	data->comp_3 = (double*)malloc(size * sizeof(double));

    if (!data->comp_1) { free(data);  return NULL; }
	if (!data->comp_2) { free(data);  return NULL; }		// ! ADD FREE
	if (!data->comp_3) { free(data);  return NULL; }		// ! ADD FREE
	
	return data;
}

image_t* init_img(pint_t width, pint_t height) { // CHECKED O2
	plint_t size = width * height;
	
	image_data_t* data = init_img_data(size);
	if (!data) return NULL;
	
	image_t* img = (image_t*)malloc(sizeof(image_t));
	if (!img) { free_img_data(data); return NULL; }

	img->sch = SCHEME_UND;
	img->width = width;
	img->height = height;
	img->data = data;
	
	return img;
}

#endif
