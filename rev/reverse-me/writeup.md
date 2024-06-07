## Reverse Me

### Description

Don't complain if you can't see me, because I have to be reversed to make me run :)

### Solution

#### Mirroring the image
We are provided with a JPG image. When we open it, nothing can be displayed.  
We try inspecting its content, we see some inversed strings such as ```tnemmoc.ssb.atad.cimanyd.yarra_inif.yarra_tini.emarf_he.rdh_emarf_he.atador.inif.txet.ces.tlp.tog.tlp.tini.tlp.aler.nyd.aler.r_noisrev.ung.noisrev.ung.rtsnyd.mysnyd.hsah.ung.gat-IBA.eton.di-dliub.ung.eton.ytreporp.ung.eton.pretni.batrtshs.0.4.11 )40.22~1utnubu1-0.4.11 utnubU( :CCG```. Hence, our first intuition is to "reverse the file", or in other words, **rewrite it in reverse order byte by byte**.

To do this we use the following script which basically mirrors the image :
```python
with open("img.jpg", "rb") as f:
    image_bytes = f.read()
image_bytes_inversed = bytes(reversed(image_bytes))
with open("reversed_image.jpg", "wb") as f:
    f.write(image_bytes_inversed)
```
When we open the `reversed_image.jpg`, we still have an error that nothing can be displayed. Nevertheless, when we perform the command `file reversed_image.jpg`, we see that it is an **`ELF 64-bit LSB pie executable`**.  
Therefore, we rename the `reversed_image.jpg` into `rev.elf` and then pass it through Ghidra.

#### Reversing the code
We try to understand the decompiled code which takes four parameters as an input. We find out that the function that controls printing the flag in the output is the following :
```c
bool FUN_00101460(int param_1,int param_2,int param_3,int param_4)
{
  bool bVar1;
  
  bVar1 = false;
  if (param_1 * -10 + param_2 * 4 + param_3 + param_4 * 3 != 0x1c) {
    return false;
  }
  if ((param_2 * 9 + param_1 * -8 + param_3 * 6 + param_4 * -2 == 0x48) &&
     (param_2 * -3 + param_1 * -2 + param_3 * -8 + param_4 == 0x1d)) {
    bVar1 = param_2 * 7 + param_1 * 5 + param_3 + param_4 * -6 == 0x58;
  }
  return bVar1;
}
```
To obtain the flag, this function has to return `True`. To do this, let's analyze it. If we take a closer look at the code, we notice that the function encompasses an `if`  condition that controls the solution of  a system of equations. Simple maths! Let's rewrite the equations in a clearer way:   
<div align="center">
-10 param<sub>1</sub> + 4 param<sub>2</sub> +  param<sub>3</sub> + 3 param<sub>4</sub> = 28 <br> 
-8 param<sub>1</sub> + 9 param<sub>2</sub> + 6 param<sub>3</sub> - 2 param<sub>4</sub> = 72 <br> 
-2 param<sub>1</sub> - 3 param<sub>2</sub> - 8 param<sub>3</sub> +  param<sub>4</sub> = 29  <br>
5 param<sub>1</sub> + 7 param<sub>2</sub> +  param<sub>3</sub> - 6 param<sub>4</sub> = 88  
</div>
We can use the following code to solve the system :

```python
from sympy import symbols, Eq, solve

x1, x2, x3, x4 = symbols('x1 x2 x3 x4')

eq1 = Eq(-10*x1 + 4*x2 + x3 + 3*x4, 28)
eq2 = Eq(-8*x1 + 9*x2 + 6*x3 - 2*x4, 72)
eq3 = Eq(-2*x1 - 3*x2 - 8*x3 + x4, 29)
eq4 = Eq(5*x1 + 7*x2 + x3 - 6*x4, 88)

solution = solve((eq1, eq2, eq3, eq4), (x1, x2, x3, x4))
```

Hence, the solution is :
param<sub>1</sub> = -3, param<sub>2</sub> = 8, param<sub>3</sub> = -7, param<sub>4</sub> = -9

We run the code with these parameters and we obtain the flag !
```
./rev.elf -3 8 -7 -9 
```

_We received some tickets and comments from participants who used Ghidra for the first time to solve this challenge which represented a great learning opportunity for them.  
Huge thanks to Simone AONZO (@packmad) for designing this challenge !_

### Flag

`N0PS{r1CKUNr0111N6}`
