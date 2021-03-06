// Copyright 2019 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <xnnpack/gavgpool.h>
#include <xnnpack/math.h>


void xnn_f32_gavgpool_ukernel_up7__scalar(
    size_t m,
    size_t n,
    const float* input,
    size_t input_stride,
    const float* zero,
    float* output,
    const union xnn_f32_avgpool_params params[restrict static 1])
{
  assert(m != 0);
  assert(m <= 7);
  assert(n != 0);

  const float* i0 = input;
  const float* i1 = (const float*) ((uintptr_t) i0 + input_stride);
  if (m < 2) {
    i1 = zero;
  }
  const float* i2 = (const float*) ((uintptr_t) i1 + input_stride);
  if (m <= 2) {
    i2 = zero;
  }
  const float* i3 = (const float*) ((uintptr_t) i2 + input_stride);
  if (m < 4) {
    i3 = zero;
  }
  const float* i4 = (const float*) ((uintptr_t) i3 + input_stride);
  if (m <= 4) {
    i4 = zero;
  }
  const float* i5 = (const float*) ((uintptr_t) i4 + input_stride);
  if (m < 6) {
    i5 = zero;
  }
  const float* i6 = (const float*) ((uintptr_t) i5 + input_stride);
  if (m <= 6) {
    i6 = zero;
  }

  const float vmultiplier = params->scalar.multiplier;
  const float voutput_min = params->scalar.output_min;
  const float voutput_max = params->scalar.output_max;
  do {
    const float vi0 = *i0++;
    const float vi1 = *i1++;
    const float vi2 = *i2++;
    const float vi3 = *i3++;
    const float vi4 = *i4++;
    const float vi5 = *i5++;
    const float vi6 = *i6++;

    const float vsum01 = vi0 + vi1;
    const float vsum23 = vi2 + vi3;
    const float vsum45 = vi4 + vi5;

    const float vsum016 = vsum01 + vi6;
    const float vsum2345 = vsum23 + vsum45;

    const float vsum = vsum016 + vsum2345;

    float vout = vsum * vmultiplier;
    vout = math_max_f32(vout, voutput_min);
    vout = math_min_f32(vout, voutput_max);

    *output++ = vout;
  } while (--n != 0);
}
