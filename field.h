#ifndef __FIELD_H_INSIDE__
#define __FIELD_H_INSIDE__

#include <omp.h>
/*#include <png.h>*/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

	typedef uint8_t field_t;

	extern field_t *field_1dim;
	extern field_t **field_2dim;
	extern field_t ***field;

#define FIELD_OUTPUT_PNG_FILENAME_FORMAT "out_%ld.png"

	extern void field_init();
	extern void field_output_png(long int turn);
	extern void field_random(long int turn);
	extern void field_square(long int turn);
	extern void field_circle(long int turn);

#endif /* __FIELD_H_INSIDE__ */
