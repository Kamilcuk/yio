/**
 * @file
 * @date 2020-08-17
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */
#pragma once

// weak callbacks need to be provided by user
char *_yIO_ryu_f2fixed(float f, uint32_t precision);
char *_yIO_ryu_f2exp(float f, uint32_t precision);
char *_yIO_ryu_d2fixed(double f, uint32_t precision);
char *_yIO_ryu_d2exp(double f, uint32_t precision);
char *_yIO_ryu_l2fixed(double f, uint32_t precision);
char *_yIO_ryu_l2exp(double f, uint32_t precision);


