## Side Channel

### Description

Your goal is to extract the key used for the encryption of the captured data.

All blocks have been encrypted in Electronic Codebook (ECB) mode.
All data blocks are in hexadecimal, byte 0 first, byte 15 last. First column: 128 bits input block, second column: 128 bits encrypted block, third column: encryption time.
The key length is 128 bits.

### Solution

#### Challenge Analysis 

We are provided with three files along with the challenge `aes.c`, `aes.h` and `ta.dat`. Moreover, as mentioned in the description of the challenge, `ta.dat` encompasses the plain text, ciphertext and the encryption time. As this is a side channel attack (according to the challenge name) and we have the encryption time for each input, we will likely perform a **timing attack**.  
The C code clearly shows that the `mixColumns` transform calls `gf256_matrix_time`, a matrix product in GF256, that itself calls `gf256_time`, the product in GF256:

```
uint8_t gf256_time(const uint8_t a, const uint8_t b) {
	uint8_t r, x, y;

	for(r = 0, x = a, y = b; y; x = gf256_xtime(x), y >>= 1) {
		if(y & 1) {
			r ^= x;
		}
	}
	return r;
}
```

Analyzing the code quickly shows that the time taken by `gf256_time` depends on the operands.
The attack exploits this data-dependent timing.

#### Timing Attack

To perform our timing attack, we follow these steps:

1. We isolate a set of operations that involves a part of the secret and a part of the plaintext, and which timing depends on both parts.
   The part of the secret shall be small enough to enumerate all candidate values; a byte (2^8 = 256 candidate values) is fine but the whole 128 bis secret (2^128, about 3.4 x 10^38 values) is not.
   At the beginning of the AES encryption, each byte `k` of the 128 bits secret key is first x-ored with the corresponding byte `p` of the 128 bits plaintext.
   Then, during the first round of encryption, the result `k^p` passes through the AES S-box (`subByte`), is moved by the `shiftRows` transform (but not otherwise modified), and enters `mixColumns` as one element of the left 4x4 matrix.
   So, `subByte(k^p)` is multiplied 4 times as left operand during the `mixColumns` transform.
   The processing of `subByte(k^p)` is a nice candidate for our timing attack: data-dependent timing, easy to compute all candidate values; let's give it a try.

1. We create a **Timing Model** of this processing: for each possible byte value that enters `mixColumns` we compute an estimate of its contribution to the computation time.
   The nice thing is that a relative estimate is enough.
   Looking at the code of the `gf256_time` function we see that the time it takes could be roughly modeled by the number of iterations of the loop, that is, the position of the most significant bit equal to 1 in operand `b`:

   `b` range   time model (`TM(b)`)
   ----------  ---------------------
   0           0
   1           1
   2-3         2
   4-7         3
   8-15        4
   16-31       5
   32-63       6
   64-127      7
   128-255     8

   As source code and executed binary can be significantly different we could cross-check this with an analysis of the assembly code but as we don't even know what the target processor was, let's optimistically assume that this data-dependent time survived the compilation.
   So, with our timing model, for each key byte (16), for each candidate value `k` of the key byte (256 values), and for each row of the `ta.dat` file, we can extract the value `p` of the corresponding byte of the plaintext block, and can compute `TM(subByte(k^p))`, our (relative) estimate of the time taken by `mixColumns` to process `k^p` during the first round.

1. The next step consists in comparing the 256 candidate values `k` and retain the most promising one.
   The usual way to do this is to compute a correlation metric between the timing model `TM(subByte(k^p))` and the real observed processing time `T`, over all rows of the `ta.dat` file.
   For the reasoning we can decompose `T` in 2 parts: the time `t` taken by the processing of `subByte(k^p)` in `mixColumns` during the first round, and the time taken by everything else, `S = T-t`.
   Of course, we do not have access to `t` and `S`, all we have is their sum `T` but still, this decomposition helps understanding.
   If the value of `k` is not the correct one there should be no correlation at all because our timing model has nothing to do with the real processing time; we could as well use a random oracle.
   But if the value of `k` is correct there should be a small correlation because our timing model is correlated with the `t` part of the real observed processing times `T`.
   As the expected correlation is linear the Pearson Correlation Coefficient (`PCC`) is the statistical tool of choice ("linear" here means that when our model says 8, `t` should be about 8 times what it is when our model says 1).

1. The `PCC` of 2 random variables `X` and `Y` is: `(E(X*Y) - E(X)*E(Y)) / (S(X) * S(Y)`, where `E(X)` is the mean value of the sample of `X` and `S(X)` is its standard deviation.
   In our situation we don't know `E(T)`, `E(TM(subByte(k^p)))`, `S(T)` and `S(TM(subByte(k^p)))` but we can estimate them from our data file with simple computations.
   For each byte of the secret key the candidate value `k` with the highest `PCC(T, TM(subByte(k^p)))` over all rows of the `ta.dat` file is the one we retain.

The complete attack script can be found [here](./ta.c).  
Output example : 

```
source/ta ./ta.dat 20000                                     
740279C13A16787F9E9BF14C2F61D669
```

#### Last check

After recovering the key, we use the command provided in the challenge description to make sure that it is correct. 

```
$ printf "$(sed -n 's/ .*//;s/\(..\)/\\x\1/gp;q' ta.dat)" | openssl enc -aes-128-ecb -nosalt -nopad -K 740279C13A16787F9E9BF14C2F61D669 | od -v -A none -tx1
 8e 37 49 35 4d 6c 1f 87 04 e7 2c 5d 0e 05 d4 d4
```

And we check that its output corresponds to the first ciphertext.

```
$ head -n 1 ta.dat 
18C882698119B9D37B65E49A41010FB1 8E3749354D6C1F8704E72C5D0E05D4D4 12117.800000
```

Huge Thanks to Renaud Pacalet for designing this challenge and providing the solution !

### Flag

`N0PS{740279C13A16787F9E9BF14C2F61D669}`
