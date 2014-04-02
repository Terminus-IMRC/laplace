#include "main.h"

int main()
{
	laplace_init();

	field_random(0);

	laplace();

	return 0;
}
