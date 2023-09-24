/*
 * MIT License
 * 
 * Copyright (c) 2023 tinyVision.ai
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include "boards/pico_ice.h"

// This module is not thread safe.

#ifdef __cplusplus
extern "C" {
#endif

void ice_fram_write_enable(bool en);
void ice_fram_init(bool write_enable);
void ice_fram_get_id(uint8_t id[4]);
void ice_fram_write_async(uint32_t addr, const uint8_t *data, size_t data_size, void (*callback)(volatile void *), void *context);
void ice_fram_write_blocking(uint32_t addr, const uint8_t *data, size_t data_size);
void ice_fram_read_async(uint32_t addr, uint8_t *data, size_t data_size, void (*callback)(volatile void *), void *context);
void ice_fram_read_blocking(uint32_t addr, uint8_t *data, size_t data_size);

#ifdef __cplusplus
}
#endif
