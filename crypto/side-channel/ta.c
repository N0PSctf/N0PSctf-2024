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
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#include "aes.h"

uint8_t **pt, ct[16];
double *t;
uint8_t timing_model[256];

void read_datafile(char *name, int n);

int main(int argc, char **argv) {
    int i, j, b, n, g, bg;
    double max, avgt, stdt, tm, avgtm, stdtm, avgttm, pcc[256];
    uint8_t key[16];

    if(argc != 3) {
        fprintf(stderr, "usage: ta <datafile> <nexp>\n");
        exit(1);
    }
    n = atoi(argv[2]);
    if(n < 2) {
        fprintf(stderr, "%d: invalid number of acquisitions (<nexp> shall be greater than 1)\n", n);
        exit(1);
    }

    // read data file
    read_datafile(argv[1], n);

    // compute average and standard deviation of times
    for(i = 0, avgt = 0.0, stdt = 0.0; i < n; i++) {
        avgt += t[i];
        stdt += t[i] * t[i];
    }
    avgt /= n;
    stdt = sqrt((stdt / n) - avgt * avgt);

    // initialize timing model table
    timing_model[0] = 0;
    for(i = 0; i < 8; i++) {
        for(j = (1 << i); j < (1 << (i + 1)); j++) {
            timing_model[j] = i + 1;
        }
    }

    // for all key bytes
    for(b = 0; b < 16; b++) {
        // for all candidate values of key byte
        for(g = 0; g < 256; g++) {
            // for all data file entries
            for(i = 0, avgtm = 0.0, stdtm = 0.0, avgttm = 0.0; i < n; i++) {
                tm = timing_model[subByte(pt[i][b] ^ g)]; // compute timing model
                avgtm += tm; // average of timing model
                stdtm += tm * tm; // standard deviation of timing model
                avgttm += t[i] * tm; // average of product time * timing model
            }
            avgtm /= n;
            avgttm /= n;
            stdtm = sqrt((stdtm / n) - avgtm * avgtm);
            pcc[g] = (avgttm - avgt * avgtm) / (stdt * stdtm); // pearson
        }
        bg = 0;
        max = 0.0;
        for(g = 1, bg = 0, max = pcc[0]; g < 256; g++) {
            if(pcc[g] > max) {
                bg = g;
                max = pcc[g];
            }
        }
        key[b] = bg;
    }

    // free allocated buffers
    for(i = 0; i < n; i++) {
        free(pt[i]);
    }
    free(pt);
    free(t);

    // print best candidate secret key
    printWord(key, 16, "", "", "\n");
    return 0;
}

void read_datafile(char *name, int n) {
    FILE *fp;
    int i, j;

    fp = fopen(name, "r");
    if(fp == NULL) {
        fprintf(stderr, "%s: file not found\n", name);
        exit(1);
    }
    pt = calloc(n, sizeof(uint8_t *));
    if(pt == NULL) {
        fprintf(stderr, "allocation error\n");
        exit(1);
    }
    t = calloc(n, sizeof(double));
    if(t == NULL) {
        fprintf(stderr, "allocation error\n");
        exit(1);
    }
    for(i = 0; i < n; i++) {
        pt[i] = calloc(16, sizeof(uint8_t));
        if(pt[i] == NULL) {
            fprintf(stderr, "allocation error\n");
            exit(1);
        }
        for(j = 0; j < 16; j++) {
            if(fscanf(fp, "%02" SCNx8, pt[i] + j) != 1) {
                fprintf(stderr, "cannot read plaintext\n");
                exit(1);
            }
        }
        for(j = 0; j < 16; j++) {
            if(fscanf(fp, "%02" SCNx8, ct + j) != 1) {
                fprintf(stderr, "cannot read ciphertext\n");
                exit(1);
            }
        }
        if(fscanf(fp, "%lf", t + i) != 1) {
            fprintf(stderr, "cannot read time\n");
            exit(1);
        }
    }
    fclose(fp);
}

// vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab textwidth=0:
