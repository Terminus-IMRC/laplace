#include "ncpus.h"

int ncpus()
{
	int n=(int)sysconf(_SC_NPROCESSORS_ONLN);

	if(n==-1){
		perror("sysconf(_SC_NPROCESSORS_ONLN)");
		exit(EXIT_FAILURE);
	}

	return n;
}
