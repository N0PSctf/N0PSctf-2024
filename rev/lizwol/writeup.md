# CTF Challenge: LZW Reverse Engineering

## Overview

This Capture The Flag (CTF) challenge involves reverse engineering a binary that uses the LZW (Lempel-Ziv-Welch) compression algorithm to encrypt a flag. Participants are provided with the compiled encoder binary (`lizwol`) and the encrypted flag (`flag.enc`). Additionally, the solution includes the source code for both the original challenge and the `solve` program, which decrypts the encrypted flag.

## Files Provided

- **`lizwol`**: Compiled binary that encrypts `flag.txt` into `flag.enc`.
- **`flag.enc`**: Encrypted flag file.
- **`solve`**: Compiled binary that decrypts `flag.enc` back to `flag_recover.txt`.
- **`solve.c`**: Source code for the `solve` decryption program.

## Usage Instructions

### Encrypting the Flag

To encrypt your `flag.txt` using the `lizwol` binary:

```bash
./lizwol flag.txt flag.enc
```


### Decrypting the Flag

To encrypt your `flag.txt` using the `lizwol` binary:

```bash
./solve flag.enc
```
