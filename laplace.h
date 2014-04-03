#ifndef __LAPLACE_H_INSIDE__
#define __LAPLACE_H_INSIDE__

#include <omp.h>
#include <stdio.h>
#include "field.h"

	extern long int turn;
	extern int fc, fn;

	void laplace_init();
	void laplace();

#endif /* __LAPLACE_H_INSIDE__ */
