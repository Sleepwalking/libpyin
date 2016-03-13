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

#include "math-funcs.h"
#include <math.h>

FP_TYPE* pyin_normalized_betapdf(FP_TYPE a, FP_TYPE b, FP_TYPE from, FP_TYPE to, int number) {
  FP_TYPE* ret = calloc(number, sizeof(FP_TYPE));
  FP_TYPE gallop = (to - from) / number;
  for(int i = 0; i < number; i ++) {
    FP_TYPE x = from + gallop * i;
    ret[i] = pow(x, a - 1) * pow(1 - x, b - 1);
  }
  for(int i = number - 2; i > 0; i --) // enforce monotonicity
    if(ret[i] < ret[i + 1]) ret[i] = ret[i + 1];
  FP_TYPE norm_factor = 1.0 / sumfp(ret, number);
  for(int i = 0; i < number; i ++)
    ret[i] *= norm_factor;
  return ret;
}

FP_TYPE  pyin_beta_b_from_au(FP_TYPE a, FP_TYPE u) {
  return (a - a * u) / u;
}

#define FCONST    127.09
#define NQDEFAULT 480
pyin_semitone_wrapper pyin_wrapper_from_frange(FP_TYPE fmin, FP_TYPE fmax) {
  pyin_semitone_wrapper ret;
  FP_TYPE k1 = log2(fmax / FCONST);
  FP_TYPE k2 = log2(fmin / FCONST);
  ret.a = k1 - k2;
  ret.b = k2 / (k2 - k1);
  ret.fmax = fmax;
  ret.fmin = fmin;
  ret.nq = NQDEFAULT;
  ret.fconst = FCONST;
  return ret;
}

int pyin_semitone_from_freq(pyin_semitone_wrapper wrapper, FP_TYPE freq) {
  FP_TYPE semitone_fp = log2(freq / wrapper.fconst) * wrapper.nq / wrapper.a + wrapper.b * wrapper.nq;
  return floor(semitone_fp);
}

FP_TYPE  pyin_freq_from_semitone(pyin_semitone_wrapper wrapper, int semitone) {
  FP_TYPE semitone_fp = semitone;
  return pow(2, (semitone_fp - wrapper.b * wrapper.nq) * wrapper.a / wrapper.nq) * wrapper.fconst;
}

