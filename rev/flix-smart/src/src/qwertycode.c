#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "qwertycode.h"

// warnings
#include <stdio.h>

#define QWERTY_CLEAR 256
#define QWERTY_END   257
#define QWERTY_START 258
#define QWERTY_MINBITS  9
#define QWERTY_MAXBITS 12 // max 12 because of table=32 bit
#define QWERTY_HASHSIZE 9001  // at least 1<<MAXBITS, should be prime 

// accessors to table[]-values
#define NEXTBYTE(a)   ((a)&0xff)
#define PREFIXCODE(a) ((a>>8)&0xfff)
#define CODE(a)       ((a>>20)&0xfff) // encode only
#define MAKETABLE(code,prefixcode,nextbyte) ( (code<<20)|(prefixcode<<8)|(nextbyte) ) // for decode: code=0
// hash func;
#define HASH(prefixcode,nextbyte) ( (((prefixcode<<8)|nextbyte)<<11)%QWERTY_HASHSIZE )

QWERTYSTATE *init_qwerty(int earlychange,READFUNC rf,WRITEFUNC wf,void *user_read,void *user_write,int tablesize,unsigned char *stack)
{
    QWERTYSTATE *ret;

    if (earlychange<0)
    {
        earlychange=1; // default
    }

    ret=malloc(sizeof(QWERTYSTATE));
    if (!ret)
    {
        return 0;
    }

    ret->read=rf;
    ret->write=wf;
    ret->user_read=user_read;
    ret->user_write=user_write;

    ret->earlychange=earlychange;

    ret->table=malloc(tablesize*sizeof(unsigned int));
    if (!ret->table)
    {
        free(ret);
        return NULL;
    }
    ret->stackend=ret->stackptr=stack+(1<<QWERTY_MAXBITS); // this is tricky!

    restart_qwerty(ret);
    return ret;
}

QWERTYSTATE *init_qwerty_read(int earlychange,READFUNC rf,void *user_read)
{
    unsigned char *stack;
    assert(rf);
    if (!rf)
    {
        return 0;
    }
    stack=malloc((1<<QWERTY_MAXBITS)*sizeof(unsigned char));
    if (!stack)
    {
        return NULL;
    }
    return init_qwerty(earlychange,rf,NULL,user_read,NULL,1<<QWERTY_MAXBITS,stack);
}

QWERTYSTATE *init_qwerty_write(int earlychange,WRITEFUNC wf,void *user_write)
{
    assert(wf);
    if (!wf)
    {
        return 0;
    }
    return init_qwerty(earlychange,NULL,wf,NULL,user_write,QWERTY_HASHSIZE,NULL);
}

void restart_qwerty(QWERTYSTATE *state)
{
    assert(state);
    if (state)
    {
        state->numcodes=QWERTY_START;
        state->codebits=QWERTY_MINBITS;
        state->prefix=-1; // no prefix / clear table
        state->stackptr=state->stackend;

        state->bitbuf=0;
        state->bitpos=0;
    }
}

void free_qwerty(QWERTYSTATE *state)
{
    if (state)
    {
        free(state->stackend-(1<<QWERTY_MAXBITS)); // look at init_qwerty !
        free(state->table);
        free(state);
    }
}

// helper

static int writecode(QWERTYSTATE *state,unsigned int code)
{
    unsigned char buf[4];
    int iA=0;

    state->bitbuf|=code<<(32-state->bitpos-state->codebits);
    state->bitpos+=state->codebits;
    while (state->bitpos>=8)
    {
        buf[iA++]=state->bitbuf>>24;
        state->bitbuf<<=8;
        state->bitpos-=8;
    }
    if (!iA)
    {
        return 0;
    }
    return (*state->write)(state->user_write,buf,iA);
}

static int writeflush(QWERTYSTATE *state)
{
    unsigned char c;

    assert(state->bitpos<8);
    if (!state->bitpos)
    {
        return 0;
    }
    c=state->bitbuf>>24;
    state->bitbuf=0;
    state->bitpos=0;
    return (*state->write)(state->user_write,&c,1);
}

// -> encode
// tries to append >nextbyte to >prefixcode.
// if a matching code is found: this is the new ("longer") prefixcode (>returned)
// otherwise a new code is created (state->numcodes resp. state->numcodes-1)
static inline int find_add_hash(QWERTYSTATE *state,int prefixcode,unsigned char nextbyte)
{
    unsigned int hash=HASH(prefixcode,nextbyte);

    while (1)
    {
        unsigned int ret=state->table[hash];
        if (!ret)   // empty entry
        {
            break;
        }
        if ( (PREFIXCODE(ret)==prefixcode)&&(NEXTBYTE(ret)==nextbyte) )   // found
        {
            return CODE(ret);
        }
        hash=(hash+1)%QWERTY_HASHSIZE;
    }
    // not found: add entry
    state->table[hash]=MAKETABLE(state->numcodes,prefixcode,nextbyte);
    state->numcodes++;
    return -1;
}

// TODO: check errors from writecode
int encode_qwerty(QWERTYSTATE *state,unsigned char *buf,int len)
{
    assert(state);
    assert(len>=0);
    if (!buf)   // finish up the stream
    {
        if (state->prefix>=0)   // the current prefixcode won't become any longer
        {
            writecode(state,state->prefix);
        }
        // TODO? empty stream:  QWERTY_CLEAR QWERTY_END
        writecode(state,QWERTY_END);
        writeflush(state);
        return 0;
    }
    while (len>0)
    {
        if (state->prefix==-1)   // begin / clear table
        {
            writecode(state,QWERTY_CLEAR);
            memset(state->table,0,QWERTY_HASHSIZE*sizeof(unsigned int));
            state->numcodes=QWERTY_START;
            state->codebits=QWERTY_MINBITS;
            state->prefix=*buf;
            len--;
            buf++;
        }
        // here we go: find prefixcode
        for (; len>0; len--,buf++)
        {
            int code=find_add_hash(state,state->prefix,*buf);
            if (code==-1)   // no longer prefix found; new code assigned
            {
                writecode(state,state->prefix);
                state->prefix=*buf; // set new prefix to current char

                if ( ((state->numcodes-1)==(1<<state->codebits)-state->earlychange-1)&&
                        (state->codebits==QWERTY_MAXBITS) )
                {
                    state->prefix=-1; // clear table (one early, so we don't "have to" increase >codebits)
                    break;
                }
                else if ((state->numcodes-1)==(1<<state->codebits)-state->earlychange)
                {
                    state->codebits++;
                }
            }
            else
            {
                state->prefix=code;
            }
        }
    }
    return 0;
}