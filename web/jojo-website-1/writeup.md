## Jojo Website 1/2

### Description

Now,  the J3JuJ4 group has moved their chat system on a website.
Can you find a way to become admin of it?

**Authors: algorab, Sto**

### Solution

We are facing a web application where we can create account, login, see list of users, edit profile and logout.

After some exploration, we can notice that the request to edit account information contains an `id` field.

![[request.png]]

We can try to change that value to see if there is any [IDOR](https://portswigger.net/web-security/access-control/idor) vulnerability. By sending `id=1`, we would then try to edit admin account, and changing its password.

![[exploit.png]]

The request seems to be accepted. We can then login as user admin and get the flag in the admin section.
### Flag

`N0PS{1d0R_p4zZw0rD_r3Z3t}`