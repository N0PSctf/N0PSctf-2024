## Interception 2/2

### Description

The message you found by decoding the radio signal seems encrypted.
Can you try to break it? We really need those information.

**Author: algorab**

### Solution

We can first analyze the data we got from previous challenge `Interception 1/2`.

The two first block can be decoded and are 
```  n=DA3A06F069A20840534AC12288B683A6AAB0D29A0E8617D2CCBB6D4A197DB5DA325439F8888B7C06C9943A3EF10F91D4D0BF6AEF7B70FD8BEAE8E2777BC193463FEBDE6DCD6CFD6D5ABFD433F925DB846E43A87615B101378F6E263E3D52E5F452422CB4ECA174452F3FEF8C04AF6088B4E67D9AA992557F0EAA68C22AEEA811
e=AE16DC5D005FCF4303281071ED04101DC3A60CA3221F4F8F34D0FD61C78EC360A712074894E9DC76B20E4AEB20D3A6401C3ABD52B421E5A72BB793D1B371EF67FC8136B0E1B801914DB99D922D1B18FF02579BB60901D805229883253BE85B7053CE813D14E761787BBB4858BF3BB31CD2DC31B4C4CB5410C0714AAE66095989
```

The other blocks cannot be read. Therefore, we can guess that they are RSA-encrypted, and that the public key is given in the first two blocks.
Given that, we can notice that `e` is very high compared to usual value. This can lead to some attacks that we can explore.
Especially, we can think about [Wiener's attack](https://en.wikipedia.org/wiki/Wiener's_attack) on RSA.

Using [an implementation of that attack](https://github.com/orisano/owiener), we can then load the blocks and try to recover data.

```python
import owiener

data = open("blocks.txt", "r").read().split("\n")

n_block = bytes.fromhex(data[0][2:]).decode()
e_block = bytes.fromhex(data[1][2:]).decode()

n = int(n_block.split("=")[1], 16)
e = int(e_block.split("=")[1], 16)

d = owiener.attack(e, n)

decrypted = b""

for block in bitstr[2:]:
	hex_str = ''.join([chr(int(block[i:i+8],2)) for i in range(0,len(block),8)])[2:]
	decrypted += decrypt(int(hex_str, 16), n, d)

open("decrypted.txt", "wb").write(decrypted)
```

We can then recover the original message, and the flag.

### Flag

`N0PS{r5A_w13n3R_4T74cK}`

