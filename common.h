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

#ifndef PYIN_COMMON
#define PYIN_COMMON

#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifndef M_PI
  #define M_PI 3.1415926535897932385
#endif
#define EPS 0.0000000001

static inline void** malloc2d(size_t m, size_t n, size_t size) {
  void** ret = calloc(m, sizeof(void*));
  for(size_t i = 0; i < m; i++)
    ret[i] = calloc(n, size);
  return ret;
}

static inline void free2d(void** ptr, size_t m) {
  for(size_t i = 0; i < m; i ++)
    free(ptr[i]);
  free(ptr);
}

static inline FP_TYPE* fetch_frame(FP_TYPE* x, int nx, int center, int nf) {
  FP_TYPE* y = calloc(nf, sizeof(FP_TYPE));
  for(int i = 0; i < nf; i ++) {
    int isrc = center + i - nf / 2;
    y[i] = (isrc >= 0 && isrc < nx) ? x[isrc] : 0;
  }
  return y;
}

#endif

