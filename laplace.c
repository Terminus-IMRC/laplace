#include "laplace.h"

long int turn;
int fc, fn;
uint8_t *field_changed_g;
pthread_barrier_t allbrr;
int *swait, *swait2;
int nthreads;

void laplace_init()
{
	int i;
	int e;

	nthreads=ncpus();

	printf("nthreads: %d\n", nthreads);

	turn=0;
	field_init();

	e=pthread_barrier_init(&allbrr, NULL, nthreads);
	if(e!=0){
		errno=e;
		perror("pthread_barrier_init");
		exit(EXIT_FAILURE);
	}

	field_changed_g=(uint8_t*)calloc(nthreads, sizeof(uint8_t));
	assert(field_changed_g);

	swait=(int*)malloc(nthreads*sizeof(int));
	assert(swait);

	swait2=(int*)malloc(nthreads*sizeof(int));
	assert(swait2);

	for(i=0; i<nthreads; i++){
		swait[i]=-1;
		swait2[i]=-1;
	}

	return;
}

void* laplace(void *arg)
{
	int i, j;
	uint8_t field_changed;
	int tid=(int)arg;

	/*printf("%d/%d: hello\n", tid, nthreads);*/

	for(;;){
		if(tid==0){
			printf("turn: %ld\n", turn);
			field_output_png(turn);
		}
		fc=turn%2;
		fn=(fc+1)%2;
		field_changed=0;
		for(i=1; i<1+M; i++)
			for(j=1; j<1+N; j++){
				field[fn][i][j]=(field[fc][i-1][j]+field[fc][i][j-1]+field[fc][i+1][j]+field[fc][i][j+1])/4;
				if((!field_changed) && field[fc][i][j]!=field[fn][i][j])
					field_changed=1;
			}

		/*printf("%d/%d: gathering field_changed\n", tid, nthreads);*/

		field_changed_g[tid]=field_changed;
		/*for(i=0; i<(int)ceil(log(nthreads)/log(2)); i++){*/
		if(tid==0){
			printf("log2(%d)=%g, ceil(log2(%d))=%f, (int)ceil(log2(%d))=%d\n", nthreads, log(nthreads)/log(2), nthreads, ceil(log(nthreads)/log(2)), nthreads, (int)ceil(log(nthreads)/log(2)));
		}
		for(i=0; i<(int)ceil(log(nthreads)/log(2)); i++){
			if(tid%(int)pow(2, i)!=0)
				break;
			if((int)(tid/pow(2, i))%2==0){
				if(tid+pow(2, i)>=nthreads)
					continue;
				swait[tid]=tid+pow(2, i);
				for(;;){
					if(swait[(int)(tid+pow(2, i))]==tid)
						break;
				}
				swait2[tid]=tid+pow(2, i);
				for(;;){
					if(swait2[(int)(tid+pow(2, i))]==tid)
						break;
				}
				swait[tid]=-1;
				swait2[tid]=-1;
				field_changed_g[tid]=(field_changed_g[tid]!=0 || field_changed_g[(int)(tid+pow(2, i))]!=0)?1:0;
			}else{
				swait[tid]=tid-pow(2, i);
				for(;;){
					if(swait[(int)(tid-pow(2, i))]==tid)
						break;
				}
				swait2[tid]=tid-pow(2, i);
				for(;;){
					if(swait2[(int)(tid-pow(2, i))]==tid)
						break;
				}
				swait[tid]=-1;
				swait2[tid]=-1;
			}
		}

		printf("%d/%d: performing allbarrier\n", tid, nthreads);
		fflush(stdout);
		pthread_barrier_wait(&allbrr);
		printf("%d/%d: barrier done\n", tid, nthreads);
		fflush(stdout);
		if(field_changed_g[0]==0)
			break;
		printf("%d/%d: not breaking\n", tid, nthreads);
		fflush(stdout);
		pthread_exit(NULL);
		pthread_barrier_wait(&allbrr);
	}
	if(tid==0)
		field_output_png(turn);
	printf("%d/%d: broke\n", tid, nthreads);
		fflush(stdout);

	return NULL;
}

