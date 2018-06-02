#ifndef MORSE_H
#define MORSE_H

#include <stdint.h>

#define ABC_LENGTH 36

typedef struct morse_symbol
{
    uint8_t symbol_code;
    uint8_t key;
} morse_symbol_t;

void morse_init_symbol(morse_symbol_t *symbol);
void morse_add_dot(morse_symbol_t *symbol);
void morse_add_dash(morse_symbol_t *symbol);
void morse_set_char(morse_symbol_t *symbol);

#endif /* MORSE_H */
