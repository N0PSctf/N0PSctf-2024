## Broken OTP

### Description

We just found this weird encryption service...
They asked us to review their algorithm, and provided a copy of their program.
They also gave us this information:
```
$ python3 --version
Python 3.12.3
```

**Author : algorab**

### Solution

#### Analysis

Through the challenge name, we first know that the crypto algorithm used here is [One Time Pad (OTP)](https://en.wikipedia.org/wiki/One-time_pad).   

Let's analyse the provided source code
```python
from time import time
import random

secret = 'XXXXXXXXXXXXXXXXXXXX'

PINK = 118
RED = 101
YELLOW = 97
GREEN = 108
BLACK = __builtins__
PURPLE = dir
e = getattr(BLACK, bytes([RED, PINK, YELLOW, GREEN]).decode())
g = e(''.__dir__()[4].strip('_')[:7])
b = g(BLACK, PURPLE(BLACK)[92])
i = g(BLACK, PURPLE(BLACK)[120])
t = ['74696d65', '72616e646f6d', '5f5f696d706f72745f5f', '726f756e64', '73656564']
d = lambda x: b.fromhex(x).decode()
fb = g(i, PURPLE(i)[-6])
_i = lambda x: e(d(t[2]))(x)
s = lambda: g(BLACK,d(t[3]))(g(_i(d(t[0])), d(t[0]))()) + fb(secret.encode())
r = g(_i(d(t[1])), d(t[4]))

def kg(l):
    return bytes([random.randint(0,255) for i in range(l)])

def c(p):
    k = kg(len(p))
    return bytes([k[i] ^ p[i] for i in range(len(p))]).hex()

if __name__ == '__main__':
    r(s())
    print("Welcome to our encryption service.")
    choice = input("Choose between:\n1. Encrypt your message.\n2. Get the encrypted secret.\nEnter your choice: ")
    match choice:
        case "1":
            message = input("Please enter the message you wish to encrypt: ")
            print(f"Your encrypted message is: {c(message.encode())}")
        case "2":
            print(f"The secret is: {c(secret.encode())}")
        case _:
            print("Invalid option!")
```

What we need to understand is especially the first part with colors' names.

To do this, we debug with some print statements. For instance, adding a `print(r)` shows that `r` is a random seed that is used in the algorithm.  By following the same strategy, we know that :

* `d(t[3])` corresponds to `round`
* `_i(d(t[0]))` corresponds to the builtin module time
* `d(t[0])` corresponds to `time`
* `g` refers to the builtin function `getattr`
* `fb` represents the from bytes builtin module
* `d(t[2])` depicts `import`

Knowing this, let's rewrite the first part of the code.

```python
PINK = 118
RED = 101
YELLOW = 97
GREEN = 108
BLACK = __builtins__
PURPLE = dir
e = getattr(__builtins__, bytes([101, 118, 97, 108]).decode())
# e corresponds to <built-in function eval> 
g = e(''.__dir__()[4].strip('_')[:7])
# g represents <built-in function getattr>
b = g(__builtins__, dir(__builtins__)[92])
i = g(__builtins__, dir(__builtins__)[120])
t = ['74696d65', '72616e646f6d', '5f5f696d706f72745f5f', '726f756e64', '73656564']
d = lambda x: b.fromhex(x).decode()
fb = g(i, dir(i)[-6])
_i = lambda x: eval("__import__")(x)
s = lambda: g(__builtins__,round)(g(<built-in module time>,time)()) + <built-in module from bytes>(secret.encode())
r = g(_i(d(t[1])), d(t[4]))
```

Let's make it even clearer
```python
PINK = 118
RED = 101
YELLOW = 97
GREEN = 108
BLACK = __builtins__
PURPLE = dir
e = <built-in function eval> 
g = <built-in function getattr>
b = <class 'bytearray'>
i = <built-in function input>
t = ['74696d65', '72616e646f6d', '5f5f696d706f72745f5f', '726f756e64', '73656564']
d = lambda x: b.fromhex(x).decode()
fb = g(i, dir(i)[-6])
_i = lambda x: eval("__import__")(x)
s = lambda: g(__builtins__,round)(g(<built-in module time>,time)()) + <built-in module from bytes>(secret.encode())
r = g(import(random), seed)
```
And clearer (we replace every variable with its function).
```python
e = eval                         # The 'eval' built-in function
g = getattr                      # The 'getattr' built-in function
b = bytearray                    # The 'bytearray' class
i = input                        # The 'input' built-in function

fb = __builtins__.frombytes()

_i = lambda x: eval("__import__")(x)

# Here we get the current time, round it to an integer, and concatenate with a secret string
s = lambda: get_attribute(__builtins__, 'round')(
    getattr(__import__('time'), 'time')()
) + frombytes(secret.encode())

# Seeding the random module
r = getattr(__import__('random'), 'seed')

```
So basically, what the first part of the code does is.
```python
import time
import random

current_time = round(time.time())+ frombytes(secret.encode())

r = random.seed
```
Hence, we know that the for the same `time.time()`, the seed is the same so the key is the same.
To summarize, the seed is computed using the current time with a precision of a second.

#### The attack

As we previously mentioned, for the same `time.time()`, the seed is the same so the key is the same. Therefore, if we obtain an encrypted message and the encrypted secret at the same time and we xor them both, we basically obtain our plaintext message xored with the secret. So once we xor the latter with the message again, we can flag.   

We can perform the following to obtain the encrypted message in hex and the encrypted secret **at the same time**, as well as the XOR operations.

```python
from pwn import *

host = '0.cloud.chals.io'
port = 33731

conn = remote(host, port)

conn.sendline(b'1')
response1 = conn.recv()
print(response1.decode())
conn.sendline(b'aaaaaaaaaaaaaaaaaaa')

response1 = conn.recv()
print(response1.decode())
conn.close()

s = response1.split(b'Your encrypted message is: ')[1].strip()
s=bytes.fromhex(s.decode())

conn = remote(host, port)
conn.sendline(b'2')

response2 = conn.recv()
print(response2.decode())

conn.close()

m = response2.split(b'The secret is: ')[1].strip()
m=bytes.fromhex(m.decode())
########### XOR Operations ###########

def c(k,p):
    return bytes([k[i] ^ p[i] for i in range(len(p))]) 

msg=b'aaaaaaaaaaaaaaaaaaa'
c1=bytes([s[i] ^ m[i] for i in range(len(m))])
print(bytes([c1[i] ^ msg[i] for i in range(len(c1))]))

```
Output example :
```
[+] Opening connection to 0.cloud.chals.io on port 33731: Done
Welcome to our encryption service.
Choose between:
1. Encrypt your message.
2. Get the encrypted secret.
Enter your choice: Please enter the message you wish to encrypt: 
Your encrypted message is: bddd5d8acfb2736ad3418be78b3ab92d8ae7de

[*] Closed connection to 0.cloud.chals.io port 33731
[+] Opening connection to 0.cloud.chals.io on port 33731: Done
Welcome to our encryption service.
Choose between:
1. Encrypt your message.
2. Get the encrypted secret.
Enter your choice: The secret is: 928c6cb8d5e3665bed4bd9dfb529eb39deb5c2

[*] Closed connection to 0.cloud.chals.io port 33731
b'aaaaaaaaaaaaaaaaaaa' b'\xbd\xdd]\x8a\xcf\xb2sj\xd3A\x8b\xe7\x8b:\xb9-\x8a\xe7\xde'
b'N0PS{0tP_k3Y_r3u53}'
```

### Flag

`N0PS{0tP_k3Y_r3u53}`
