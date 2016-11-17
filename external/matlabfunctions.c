//-----------------------------------------------------------------------------
// Copyright 2012-2015 Masanori Morise. All Rights Reserved.
// Author: mmorise [at] yamanashi.ac.jp (Masanori Morise)
//
// Matlab functions implemented for WORLD
// Since these functions are implemented as the same function of Matlab,
// the source code does not follow the style guide (Names of variables
// and functions).
// Please see the reference of Matlab to show the usage of functions.
// Caution:
//   Since these functions (wavread() and wavwrite()) are roughly implemented,
//   we recommend more suitable functions provided by other organizations.
//-----------------------------------------------------------------------------

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define false 0
#define true  1
typedef int bool;

static inline int MyMax(int x, int y) {
  return x > y ? x : y;
}

static inline int MyMin(int x, int y) {
  return x < y ? x : y;
}

//-----------------------------------------------------------------------------
// CheckHeader() checks the .wav header. This function can only support the
// monaural wave file. This function is only used in waveread().
//-----------------------------------------------------------------------------
static bool CheckHeader(FILE *fp) {
  char data_check[5];
  fread(data_check, 1, 4, fp);  // "RIFF"
  data_check[4] = '\0';
  if (0 != strcmp(data_check, "RIFF")) {
    printf("RIFF error.\n");
    return false;
  }
  fseek(fp, 4, SEEK_CUR);
  fread(data_check, 1, 4, fp);  // "WAVE"
  if (0 != strcmp(data_check, "WAVE")) {
    printf("WAVE error.\n");
    return false;
  }
  fread(data_check, 1, 4, fp);  // "fmt "
  if (0 != strcmp(data_check, "fmt ")) {
    printf("fmt error.\n");
    return false;
  }
  fread(data_check, 1, 4, fp);  // 1 0 0 0
  if (!(16 == data_check[0] && 0 == data_check[1] &&
      0 == data_check[2] && 0 == data_check[3])) {
    printf("fmt (2) error.\n");
    return false;
  }
  fread(data_check, 1, 2, fp);  // 1 0
  if (!(1 == data_check[0] && 0 == data_check[1])) {
    printf("Format ID error.\n");
    return false;
  }
  fread(data_check, 1, 2, fp);  // 1 0
  if (!(1 == data_check[0] && 0 == data_check[1])) {
    printf("This function cannot support stereo file\n");
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------------
// GetParameters() extracts fp, nbit, wav_length from the .wav file
// This function is only used in wavread().
//-----------------------------------------------------------------------------
static bool GetParameters(FILE *fp, int *fs, int *nbit, int *wav_length) {
  char data_check[5] = {0};
  data_check[4] = '\0';
  unsigned char for_int_number[4];
  fread(for_int_number, 1, 4, fp);
  *fs = 0;
  for (int i = 3; i >= 0; --i) *fs = *fs * 256 + for_int_number[i];
  // Quantization
  fseek(fp, 6, SEEK_CUR);
  fread(for_int_number, 1, 2, fp);
  *nbit = for_int_number[0];

  // Skip until "data" is found. 2011/03/28
  while (0 != fread(data_check, 1, 1, fp)) {
    if (data_check[0] == 'd') {
      fread(&data_check[1], 1, 3, fp);
      if (0 != strcmp(data_check, "data")) {
        fseek(fp, -3, SEEK_CUR);
      } else {
        break;
      }
    }
  }
  if (0 != strcmp(data_check, "data")) {
    printf("data error.\n");
    return false;
  }

  fread(for_int_number, 1, 4, fp);  // "data"
  *wav_length = 0;
  for (int i = 3; i >= 0; --i)
    *wav_length = *wav_length * 256 + for_int_number[i];
  *wav_length /= (*nbit / 8);
  return true;
}

void wavwrite(FP_TYPE *x, int x_length, int fs, int nbit, char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("File cannot be opened.\n");
    return;
  }

  char text[4] = {'R', 'I', 'F', 'F'};
  uint32_t long_number = 36 + x_length * 2;
  fwrite(text, 1, 4, fp);
  fwrite(&long_number, 4, 1, fp);

  text[0] = 'W';
  text[1] = 'A';
  text[2] = 'V';
  text[3] = 'E';
  fwrite(text, 1, 4, fp);
  text[0] = 'f';
  text[1] = 'm';
  text[2] = 't';
  text[3] = ' ';
  fwrite(text, 1, 4, fp);

  long_number = 16;
  fwrite(&long_number, 4, 1, fp);
  int16_t short_number = 1;
  fwrite(&short_number, 2, 1, fp);
  short_number = 1;
  fwrite(&short_number, 2, 1, fp);
  long_number = fs;
  fwrite(&long_number, 4, 1, fp);
  long_number = fs * 2;
  fwrite(&long_number, 4, 1, fp);
  short_number = 2;
  fwrite(&short_number, 2, 1, fp);
  short_number = 16;
  fwrite(&short_number, 2, 1, fp);

  text[0] = 'd';
  text[1] = 'a';
  text[2] = 't';
  text[3] = 'a';
  fwrite(text, 1, 4, fp);
  long_number = x_length * 2;
  fwrite(&long_number, 4, 1, fp);

  int16_t tmp_signal;
  for (int i = 0; i < x_length; ++i) {
    tmp_signal = (int16_t)(MyMax(-32768,
        MyMin(32767, (int)(x[i] * 32767))));
    fwrite(&tmp_signal, 2, 1, fp);
  }

  fclose(fp);
}

FP_TYPE * wavread(char* filename, int *fs, int *nbit, int *wav_length) {
  FILE *fp = fopen(filename, "rb");
  if (NULL == fp) {
    printf("File not found.\n");
    return NULL;
  }

  if (CheckHeader(fp) == false) {
    fclose(fp);
    return NULL;
  }

  if (GetParameters(fp, fs, nbit, wav_length) == false) {
    fclose(fp);
    return NULL;
  }

  FP_TYPE *waveform = calloc(*wav_length, sizeof(FP_TYPE));
  if (waveform == NULL) return NULL;

  int quantization_byte = *nbit / 8;
  FP_TYPE zero_line = pow(2.0, *nbit - 1);
  FP_TYPE tmp, sign_bias;
  unsigned char for_int_number[4];
  for (int i = 0; i < *wav_length; ++i) {
    sign_bias = tmp = 0.0;
    fread(for_int_number, 1, quantization_byte, fp);  // "data"
    if (for_int_number[quantization_byte-1] >= 128) {
      sign_bias = pow(2.0, *nbit - 1);
      for_int_number[quantization_byte - 1] =
        for_int_number[quantization_byte - 1] & 0x7F;
    }
    for (int j = quantization_byte - 1; j >= 0; --j)
      tmp = tmp * 256.0 + for_int_number[j];
    waveform[i] = (tmp - sign_bias) / zero_line;
  }
  fclose(fp);
  return waveform;
}

