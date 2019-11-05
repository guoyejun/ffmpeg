/*
 * Copyright (c) 2019 Wenqian Xing
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libavfilter/dnn/dnn_backend_native_layer_reshape.h"

#define EPSON 0.00001

static int test(void)
{
    DnnLayerReshapeParams params;
    DnnOperand operands[2];
    int32_t input_indexes[1];
    float input[1*1*2*3] = {
        -3, 2.5, 2, -2.1, 7.8, 100
    };
    float *output;

    params.new_numbers = 1;
    params.new_height = 1;
    params.new_weight = 3;
    params.new_channels = 2;

    int32_t new_dims[4];
    new_dims[0] = params.new_numbers;
    new_dims[1] = params.new_height;
    new_dims[2] = params.new_weight;
    new_dims[3] = params.new_channels;

    operands[0].data = input;
    operands[0].dims[0] = 1;
    operands[0].dims[1] = 1;
    operands[0].dims[2] = 2;
    operands[0].dims[3] = 3;
    operands[1].data = NULL;

    input_indexes[0] = 0;
    dnn_execute_layer_reshape(operands, input_indexes, 1, &params);

    output = operands[1].data;
    for (int i = 0; i < 4; i++) {
        if (!operands[1].dims[i] == new_dims[i]) {
            printf("new dims %f are incorrect", i);
            av_freep(&output);
            return 1;
        }
    }

    av_freep(&output);
    return 0;

}

int main(int argc, char **argv)
{
    if (test())
        return 1;

    return 0;
}
