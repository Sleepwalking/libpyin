/*
libpyin
===

Copyright (c) 2015, Kanru Hua
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include "../external/matlabfunctions.h"
#include "../math-funcs.h"
#include "../pyin.h"

int main(int argc, char** argv) {
  if(argc < 2) {
    fprintf(stderr, "Missing argument.\n");
    return 1;
  }

  int fs = 0;
  int nbit = 0;
  int nx = 0;
  FP_TYPE* x = wavread(argv[1], & fs, & nbit, & nx);
  
  pyin_config param = pyin_init(ceil(fs * 0.005));
  param.fmin = 50.0;
  param.fmax = 800.0;
  param.trange = pyin_trange(param.nq, param.fmin, param.fmax);
  param.nf = ceil(fs * 0.025);
  param.w = param.nf / 3;
  
  int nfrm = 0;
  FP_TYPE* f0 = pyin_analyze(param, x, nx, fs, & nfrm);
  
  FP_TYPE min = param.fmax;
  FP_TYPE max = param.fmin;
  for(int i = 0; i < nfrm; i ++) {
    if(f0[i] > 0) {
      if(f0[i] < min) min = f0[i];
      if(f0[i] > max) max = f0[i];
    }
  }
  
  for(int i = 0; i < nfrm; i ++) {
    printf("%7.2f  ", f0[i]);
    int l = f0[i] > 0 ? ((f0[i] - min) / (max - min)) * 40 + 5 : 0;
    for(int j = 0; j < l; j ++)
      printf("█");
    printf("\n");
  }
  
  free(f0);
  free(x);
  return 0;
}

