#include "main.h"

int main()
{
	int i;
	int e;
	pthread_t *thr;

	laplace_init();

	field_random(turn);
	field_square(turn);
	field_circle(turn);

	thr=(pthread_t*)malloc(nthreads*sizeof(pthread_t));
	assert(thr);

	for(i=0; i<nthreads; i++){
		e=pthread_create(&(thr[i]), NULL, laplace, (void*)i);
		if(e!=0){
			errno=e;
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for(i=0; i<nthreads; i++){
		e=pthread_join(thr[i], NULL);
		if(e!=0){
			errno=e;
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
