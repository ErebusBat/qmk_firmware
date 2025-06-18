/* Copyright 2024 @ Keychron(https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* I2C Driver Configuration */
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C1_CLOCK_SPEED 400000
#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2
#define I2C1_OPMODE OPMODE_I2C

/* EEPROM Driver Configuration */
#define EXTERNAL_EEPROM_WP_PIN  A15
#define EXTERNAL_EEPROM_I2C_BASE_ADDRESS 0b10100010
#define EXTERNAL_EEPROM_BYTE_COUNT 2048
#define EXTERNAL_EEPROM_PAGE_SIZE  32
#define EXTERNAL_EEPROM_WRITE_TIME 3

/* Winlock indicator */
#define LED_WIN_LOCK_PIN A4
#define LED_WIN_LOCK_PIN_ON_STATE 1

/* fn1 indicator */
#define LED_FN1_PIN A6
#define LED_FN1_PIN_ON_STATE 1

/* fn2 indicator */
#define LED_FN2_PIN C4
#define LED_FN2_PIN_ON_STATE 1

/* User used eeprom */
#define EECONFIG_USER_DATA_SIZE 1

/* Factory test keys */
#define FN_KEY1 MO(1)

#define DEBOUNCE 10
