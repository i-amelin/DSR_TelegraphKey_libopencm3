#include "morse.h"

/*!
  USB Key Code for each letter and digit, and mask of dots and dashes sequence.
*/
static const morse_symbol_t symbols[] =
{
    { 0x04, 0b00000101 }, // A
    { 0x05, 0b00011000 }, // B
    { 0x06, 0b00011010 }, // C
    { 0x07, 0b00001100 }, // D
    { 0x08, 0b00000010 }, // E
    { 0x09, 0b00000110 }, // F
    { 0x0A, 0b00001110 }, // G
    { 0x0B, 0b00010000 }, // H
    { 0x0C, 0b00000100 }, // I
    { 0x0D, 0b00010111 }, // J
    { 0x0E, 0b00001101 }, // K
    { 0x0F, 0b00010100 }, // L
    { 0x10, 0b00000111 }, // M
    { 0x11, 0b00000110 }, // N
    { 0x12, 0b00001111 }, // O
    { 0x13, 0b00010110 }, // P
    { 0x14, 0b00011101 }, // Q
    { 0x15, 0b00001010 }, // R
    { 0x16, 0b00001000 }, // S
    { 0x17, 0b00000011 }, // T
    { 0x18, 0b00001001 }, // U
    { 0x19, 0b00010001 }, // V
    { 0x1A, 0b00001011 }, // W
    { 0x1B, 0b00011001 }, // X
    { 0x1C, 0b00011011 }, // Y
    { 0x1D, 0b00011100 }, // Z
    
    { 0x1E, 0b00101111 }, // 1
    { 0x1F, 0b00100111 }, // 2
    { 0x20, 0b00100011 }, // 3
    { 0x21, 0b00100001 }, // 4
    { 0x22, 0b00100000 }, // 5
    { 0x23, 0b00110000 }, // 6
    { 0x24, 0b00111000 }, // 7
    { 0x25, 0b00111100 }, // 8
    { 0x26, 0b00111110 }, // 9
    { 0x27, 0b00111111 }  // 0
};

void morse_init_symbol(morse_symbol_t *symbol)
{
    symbol->symbol_code = 0;
    symbol->key = 1;
}

void morse_add_dot(morse_symbol_t *symbol)
{
    uint8_t key = symbol->key;
    key <<= 1;
    symbol->key = key;
}

void morse_add_dash(morse_symbol_t *symbol)
{
    uint8_t key = symbol->key;
    key <<= 1;
    key |= 1;
    symbol->key = key;
}

void morse_set_char(morse_symbol_t *symbol)
{
    int i = 0;
    int is_set = 0;

    while (i < ABC_LENGTH && !is_set)
    {
        uint8_t not_equals = symbols[i].key ^ (symbol->key);
        if (!not_equals)
        {
            symbol->symbol_code = symbols[i].symbol_code;
            is_set = 1;
        }
        i++;
    }

    if (!is_set)
    {
        symbol->symbol_code = 0x2C;
    }
}
