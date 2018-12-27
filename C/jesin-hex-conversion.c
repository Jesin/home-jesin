/* jesin-hex-conversion.c by Kevin Dodd */
#ifndef jesin_hex_conversion_c_INCLUDED
#define jesin_hex_conversion_c_INCLUDED
#include "jesin-hex-conversion.h"
#include "jesin-unreachable.h"
#include <errno.h>
#include <limits.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef JESIN_B2H_BYTES_PER_LINE
#define JESIN_B2H_BYTES_PER_LINE ((size_t)64)
#endif
#ifndef JESIN_BUFALIGN
#define JESIN_BUFALIGN ((size_t)32)
#endif
#ifndef JESIN_BUFSIZ
#define JESIN_BUFSIZ ((size_t)65536)
#endif

int jesin_bin_to_hex(FILE *ibin, FILE *ohex) {
	alignas(JESIN_BUFALIGN) uint8_t inputBuf[JESIN_B2H_BYTES_PER_LINE];
	alignas(JESIN_BUFALIGN) char outputBuf[2*JESIN_B2H_BYTES_PER_LINE + 1];
	while (1) {
		const size_t numRead = fread(inputBuf, sizeof(uint8_t), JESIN_B2H_BYTES_PER_LINE, ibin);
		JESIN_ASSUME(numRead <= JESIN_B2H_BYTES_PER_LINE);
		if (!numRead) {
			return ferror(ibin);
		}
		for (size_t i = 0; i < numRead; ++i) {
			uint8_t ch = inputBuf[i];
			uint8_t x = ch >> 4;
			uint8_t y = ch & 15;
			x += (x < 10 ? (uint8_t)'0' : (uint8_t)('a' - 10));
			y += (y < 10 ? (uint8_t)'0' : (uint8_t)('a' - 10));
			outputBuf[2*i + 0] = x;
			outputBuf[2*i + 1] = y;
		}
		outputBuf[2*numRead] = '\n';
		const size_t numWritten = fwrite(outputBuf, sizeof(char), 2*numRead + 1, ohex);
		JESIN_ASSUME(numWritten <= (2*JESIN_B2H_BYTES_PER_LINE + 1));
		if (numWritten < (2*numRead + 1)) {
			return ferror(ohex);
		}
	}
}

int jesin_hex_to_bin(FILE *ihex, FILE *obin) {
	alignas(4096) char inputBuf[JESIN_BUFSIZ];
	alignas(JESIN_BUFALIGN) uint8_t outputBuf[JESIN_BUFSIZ];
	size_t j = 0;
	while (1) {
		const size_t numRead = fread(inputBuf, sizeof(char), JESIN_BUFSIZ, ihex);
		JESIN_ASSUME(numRead <= JESIN_BUFSIZ);
		for (size_t i = 0; i < numRead; ++i) {
			char c = inputBuf[i];
			uint8_t x;
			if ('0' <= c && c <= '9') {
				x = c - '0';
			} else if ('A' <= c && c <= 'F') {
				x = c - ('A' - 10);
			} else if ('a' <= c && c <= 'f') {
				x = c - ('a' - 10);
			} else {
				continue;
			}
			if (j & 1) {
				outputBuf[j >> 1] |= x;
			} else {
				outputBuf[j >> 1] = (x << 4);
			}
			++j;
			if (j == 2*JESIN_BUFSIZ) {
				size_t numWritten = fwrite(outputBuf, sizeof(uint8_t), JESIN_BUFSIZ, obin);
				if (numWritten < JESIN_BUFSIZ) {
					return ferror(obin);
				}
				j = 0;
			}
		}
		if (numRead < JESIN_BUFSIZ) {
			size_t numWritten = fwrite(outputBuf, sizeof(uint8_t), (j >> 1), obin);
			return numWritten < (j >> 1) ? ferror(obin) : ferror(ihex);
		}
	}
}

static void jesin_hex_conversion_print_usage(const char *restrict progName, FILE *restrict out) {
	fprintf(out, "Usage: %s {b2h | h2b} [inputfile [outputfile]]\n", (progName ? progName : "jesin_hex_conversion"));
}

int jesin_hex_conversion_main(int argc, char const *const *argv) {
	if (errno) {
		int e = errno;
		perror("Pre-existing error condition");
		return e;
	}
	if (argc < 2 || argc > 4) {
		jesin_hex_conversion_print_usage(argv[0], stderr);
		errno = EINVAL;
		return EINVAL;
	}
	bool convertingHexToBin;
	if ((argv[1][0] == 'b' || argv[1][0] == 'B') && argv[1][1] == '2' && (argv[1][2] == 'h' || argv[1][2] == 'H') && !argv[1][3]) {
		convertingHexToBin = false;
	} else if ((argv[1][0] == 'h' || argv[1][0] == 'H') && argv[1][1] == '2' && (argv[1][2] == 'b' || argv[1][2] == 'B') && !argv[1][3]) {
		convertingHexToBin = true;
	} else {
		jesin_hex_conversion_print_usage(argv[0], stderr);
		errno = EINVAL;
		return EINVAL;
	}
	FILE *infile = stdin;
	FILE *outfile = stdout;
	if (argc > 2) {
		if (argv[2][0]) {
			infile = fopen(argv[2], (convertingHexToBin ? "r" : "rb"));
			if (!infile) {
				int e = errno;
				perror("Error opening input file");
				return e;
			}
		}
	}
	if (argc > 3) {
		if (argv[3][0]) {
			outfile = fopen(argv[3], (convertingHexToBin ? "wb" : "w"));
			if (!outfile) {
				int e = errno;
				perror("Error opening output file");
				return e;
			}
		}
	}
	int retcode = errno;
	if (convertingHexToBin) {
		if (setvbuf(infile, NULL, _IONBF, 0)) {
			perror("Failure in setvbuf(infile, NULL, _IONBF, 0)");
		}
		if (setvbuf(outfile, NULL, _IONBF, 0)) {
			perror("Failure in setvbuf(outfile, NULL, _IONBF, 0)");
		}
		retcode = jesin_hex_to_bin(infile, outfile);
	} else {
		retcode = jesin_bin_to_hex(infile, outfile);
	}
	if (retcode && errno) { perror("Error while converting"); }
	if (infile != stdin) { fclose(infile); }
	if (outfile == stdout || outfile == stderr) {
		fflush(outfile);
	} else {
		fclose(outfile);
	}
	if (!retcode) { retcode = errno; }
	return retcode;
}

#ifndef JESIN_HEX_CONVERSION_OMIT_MAIN
int main(int argc, char const *const *argv) {
	int r = jesin_hex_conversion_main(argc, argv);
	if (r && !(r & 255)) { r |= 128; }
	return r;
}
#endif

#endif
