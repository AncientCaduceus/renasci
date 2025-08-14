#ifndef PADMA_TYPES_H
#define PADMA_TYPES_H

#include <stdint.h>

typedef uint64_t plint_t;
typedef uint32_t pint_t;
typedef uint16_t psint_t;

///////////////////////////
//
//	Enums
//

typedef enum {
	SIMD_UND,
	SIMD_SSE,
	SIMD_SSE2,
	SIMD_SSE4_1,
	SIMD_AVX,
	SIMD_AVX2
} simd_mode;

typedef enum {
	SCHEME_UND,
	SCHEME_S_RGB,
	SCHEME_A_RGB,
	SCHEME_L_RGB,
	SCHEME_XYZ,
	SCHEME_LAB
} image_scheme;

///////////////////////////
//
//	Structs
//

typedef struct {		// Changed. 1. Array of Structures -> SoA, 2. 4 components -> 3.
	double* comp_1;
	double*	comp_2;
	double*	comp_3;
} image_data_t;

typedef struct {
	image_scheme sch;
	pint_t width, height;		// Changed.
	image_data_t* data;
} image_t;

#endif
