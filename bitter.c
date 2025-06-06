/*
 * File: bitter.c
 * Author: Ragib Asif
 * Email: ragib.asif30@myhunter.cuny.edu
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 *
 * implementation
 *
 */

// Bitter commands:

// >
// increment data pointer and invert bit
//
// <
// decrement data pointer and invert bit
//
// (
// if [p] == 1, proceed to next command, otherwise advance to command after
// matching ')'
//
// )
// go back to matching (
//
// !
// interpreter command: dump memory from 0 to highest value of chunkta pointer
//
// #
// interpreter command: pause program and dump memory

#include "bitter.h"
