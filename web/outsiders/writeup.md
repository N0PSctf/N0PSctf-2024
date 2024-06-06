## Outsiders

### Description

*Wish you were here..*  

**Author: algorab**

### Solution

This web page says that we come from the big outside, and that we cannot be trusted. It means that we have to prove that we come from inside.
We can try to impersonate localhost by using `X-Forwarded-For` header, as [this header can be used to determine the origin of the request](This should help: https://stackoverflow.com/questions/3003145/how-to-get-the-client-ip-address-in-php).

```
curl http://localhost/ -H "X-Forwarded-For: 127.0.0.1"
```

The server then believes the request comes from localhost, and outputs the flag.

### Flag

`N0PS{XF0rw4Rd3D}`