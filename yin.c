/*
libpyin
===

Copyright (c) 2015-2017, Kanru Hua
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

#include "math-funcs.h"

static FP_TYPE diffsum(FP_TYPE* x, int t, int w) {
  FP_TYPE ret = 0;
  for(int i = 0; i < w; i ++)
    ret += (x[i] - x[t + i]) * (x[i] - x[t + i]);
  return ret;
}

FP_TYPE* pyin_yincorr(FP_TYPE* x, int nx, int w) {
  int nd = nx - w;
  FP_TYPE* d = calloc(nd, sizeof(FP_TYPE));

  d[0] = 1.0;
  FP_TYPE norm_factor = 0;
  for(int i = 1; i < nd; i ++) {
    FP_TYPE diff = diffsum(x, i, w);
    norm_factor += diff;
    d[i] = diff * i / (norm_factor <= EPS ? EPS: norm_factor);
  }
  
  return d;
}

FP_TYPE pyin_qinterp(FP_TYPE* x, int k, FP_TYPE* y) {
  FP_TYPE a, b, c, a1, a2, x1;
  a = x[k - 1];
  b = x[k + 0];
  c = x[k + 1];
  a1 = (a + c) / 2.0 - b;
  a2 = c - b - a1;
  x1 = - a2 / a1 * 0.5;
  
  x1 = (fabs(x1) < 1.0) ? x1 : 0; // in case we get some x outside of [k-1, k+1]
  
  if(y != NULL)
    *y = (FP_TYPE)k + x1;
  return a1 * x1 * x1 + a2 * x1 + b;
}
