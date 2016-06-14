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

#ifndef PYIN_MFUNCS
#define PYIN_MFUNCS

#include "common.h"

typedef struct {
  FP_TYPE a;
  FP_TYPE b;
  FP_TYPE fmax;
  FP_TYPE fmin;
  FP_TYPE fconst;
  int nq;
} pyin_semitone_wrapper;

FP_TYPE* pyin_normalized_betapdf(FP_TYPE a, FP_TYPE b, FP_TYPE from, FP_TYPE to, int number);
FP_TYPE  pyin_beta_b_from_au(FP_TYPE a, FP_TYPE u);

pyin_semitone_wrapper pyin_wrapper_from_frange(FP_TYPE fmin, FP_TYPE fmax);
int      pyin_semitone_from_freq(pyin_semitone_wrapper wrapper, FP_TYPE freq);
FP_TYPE  pyin_freq_from_semitone(pyin_semitone_wrapper wrapper, int semitone);

#define def_singlepass(name, op, init) \
static inline double name(FP_TYPE* src, int n) { \
  FP_TYPE ret = init; \
  for(int i = 0; i < n; i ++) \
    ret = op(ret, src[i]); \
  return ret; \
}

#define def_add(a, b) ((a) + (b))
#define def_max(a, b) ((a) > (b) ? (a) : (b))
#define def_min(a, b) ((a) < (b) ? (a) : (b))

def_singlepass(sumfp, def_add, 0)
def_singlepass(maxfp, def_max, src[0])
def_singlepass(minfp, def_min, src[0])

static inline FP_TYPE* hanning(int n) {
  FP_TYPE* ret = calloc(n, sizeof(FP_TYPE));
  for(int i = 0; i < n; i ++)
    ret[i] = 0.5 * (1 - cos(2 * M_PI * i / (n - 1)));
  return ret;
}

static inline FP_TYPE* hamming(int n) {
  FP_TYPE* ret = calloc(n, sizeof(FP_TYPE));
  for(int i = 0; i < n; i ++)
    ret[i] = 0.54 - 0.46 * cos(2 * M_PI * i / (n - 1));
  return ret;
}

static inline FP_TYPE* blackman_harris(int n) {
  FP_TYPE* ret = calloc(n, sizeof(FP_TYPE));
  const FP_TYPE a0 = 0.35875;
  const FP_TYPE a1 = 0.48829;
  const FP_TYPE a2 = 0.14128;
  const FP_TYPE a3 = 0.01168;
  for(int i = 0; i < n; i ++)
    ret[i] = a0 - a1 * cos(2.0 * M_PI * i / n) +
                  a2 * cos(4.0 * M_PI * i / n) -
                  a3 * cos(6.0 * M_PI * i / n);
  return ret;
}

#endif

