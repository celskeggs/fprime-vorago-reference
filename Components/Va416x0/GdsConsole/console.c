/*
 * Copyright 2026 California Institute of Technology
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ======================================================================
// \title  console.c
// \brief  Implementation file for stream redirections
// ======================================================================

#include "console.h"
#include <stdio-bufio.h>

static int console_putc(char c, FILE* file) {
    return gds_console_put_char(c);
}

static int console_getc(FILE* file) {
    // NOTE: getc is not supported
    return _FDEV_EOF;
}

static int console_flush(FILE* file) {
    return gds_console_flush();
}

static FILE stdio_stream = FDEV_SETUP_STREAM(console_putc, console_getc, console_flush, _FDEV_SETUP_RW);

FILE* const stdin = &stdio_stream;
FILE* const stdout = &stdio_stream;
FILE* const stderr = &stdio_stream;
