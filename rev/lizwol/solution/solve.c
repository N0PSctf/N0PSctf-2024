#include "lzw.h"

static int lzw_dec_readbits(lzw_dec_t *const ctx, unsigned nbits)
{
	while (ctx->bb.n < nbits)
	{
		if (ctx->lzwn == ctx->lzwm)
			return -1;

		ctx->bb.buf = (ctx->bb.buf << 8) | ctx->inbuff[ctx->lzwn++];
		ctx->bb.n += 8;
	}

	ctx->bb.n -= nbits;

	return (ctx->bb.buf >> ctx->bb.n) & ((1 << nbits)-1);
}

void lzw_dec_init(lzw_dec_t *ctx, void *stream)
{
	unsigned i;

	ctx->code     = CODE_NULL;
	ctx->max      = 255;
	ctx->codesize = 8;
	ctx->bb.n     = 0;
	ctx->stream   = stream;

	for (i = 0; i < 256; i++)
	{
		ctx->dict[i].prev = CODE_NULL;
		ctx->dict[i].ch   = i;
	}
}

static void lzw_dec_reset(lzw_dec_t *const ctx)
{
	ctx->code     = CODE_NULL;
	ctx->max      = 255;
	ctx->codesize = 8;
#if DEBUG
	printf("reset\n");
#endif
}

static unsigned lzw_dec_getstr(lzw_dec_t *const ctx, int code)
{
	unsigned i = sizeof(ctx->buff);

	while (code != CODE_NULL && i)
	{
		ctx->buff[--i] = ctx->dict[code].ch;
		code = ctx->dict[code].prev;
	}

	return sizeof(ctx->buff) - i;
}

static int lzw_dec_addstr(lzw_dec_t *const ctx, int code, unsigned char c)
{
	if (code == CODE_NULL)
		return c;
		
	if (++ctx->max == CODE_NULL)
		return CODE_NULL;

	ctx->dict[ctx->max].prev = code;
	ctx->dict[ctx->max].ch   = c;
#if DEBUG
	printf("add code %x = %x + %c\n", ctx->max, code, c);
#endif

	return ctx->max;
}

static unsigned char lzw_dec_writestr(lzw_dec_t *const ctx, int code)
{
	unsigned strlen = lzw_dec_getstr(ctx, code);

	lzw_writebuf(ctx->stream, ctx->buff+(sizeof(ctx->buff) - strlen), strlen);

	return ctx->buff[sizeof(ctx->buff) - strlen];
}

int lzw_decode(lzw_dec_t *ctx, char buf[], unsigned size)
{
	if (!size) return 0;

	ctx->inbuff = buf;	
	ctx->lzwn   = 0;	
	ctx->lzwm   = size;	

	for (;;)
	{
		int ncode;

		ncode = lzw_dec_readbits(ctx, ctx->codesize);
#if DEBUG
		printf("code %x (%d)\n", ncode, ctx->codesize);
#endif
		if (ncode < 0)
		{
#if DEBUG
			if (ctx->lzwn != ctx->lzwm)
				return LZW_ERR_INPUT_BUF;
#endif
			break;
		}
		else if (ncode <= ctx->max)
		{
			ctx->c = lzw_dec_writestr(ctx, ncode);

			if (lzw_dec_addstr(ctx, ctx->code, ctx->c) == CODE_NULL)
				return LZW_ERR_DICT_IS_FULL;
		}
		else 
		{
			if (ncode != ctx->max+1)
				return LZW_ERR_WRONG_CODE;

			if (lzw_dec_addstr(ctx, ctx->code, ctx->c) == CODE_NULL)
				return LZW_ERR_DICT_IS_FULL;

			ctx->c = lzw_dec_writestr(ctx, ncode);
		}

		ctx->code = ncode;

		if (ctx->max+1 == (1 << ctx->codesize))
			ctx->codesize++;

		if (ctx->max+1 == DICT_SIZE)
			lzw_dec_reset(ctx);
	}

	return ctx->lzwn;
}
