/*
 * Copyright (C) Telecom Paris
 * 
 * This file must be used under the terms of the CeCILL. This source
 * file is licensed as described in the file COPYING, which you should
 * have received as part of this distribution. The terms are also
 * available at:
 * https://cecill.info/licences/Licence_CeCILL_V2.1-en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "aes.h"

// tabulated S-box
const uint8_t sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// tabulated inverse S-box
const uint8_t invSbox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// mixColumns matrix
const state_t mc = {
	{ 0x02, 0x03, 0x01, 0x01 },
	{ 0x01, 0x02, 0x03, 0x01 },
	{ 0x01, 0x01, 0x02, 0x03 },
	{ 0x03, 0x01, 0x01, 0x02 }
};

// invMixColumns matrix
const state_t invMc = {
	{ 0x0e, 0x0b, 0x0d, 0x09 },
	{ 0x09, 0x0e, 0x0b, 0x0d },
	{ 0x0d, 0x09, 0x0e, 0x0b },
	{ 0x0b, 0x0d, 0x09, 0x0e }
};

// round constants
const uint8_t rcon[10] = {
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// multiplication by X in GF256 / X^8+X^4+X^3+X+1
uint8_t gf256_xtime(uint8_t a) {
	uint16_t r;

	r = a << 1;
	if(r & 0x100) {
		r ^= 0x11b;
	}
	return r & 0xff;
}

// multiplication in GF256 / X^8+X^4+X^3+X+1
uint8_t gf256_time(const uint8_t a, const uint8_t b) {
	uint8_t r, x, y;

	for(r = 0, x = a, y = b; y; x = gf256_xtime(x), y >>= 1) {
		if(y & 1) {
			r ^= x;
		}
	}
	return r;
}

// 4x4 matrix product in GF256 / X^8+X^4+X^3+X+1, 
void gf256_matrix_time(state_t s, const state_t a, const state_t b) {
	int r, c, i;

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      s[r][c] = 0;
      for(i = 0; i < 4; i++) {
        s[r][c] ^= gf256_time(a[r][i], b[i][c]);
      }
    }
  }
}

// warning: rk[0] is a 4 bytes column
void addRoundKey(state_t s, const word_t rk[]) {
	int r, c;

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
			s[r][c] ^= rk[c][r];
		}
	}
}

uint8_t subByte(uint8_t b) {
	return sbox[b];
}

void subBytes(state_t s) {
	int r, c;

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
			s[r][c] = sbox[s[r][c]];
		}
	}
}

uint8_t invSubByte(uint8_t b) {
	return invSbox[b];
}

void invSubBytes(state_t s) {
	int r, c;

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
			s[r][c] = invSbox[s[r][c]];
		}
	}
}

void shiftRows(state_t s) {
	int r, c;
	word_t temp;

	for(r = 1; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			temp[c] = s[r][(c + r) % 4];
		}
		for(c = 0; c < 4; c++) {
			s[r][c] = temp[c];
		}
	}
}

void invShiftRows(state_t s) {
	int r, c;
	word_t temp;

	for(r = 1; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			temp[c] = s[r][(c - r + 4) % 4];
		}
		for(c = 0; c < 4; c++) {
			s[r][c] = temp[c];
		}
	}
}

void mixColumns(state_t s) {
	int r, c;
	state_t temp;

  gf256_matrix_time(temp, mc, s);
  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
			s[r][c] = temp[r][c];
		}
	}
}

void invMixColumns(state_t s) {
	int r, c;
	state_t temp;

  gf256_matrix_time(temp, invMc, s);
  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
			s[r][c] = temp[r][c];
		}
	}
}

void rotWord(word_t a) {
	int r;
	uint8_t temp;

	temp = a[0];
	for(r = 0; r < 3; r++) {
		a[r] = a[r + 1];
	}
	a[3] = temp;
}

void subWord(word_t a) {
	int r;

	for(r = 0; r < 4; r++) {
		a[r] = sbox[a[r]];
	}
}

void fprintWord(FILE *f, const uint8_t w[], const unsigned int n, const char * const sep, const char * const pre, const char * const post) {
	int r;

	fprintf(f, "%s", pre);
	for(r = 0; r < n; r++) {
		fprintf(f, "%02" PRIX8 "%s", w[r], (r == n - 1) ? "" : sep);
	}
	fprintf(f, "%s", post);
}

void printWord(const uint8_t w[], const unsigned int n, const char * const sep, const char * const pre, const char * const post) {
	int r;

	printf("%s", pre);
	for(r = 0; r < n; r++) {
		printf("%02" PRIX8 "%s", w[r], (r == n - 1) ? "" : sep);
	}
	printf("%s", post);
}

void printState(const state_t s) {
	int r, c;

	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			printf("%02" PRIX8, s[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

void keyExpansion(round_keys_t rk, const uint8_t key[16], const unsigned int len) {
	int r, i, nk, nr;
	word_t temp;

	assert((len == 128) || (len == 192) || (len == 256));
	nk = len / 32;
	nr = nk + 6;
	for(i = 0; i < nk; i++) {
		for(r = 0; r < 4; r++) {
			rk[i][r] = key[4 * i + r];
		}
	}
	for(; i < 4 * (nr + 1); i++) {
		for(r = 0; r < 4; r++) {
			temp[r] = rk[i - 1][r];
		}
		if((i % nk) == 0) {
			rotWord(temp);
			subWord(temp);
			temp[0] ^= rcon[i / nk - 1];
		} else if((nk > 6) && ((i % nk) == 4)) {
			subWord(temp);
		}
		for(r = 0; r < 4; r++) {
			rk[i][r] = rk[i - nk][r] ^ temp[r];
		}
	}
}

void cipher(uint8_t out[16], const uint8_t in[16], const unsigned int nr, const round_keys_t rk) {
	int r, c, n;
	state_t s;

  for(c = 0; c < 4; c++) {
    for(r = 0; r < 4; r++) {
			s[r][c] = in[r + 4 * c];
		}
	}
	addRoundKey(s, rk);
	for(n = 1; n <= nr; n++) {
		subBytes(s);
		shiftRows(s);
		if(n != nr) {
			mixColumns(s);
		}
		addRoundKey(s, rk + 4 * n);
	}
  for(c = 0; c < 4; c++) {
    for(r = 0; r < 4; r++) {
			out[r + 4 * c] = s[r][c];
		}
	}
}

void invCipher(uint8_t out[16], const uint8_t in[16], const unsigned int nr, const round_keys_t rk) {
	int r, c, n;
	state_t s;

  for(c = 0; c < 4; c++) {
    for(r = 0; r < 4; r++) {
			s[r][c] = in[r + 4 * c];
		}
	}
	addRoundKey(s, rk + 4 * nr);
	for(n = nr - 1; n >= 0; n--) {
		invShiftRows(s);
		invSubBytes(s);
		addRoundKey(s, rk + 4 * n);
		if(n != 0) {
			invMixColumns(s);
		}
	}
  for(c = 0; c < 4; c++) {
    for(r = 0; r < 4; r++) {
			out[r + 4 * c] = s[r][c];
		}
	}
}

// vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab textwidth=0:
