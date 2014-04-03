#include "field.h"

field_t *field_1dim;
field_t **field_2dim;
field_t ***field;

void field_init()
{
	int i;

	field_1dim=(field_t*)calloc(2*(1+M+1)*(1+N+1), sizeof(field_t));
	if(!field_1dim){
		fprintf(stderr, "%s: field_init: malloc: %d: %s\n", __FILE__, 2*(1+M+1)*(1+N+1), strerror(errno));
		exit(EXIT_FAILURE);
	}

	field_2dim=(field_t**)malloc(2*(1+M+1)*sizeof(field_t*));
	if(!field_2dim){
		fprintf(stderr, "%s: field_init: malloc: %d: %s\n", __FILE__, (1+M+1)*(1+N+1), strerror(errno));
		exit(EXIT_FAILURE);
	}

	field=(field_t***)malloc(2*sizeof(field_t**));
	if(!field){
		fprintf(stderr, "%s: field_init: malloc: %d: %s\n", __FILE__, 1+N+1, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for(i=0; i<2*(1+M+1); i++)
		field_2dim[i]=field_1dim+(1+N+1)*i;
	for(i=0; i<2; i++)
		field[i]=field_2dim+(1+M+1)*i;

	return;
}

void field_output_png(long int turn)
{
	int i, j;
	field_t *field_for_print[M];
	field_t *field_for_print_1dim;
	int f;
	FILE *fp;
	char filename[0xff];
	png_structp png_ptr;
	png_infop info_ptr;

	field_for_print_1dim=(field_t*)malloc(M*N*sizeof(field_t));
	if(!field_for_print_1dim){
		fprintf(stderr, "%s: field_output_png: malloc: %s\n", __FILE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	f=turn%2;
	for(i=1; i<1+M; i++){
		field_for_print[i-1]=field_for_print_1dim+N*(i-1);
		for(j=1; j<1+N; j++)
			field_for_print[i-1][j-1]=field[f][i][j];
	}

	sprintf(filename, FIELD_OUTPUT_PNG_FILENAME_FORMAT, turn);
	fp=fopen(filename, "wb");
	if(!fp){
		fprintf(stderr, "%s: field_output_png: fopen: %s: %s\n", __FILE__, filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		fprintf(stderr, "%s: field_output_png: failed on png_create_write_struct\n", __FILE__);
		exit(EXIT_FAILURE);
	}

	info_ptr=png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fprintf(stderr, "%s: field_output_png: failed on png_create_info_struct\n", __FILE__);
		exit(EXIT_FAILURE);
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, N, M, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(png_ptr, Z_NO_COMPRESSION);
	png_write_info(png_ptr, info_ptr);
	png_write_image(png_ptr, (png_bytepp)field_for_print);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(fp);

	free(field_for_print_1dim);

	return;
}

void field_random(long int turn)
{
	int i, j;
	int f=turn%2;

#pragma omp parallel for schedule(static) shared(field) private(i,j)
	for(i=1; i<1+M; i++)
		for(j=1; j<1+N; j++)
			field[f][i][j]=rand()%((int)((uint8_t)~0)+1);

	return;
}

void field_square(long int turn)
{
	int i, j;
	int f=turn%2;

#pragma omp parallel for schedule(static) shared(field) private(i,j)
	for(i=1.0/4*M; i<3.0/4*M; i++)
		for(j=1.0/4*N; j<3.0/4*N; j++)
			field[f][i][j]=~0;
	
	return;
}

void field_circle(long int turn)
{
	int i, j, k;
	int f=turn%2;

#pragma omp parallel for schedule(static) shared(field) private(i,j,k)
	for(i=1; i<1+M; i++)
		for(j=1; j<1+N; j++)
			for(k=0; k<(M<N?M:N)/4; k++)
				if(pow(i-(1+M+1)/2, 2)+pow(j-(1+N+1)/2, 2)<=pow(k, 2))
					field[f][i][j]=(uint8_t)~0;

	return;
}
