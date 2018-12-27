/* jesin-hex-conversion.h by Kevin Dodd */
#ifndef jesin_hex_conversion_h_INCLUDED
#define jesin_hex_conversion_h_INCLUDED
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>

int jesin_bin_to_hex(FILE *ibin, FILE *ohex);
/*int jesin_portable_bin_to_hex(FILE *ibin, FILE *ohex);*/

int jesin_hex_to_bin(FILE *ihex, FILE *obin);
/*int jesin_portable_hex_to_bin(FILE *ihex, FILE *obin);*/

int jesin_hex_conversion_main(int argc, char const *const *argv);

#endif
