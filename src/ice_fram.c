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

// libc
#include <assert.h>

// pico-sdk
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"

// pico-ice-sdk
#include "ice_fram.h"
#include "ice_spi.h"
#include "boards/pico_ice.h"

#define CMD_GET_ID              0x9F
#define CMD_GET_STATUS          0x05
#define CMD_WRITE_ENABLE        0x06
#define CMD_WRITE_DISABLE       0x04
#define CMD_READ                0x03
#define CMD_WRITE               0x02

void ice_fram_write_enable(bool en) {
    const uint8_t command = en ? CMD_WRITE_ENABLE : CMD_WRITE_DISABLE;

    ice_spi_chip_select(ICE_FRAM_CSN_PIN);
    ice_spi_write_blocking(&command, 1);
    ice_spi_chip_deselect(ICE_FRAM_CSN_PIN);
}

void ice_fram_init(bool write_enable) {
    gpio_init(ICE_FRAM_HOLD_PIN);
    gpio_set_dir(ICE_FRAM_HOLD_PIN, GPIO_IN);
    gpio_set_pulls(ICE_FRAM_HOLD_PIN, true, false);

    ice_spi_init();
    ice_spi_init_cs_pin(ICE_FRAM_CSN_PIN, false);

    // Device initialization procedure
    sleep_us(150);
    ice_fram_write_enable(write_enable);
}

void ice_fram_get_id(uint8_t id[4]) {
    const uint8_t command[] = { CMD_GET_ID };

    ice_spi_chip_select(ICE_FRAM_CSN_PIN);
    ice_spi_write_blocking(command, sizeof(command));
    ice_spi_read_blocking(id, 4);
    ice_spi_chip_deselect(ICE_FRAM_CSN_PIN);
}

void ice_fram_write_async(uint32_t addr, const uint8_t *data, size_t data_size,
                          void (*callback)(volatile void *), void *context) {
    uint8_t command[] = { CMD_WRITE, addr >> 16, addr >> 8, addr };

    ice_spi_chip_select(ICE_FRAM_CSN_PIN);
    ice_spi_write_blocking(command, sizeof(command));
    ice_spi_write_async(data, data_size, callback, context);
}

void ice_fram_write_blocking(uint32_t addr, const uint8_t *data, size_t data_size) {
    ice_fram_write_async(addr, data, data_size, NULL, NULL);
    ice_spi_wait_completion();
    ice_spi_chip_deselect(ICE_FRAM_CSN_PIN);
}

void ice_fram_read_async(uint32_t addr, uint8_t *data, size_t data_size,
                          void (*callback)(volatile void *), void *context) {
    uint8_t command[] = { CMD_READ, addr >> 16, addr >> 8, addr };

    ice_spi_chip_select(ICE_FRAM_CSN_PIN);
    ice_spi_write_blocking(command, sizeof(command));
    ice_spi_read_async(data, data_size, callback, context);
}

void ice_fram_read_blocking(uint32_t addr, uint8_t *data, size_t data_size) {
    ice_fram_read_async(addr, data, data_size, NULL, NULL);
    ice_spi_wait_completion();
    ice_spi_chip_deselect(ICE_FRAM_CSN_PIN);
}
