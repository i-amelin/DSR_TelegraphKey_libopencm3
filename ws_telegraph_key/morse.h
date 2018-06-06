/*!
  \file
  \brief Morse Header File
 
  Contains functions using for encoding symbols by Morse codes.
*/

#ifndef MORSE_H
#define MORSE_H

#include <stdint.h>

/*! Number of symbols in the ABC */
#define ABC_LENGTH 36

/*!
  Describes the sequence of dots and dashes for a symbol, and stores a keyboard code for this symbol.
*/
typedef struct morse_symbol
{
    uint8_t symbol_code;
    uint8_t key;
} morse_symbol_t;

/*!
  Initializes the symbol description structure.
  \param symbol Pointer to the symbol description structure
*/
void morse_init_symbol(morse_symbol_t *symbol);

/*!
  Adds a dot to the symbol mask.
  \param symbol Pointer to the symbol description structure
*/
void morse_add_dot(morse_symbol_t *symbol);

/*!
  Adds a dash to the symbol mask.
  \param symbol Pointer to the symbol description structure
*/
void morse_add_dash(morse_symbol_t *symbol);

/*!
  Sets a USB Key Code for symbol's mask.
  \param symbol Pointer to the symbol description structure
*/
void morse_set_char(morse_symbol_t *symbol);

#endif /* MORSE_H */
