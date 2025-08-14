#ifndef PADMA_PX_OPS_H
#define PADMA_PX_OPS_H

#include <omp.h>
#include <math.h>

#include "types.h"

///////////////////
//
//	Convertaion
//

double	px_conv_srgb_lrgb		(	double	);
double	px_conv_argb_lrgb		(	double	);

/////////////////////////
//
//	Realization
//

double px_conv_srgb_lrgb(double comp) {
	if (comp > 0.04045) {
        return pow( ( comp + 0.055) / 1.055, 2.4);
    }
    return comp / 12.92;
}

double px_conv_argb_lrgb(double comp) {
	return pow( comp, 2.19921875 );
}

#endif
