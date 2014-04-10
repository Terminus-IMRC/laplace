#include "laplace.h"

long int turn;
int fc, fn;

void laplace_init()
{
	turn=0;
	field_init();

	return;
}

void laplace()
{
	int i, j;
	_Bool field_changed=1;

	for(; field_changed; turn++){
		printf("turn: %ld\n", turn);
		field_output_png(turn);
		fc=turn%2;
		fn=(fc+1)%2;
		field_changed=0;
#pragma omp parallel for schedule(static) shared(field_changed) private(i,j) collapse(2)
		for(i=1; i<1+M; i++)
			for(j=1; j<1+N; j++){
				field[fn][i][j]=(field[fc][i-1][j]+field[fc][i][j-1]+field[fc][i+1][j]+field[fc][i][j+1])/4;
				if((!field_changed) && field[fc][i][j]!=field[fn][i][j])
					field_changed=1;
			}
	}
	field_output_png(turn);
}
