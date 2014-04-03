#include "main.h"

int main()
{
	laplace_init();

	field_random(turn);
	field_square(turn);
	field_circle(turn);

	laplace();

	return 0;
}
