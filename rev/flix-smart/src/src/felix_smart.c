/**
 * QWERTY Encoder and Decoder
 * (c) 2007 by Tobias Hoffmann
 */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "qwertycode.h"


int wrfunc(void *user,unsigned char *buf,int len)
{
    FILE *f=(FILE *)user;

    return (fwrite(buf,1,len,f)==len)?0:1;
}

int rdfunc(void *user,unsigned char *buf,int len)
{
    FILE *f=(FILE *)user;

    return (fread(buf,1,len,f)==len)?0:1;
}

int wrfunc_mem(void *user,unsigned char *buf,int len)
{
    char **tmp=(char **)user;
    memcpy(*tmp,buf,len);
    *tmp+=len;
    return 0;
}

int rdfunc_mem(void *user,unsigned char *buf,int len)
{
    char **tmp=(char **)user;
    memcpy(buf,*tmp,len);
    *tmp+=len;
    return 0;
}

int wrfunc_bits(void *user,unsigned char *buf,int len)
{
    FILE *f=(FILE *)user;
    int iA;
    unsigned char iB;

    for (iA=0; iA<len; iA++)
    {
        for (iB=0x80; iB>0; iB>>=1)
        {
            if (buf[iA]&iB)
            {
                putc('1',f);
            }
            else
            {
                putc('0',f);
            }
        }
    }
    printf("\n");
    for (iA=0; iA<len; iA++)
    {
        printf("x%02x",buf[iA]);
    }
    printf("\n");

    return 0;
}

int main(int argc,char **argv)
{
    QWERTYSTATE *qwerty;
    int ret=0,early=-1;
    char *files[2]= {NULL,NULL};
    unsigned char *buf=NULL;
    int iA,iB;
    FILE *f=NULL,*g=NULL; // avoid warning

    // parse commandline
    iB=0;
    for (iA=1; iA<argc; iA++)
    {
        if (iB<2)
        {
            files[iB++]=argv[iA];
        }
    }

#define BUFSIZE 4096
            buf=malloc(BUFSIZE);
            if (!buf)
            {
                fprintf(stderr,"Malloc failed: %s\n", strerror(errno));
                return 2;
            }
            if (files[0])
            {
                if ((f=fopen(files[0],"rb"))==NULL)
                {
                    fprintf(stderr,"Error opening \"%s\" for reading: %s\n",files[0], strerror(errno));
                    free(buf);
                    return 2;
                }
            }
            else
            {
                f=stdin;
#ifdef _WIN32
                _setmode(_fileno(f), _O_BINARY);
#endif
            }

        if (files[1])
        {
            if ((g=fopen(files[1],"wb"))==NULL)
            {
                fprintf(stderr,"Error opening \"%s\" for writing: %s\n",files[1], strerror(errno));
                free(buf);
                if ((files[0]) )
                {
                    fclose(f);
                }
                return 3;
            }
        }
        else
        {
            g=stdout;
#ifdef _WIN32
            _setmode(_fileno(g), _O_BINARY);
#endif
        }
//    qwerty=init_qwerty_write(1,wrfunc_mem,&tmp);
        qwerty=init_qwerty_write(early,wrfunc,g);
        if (!qwerty)
        {
            fprintf(stderr,"Alloc error: %s\n", strerror(errno));
            free(buf);
            if ( (files[0]) )
            {
                fclose(f);
            }
            if (files[1])
            {
                fclose(g);
            }
            return 2;
        }
        // encode
            int len;
            while ((len=fread(buf,1,BUFSIZE,f))>0)
            {
                ret=encode_qwerty(qwerty,buf,len);
                if (ret)
                {
                    break;
                }
            }
    
        free(buf);
        if (!ret)
        {
            ret=encode_qwerty(qwerty,NULL,0);
        }
        free_qwerty(qwerty);
        if ( files[0] )
        {
            fclose(f);
        }
        if (files[1])
        {
            fclose(g);
        }
        if (ret)
        {
            fprintf(stderr,"Error: %d\n",ret);
            return 2;
        }


    return 0;
}
