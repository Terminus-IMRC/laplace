#include "main.h"

int main()
{
	laplace_init();

	field_random(0);
	field_square(turn);

	laplace();

	return 0;
}
