#ifndef PADMA_IMAGE_OPS_H
#define PADMA_IMAGE_OPS_H

#include <omp.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "memo.h"
#include "types.h"
#include "px_ops.h"

///////////////////
//
//	Convertaion
//

/*
	Чтобы уменьшить занимаемую память, можно загружать в постоянную память.
*/

static	inline	double 	d65_to_cielab_func	(	double	);

/// extrern

image_t*	imgu_conv_lrgb_c	(	const image_t*	);		// return copy in linear rgb space
image_t*	imgu_conv_xyz_c		(	const image_t*	);	// NoR
image_t*	imgu_conv_lab_c		(	const image_t*	);	// NoR

void		imgu_conv_lrgb 		(	image_t*	);	// NoR
void		imgu_conv_xyz 		(	image_t*	);	// NoR
void		imgu_conv_lab 		(	image_t*	);	// NoR

double*		select_lightness	(	image_t*						);
double* 	apply_Sobel			(	double*,	pint_t,		pint_t	);	
/////////////////////////
//
//	Realization
//

static inline double d65_to_cielab_func(double component) {
    double sigma = 0.2068965517;

    if (component > pow(sigma, 3)) {
        return cbrt(component);
    }
    return ( component * 29.0 * 29.0 ) / (3.0 * 36.0) + 4.0 / 29.0;
}

image_t* imgu_conv_lrgb_c(const image_t* image) {
	image_t* img = init_img(image->width, image->height);
	if (!img) return NULL;

	plint_t size = image->width * image->height;

    #pragma omp parallel for proc_bind(spread)
    for (plint_t i = 0; i < size; i++) {
    	img->data->comp_1[i] = px_conv_srgb_lrgb(image->data->comp_1[i] / 255.0 );
    	img->data->comp_2[i] = px_conv_srgb_lrgb(image->data->comp_2[i] / 255.0 );
    	img->data->comp_3[i] = px_conv_srgb_lrgb(image->data->comp_3[i] / 255.0 );
    }
    
    return img;
}

double* select_lightness(image_t* image) {
	plint_t size = image->width * image->height;
	double* larr = (double*)malloc(size * sizeof(double));
	double tmp = 0.0;
	if (!larr) return NULL;

	#pragma omp parallel for proc_bind(spread)
	for (plint_t i = 0; i < size; i++) {
		tmp = 0.2126729 * image->data->comp_1[i] + 0.7151522 * image->data->comp_2[i] + 0.0721750 * image->data->comp_3[i];
		larr[i] = 116.0 * d65_to_cielab_func(tmp)  - 16;
	}
	
	return larr;
}

double* apply_Sobel(double* mono, pint_t height, pint_t width) {		// NoO
	double g_x = 0.0;
	double g_y = 0.0;
	double tmp = 0.0;

	if (height < 2 || width < 2) {
		return NULL;
	}

	double* arr = (double*)malloc(width * height * sizeof(double));
	if (!arr) return NULL;

	#pragma omp parallel for proc_bind(spread)
	for (pint_t h_ind = 0; h_ind < height; h_ind++) {
		for (pint_t w_ind = 0; w_ind < width; w_ind++) {
			if ((h_ind == 0 || h_ind == height - 1) || (w_ind == 0 || w_ind == width - 1)) {
				if ((h_ind == 0 || h_ind == height - 1) && (w_ind == 0 || w_ind == width - 1)) {
					arr[h_ind * width +  w_ind] = 0.0;			
				} else {
					if (h_ind == 0) {
						tmp = -2.0 * mono[ (w_ind - 1) ] - 2.0 * mono[ (h_ind + 1) * width + (w_ind - 1) ] + 2.0 * mono[ h_ind * width + (w_ind + 1) ] + 2.0 * mono[ (h_ind + 1) * width + (w_ind + 1) ];
						arr[h_ind * width + w_ind] = fabs(tmp);
					} else if (h_ind == height - 1) {
						tmp = -2.0 * mono[ h_ind * width + (w_ind - 1) ] - 2.0 * mono[ (h_ind - 1) * width + (w_ind - 1) ] + 2.0 * mono[ h_ind * width + (w_ind + 1) ] + 2.0 * mono[ (h_ind - 1) * width + (w_ind + 1) ];
						arr[h_ind * width + w_ind] = fabs(tmp);
					} else if (w_ind == 0) {
						tmp = 2.0 * mono[ (h_ind - 1) * width ] - 2.0 * mono[ (h_ind + 1) * width ] + 2.0 * mono[ (h_ind - 1) * width + 1 ] - 2.0 * mono[ (h_ind + 1) * width + 1 ];
						arr[h_ind * width + w_ind] = fabs(tmp);
					} else {
						tmp = 2.0 * mono[ (h_ind - 1) * width + w_ind ] - 2.0 * mono[ (h_ind + 1) * width + w_ind ] + 2.0 * mono[ (h_ind - 1) * width + (w_ind - 1) ] - 2.0 * mono[ (h_ind + 1) * width + (w_ind - 1) ];
						arr[h_ind * width + w_ind] = fabs(tmp);	
					}
				}
			} else {
				g_x = -1.0 * mono[ (h_ind - 1) * width + (w_ind - 1) ] \
				+ mono[ (h_ind - 1) * width + (w_ind + 1) ] \
				 - 2.0 * mono[ h_ind  * width + (w_ind - 1) ] \
				 + 2.0 * mono[ h_ind  * width + (w_ind + 1) ] \
				 - 1.0 * mono[ (h_ind + 1) * width + (w_ind - 1) ] \
				 + mono[ (h_ind + 1)  * width + (w_ind + 1) ];
				 
				 
				g_y = mono[ (h_ind - 1) * width + (w_ind - 1) ] \
				- 1.0 * mono[ (h_ind + 1) * width + (w_ind - 1) ] \
				+ 2.0 * mono[ (h_ind - 1) * width + w_ind ] - 2.0 \
				* mono[ (h_ind + 1) * width + w_ind ] \
				+ mono[ (h_ind - 1) * width + (w_ind + 1) ] \
				- mono[ (h_ind + 1) * width + (w_ind + 1) ];

				
				arr[h_ind * width + w_ind] = sqrt(g_x * g_x + g_y * g_y);	
			}
		}
	}
	return arr;
}

#endif
