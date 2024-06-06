## Just Read

### Description

Find a way to break this.

**Author: algorab**

### Solution

#### Reversing the code

We basically have an ELF executable that we need to reverse. We can do this by performing the command : 
```
$ file main              
main: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=d60942d0003ba2a102c7ef78d939c4a9232cf94a, for GNU/Linux 3.2.0, not stripped
```
We use Ghidra to reverse the code and we obtain the following function 

```
#include "out.h"

int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  iVar1 = __gmon_start__();
  return iVar1;
}



void FUN_00101020(void)

{
  (*(code *)(undefined *)0x0)();
  return;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

int puts(char *__s)

{
  int iVar1;
  
  iVar1 = puts(__s);
  return iVar1;
}



// WARNING: Unknown calling convention -- yet parameter storage is locked

size_t strlen(char *__s)

{
  size_t sVar1;
  
  sVar1 = strlen(__s);
  return sVar1;
}



void __cxa_finalize(void)

{
  __cxa_finalize();
  return;
}



void processEntry _start(undefined8 param_1,undefined8 param_2)

{
  undefined auStack_8 [8];
  
  __libc_start_main(main,param_2,&stack0x00000008,0,0,param_1,auStack_8);
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



// WARNING: Removing unreachable block (ram,0x001010a3)
// WARNING: Removing unreachable block (ram,0x001010af)

void deregister_tm_clones(void)

{
  return;
}



// WARNING: Removing unreachable block (ram,0x001010e4)
// WARNING: Removing unreachable block (ram,0x001010f0)

void register_tm_clones(void)

{
  return;
}



void __do_global_dtors_aux(void)

{
  if (completed_0 != '\0') {
    return;
  }
  __cxa_finalize(__dso_handle);
  deregister_tm_clones();
  completed_0 = 1;
  return;
}



void frame_dummy(void)

{
  register_tm_clones();
  return;
}



undefined8 main(undefined8 param_1,long param_2)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  char cVar7;
  char cVar8;
  char cVar9;
  char cVar10;
  char cVar11;
  char cVar12;
  char cVar13;
  char cVar14;
  char cVar15;
  char cVar16;
  char cVar17;
  char cVar18;
  char cVar19;
  char cVar20;
  char cVar21;
  char cVar22;
  char cVar23;
  char *__s;
  size_t sVar24;
  
  __s = *(char **)(param_2 + 8);
  cVar1 = *__s;
  cVar2 = __s[1];
  cVar3 = __s[2];
  cVar4 = __s[3];
  cVar5 = __s[4];
  cVar6 = __s[5];
  cVar7 = __s[6];
  cVar8 = __s[7];
  cVar9 = __s[8];
  cVar10 = __s[9];
  cVar11 = __s[10];
  cVar12 = __s[0xb];
  cVar13 = __s[0xc];
  cVar14 = __s[0xd];
  cVar15 = __s[0xe];
  cVar16 = __s[0xf];
  cVar17 = __s[0x10];
  cVar18 = __s[0x11];
  cVar19 = __s[0x12];
  cVar20 = __s[0x13];
  cVar21 = __s[0x14];
  cVar22 = __s[0x15];
  cVar23 = __s[0x16];
  sVar24 = strlen(__s);
  if (sVar24 == 0x17 &&
      ((((((((((((((((((((((cVar2 == '0' && cVar1 == 'N') && cVar3 == 'P') && cVar4 == 'S') &&
                        cVar5 == '{') && cVar6 == 'c') && cVar7 == 'H') && cVar8 == '4') &&
                    cVar9 == 'r') && cVar10 == '_') && cVar11 == '1') && cVar12 == 's') &&
                cVar13 == '_') && cVar14 == '8') && cVar15 == 'b') && cVar16 == 'i') &&
            cVar17 == 't') && cVar18 == 's') && cVar19 == '_') && cVar20 == '1') && cVar21 == 'N')
       && cVar22 == 't') && cVar23 == '}')) {
    puts("Well done, you can validate with this flag!");
  }
  else {
    puts("Wrong flag!");
  }
  return 0;
}



void _fini(void)

{
  return;
}
```

Our focus is the `main` function.
So basically, we have a verification on the characters of the input parameter as highlighted in the if condition that controls the characters of the parameter `__s`.  

If we sum up the conditions we have in order to have the `"Well done, you can validate with this flag!"` message, we obtain :
```
  __s[1] ->'N'
  __s[2] ->'0'
  __s[3] ->'P'
  __s[4] ->'S'
  __s[5] ->'{'
  __s[6] ->'c'
  __s[7] ->'H'
  __s[8] ->'4'
  __s[9] ->'r'
  __s[10]->'_'
  __s[11]->'1'
  __s[12]->'s'
  __s[13]->'8'
  __s[14]->'b'
  __s[15]->'i'
  __s[16]->'t'
  __s[17]->'s'
  __s[18]->'_'
  __s[19]->'1'
  __s[20]->'N'
```

### Flag

`N0PS{cH4r_1s_8bits_1Nt}`
