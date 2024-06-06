## Jojo Chat 2/2

### Challenge description

The kidnappers of Jojo just published a new version of their chat. I heard that they are using some cryptography mechanism in order to ensure its security.
Get an admin access!

**Author: algorab**
### Solution

We can see that now, authentication is done by a signed token:
```python
def connect():
    token = input("Token: ")
    if not verify(token):
        return False, None, None
    name = b64decode(token).split(b";")[0].decode()
    names = os.listdir("./log")
    while not(hashlib.sha256(name.encode()).hexdigest() in names):
        token = input("This user does not exists! Token: ")
        if not verify(token):
            return False, None, None
        name = b64decode(token).split(b";")[0].decode()
    role = b64decode(token).split(b"|")[0].split(b";")[-1].decode()
    return True, name, role
```

And the generation of tokens is done once when creating account!
```python
def sign(username, is_admin=False):
    if is_admin:
        sig = hashlib.sha256(SECRET + username.encode() + b";admin").digest()
        return b64encode(username.encode() + b";admin|" + sig)
    else:
        sig = hashlib.sha256(SECRET + username.encode() + b";user").digest()
        return b64encode(username.encode() + b";user|" + sig)
```

The general form of the token will be `<username>;<role>|<signature>`.

We can then see that regular user will have a token `<username>;user`, whereas admins will have `<username>;admin`.

Moreover, we can see that the role is defined by taking the last part of the token, which means that something like `<username>;<garbage>;admin` will return `admin` as a role.

Then, maybe the signature scheme has a flaw somewhere ?? 

In fact, it does. It is vulnerabile to [length extension attack](https://en.wikipedia.org/wiki/Length_extension_attack). Which means that we can create a valid signature by only knowing the length of the key, which is 40 characters here.

By rewriting [this implementation of sha256 in python](https://github.com/keanemind/Python-SHA-256), we can then implement this attack. The only change is allowing initialization at a custom state of the hashing process.

```python
def generate_padding(messageLen, length=0):
    
    pad = bytearray()
    pad.append(0x80)
    print(messageLen)
    length += messageLen * 8
    while ((messageLen + len(pad)) * 8 + 64) % 512 != 0:
        pad.append(0x00)

    pad += length.to_bytes(8, 'big')

    assert ((messageLen + len(pad)) * 8) % 512 == 0, "Padding did not complete properly!"

    return pad


def generate_hash(message, 
                h0 = 0x6a09e667,
                h1 = 0xbb67ae85,
                h2 = 0x3c6ef372,
                h3 = 0xa54ff53a,
                h4 = 0x510e527f,
                h5 = 0x9b05688c,
                h6 = 0x1f83d9ab,
                h7 = 0x5be0cd19,
                length = 0):

    if isinstance(message, str):
        message = bytearray(message, 'ascii')
    elif isinstance(message, bytes):
        message = bytearray(message)
    elif not isinstance(message, bytearray):
        raise TypeError

    pad = generate_padding(len(message), length)

    message += pad

    # Parsing
    blocks = [] # contains 512-bit chunks of message
    for i in range(0, len(message), 64): # 64 bytes is 512 bits
        blocks.append(message[i:i+64])

    # Setting Initial Hash Value
    h0 = h0
    h1 = h1
    h2 = h2
    h3 = h3
    h4 = h4
    h5 = h5
    h6 = h6
    h7 = h7
    
    #[...]

    return ((h0).to_bytes(4, 'big') + (h1).to_bytes(4, 'big') +
            (h2).to_bytes(4, 'big') + (h3).to_bytes(4, 'big') +
            (h4).to_bytes(4, 'big') + (h5).to_bytes(4, 'big') +
            (h6).to_bytes(4, 'big') + (h7).to_bytes(4, 'big'))

    #[...]
```

And then we can write our attack:

```python
from sha256 import generate_hash, generate_padding
from base64 import b64decode, b64encode

SECRET_LEN = 40

def token_to_hash(token):
    data = b64decode(token)
    hash = data.split(b"|")[1]
    print(hash.hex())
    return hash.hex()

def generate_malicious_token(token):
    data = b64decode(token)
    origin = data.split(b"|")[0]
    pad = generate_padding(len(origin)+SECRET_LEN)
    print(len(origin)+SECRET_LEN)
    hash = attack(
        token_to_hash(token),
        len(origin)+SECRET_LEN,
        ";admin"
    )
    print(hash.hex())
    malicious_data = origin+pad+b";admin|"+hash
    return b64encode(malicious_data)

def attack(hash, messageLen, maliciousData):
    blocks = [int(hash[i*8:(i+1)*8], 16) for i in range(8)]
    shaLen = (messageLen + len(generate_padding(messageLen)))*8
    hash = generate_hash(
        maliciousData,
        blocks[0],
        blocks[1],
        blocks[2],
        blocks[3],
        blocks[4],
        blocks[5],
        blocks[6],
        blocks[7],
        shaLen
    )
    return hash


print(generate_malicious_token("<token>"))
```

Then, we can hit the admin section, and get the flag.

### Flag

`N0PS{b3w4R3_0F_l3NgTh_XT3nS1on_4Tt4cK5}`