#ifndef MATFUNC
#define MATFUNC

FP_TYPE * wavread(char* filename, int *fs, int *nbit, int *wav_length);
void wavwrite(double *x, int x_length, int fs, int nbit, char *filename);

#endif

