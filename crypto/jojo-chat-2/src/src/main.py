import os
import hashlib
from datetime import datetime
import re
from base64 import b64encode, b64decode
from admin import admin

SECRET = b'J0j0chatVeryS3cUreKeytH4tN0b0dYW1llCr4cK'

def sign(username, is_admin=False):
    if is_admin:
        sig = hashlib.sha256(SECRET + username.encode() + b";admin").digest()
        return b64encode(username.encode() + b";admin|" + sig)
    else:
        sig = hashlib.sha256(SECRET + username.encode() + b";user").digest()
        return b64encode(username.encode() + b";user|" + sig)
    
def verify(token):
    data = b64decode(token)
    block, sig = data.split(b"|")[0], data.split(b"|")[1]
    if hashlib.sha256(SECRET + block).digest() != sig:
        return False
    return True

def sort_messages(messages):
    try:
        messages.sort(key=lambda x: datetime.strptime(x[1][:19], "%Y-%m-%d %H:%M:%S"))
    except:
        pass
    return messages

def create_account():
    name = input("Enter your username: ")
    names = os.listdir("./log")
    while hashlib.sha256(name.encode()).hexdigest() in names:
        name = input("This username is already used! Enter another one: ")
    if not re.match("^[A-Za-z0-9]+$", name):
        print("\nEmpty usernames are forbidden, and you can only use uppercases, lowercases and numbers, you hacker!")
        exit()
    log = open(f"./log/{hashlib.sha256(name.encode()).hexdigest()}", 'w')
    log.write(f"Username : {name}\n")
    token = sign(name)
    print("\nAccount was successfully created!\nHere is your connection token:\n"+token.decode())
    log.close()

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

def get_all_messages():
    names = os.listdir("./log")
    messages = []
    for name in names:
        with (open(f"./log/{name}", 'r')) as log:
            username = log.readline().split(" : ")[1][:-1]
            for line in log.readlines():
                messages.append((username, line))
    return sort_messages(messages)

def send_message(name):
    message = input("Enter your message: ")
    with (open(f"./log/{hashlib.sha256(name.encode()).hexdigest()}", 'a')) as log:
        log.write(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} {message}\n")
    print("\nYour message has been sent!")

connected = False

print("Hey, welcome to j0j0 Chat v2! Feel free to chat with everyone here :)\nUpdate note: We noticed some security issues in our previous chat, this should be fixed now!")

while True:
    if not connected:
        option = input("\nChoose an option:\n1) Create an account\n2) Login\n3) Leave\n")
        match option:
            case "1":
                create_account()
            case "2":
                connected, name, role = connect()
                if not(connected):
                    print("Incorrect token!")
            case "3":
                print("Bye! Come back whenever you want!")
                exit()
    else:
        option = input("\nChoose an option:\n1) See messages\n2) Send a message\n3) Logout\n")
        match option:
            case "1":
                print()
                messages = get_all_messages()
                for message in messages:
                    print(f"{message[0]} : {message[1][20:]}", end="")
            case "2":
                send_message(name)
            case "3":
                print("\nYou successfully logged out!")
                connected = False
                role = None
            case "admin":
                if role == "admin":
                    admin()