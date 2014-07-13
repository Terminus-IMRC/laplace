#include "main.h"
#include <sys/time.h>

int main()
{
	int i;
	int e;
	pthread_t *thr;
	struct timeval start, end;

	laplace_init();

	field_random(turn);
	field_square(turn);
	field_circle(turn);

	thr=(pthread_t*)malloc(nthreads*sizeof(pthread_t));
	assert(thr);

	gettimeofday(&start, NULL);
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
	gettimeofday(&end, NULL);

	printf("time: %g\n", (end.tv_sec+end.tv_usec*1e-6)-(start.tv_sec+start.tv_usec*1e-6));

	return 0;
}
