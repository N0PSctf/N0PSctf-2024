#include "lzw.h"

static void lzw_enc_writebits(lzw_enc_t *const ctx, unsigned bits, unsigned nbits)
{
	// shift old bits to the left, add new to the right
	ctx->bb.buf = (ctx->bb.buf << nbits) | (bits & ((1 << nbits)-1));

	nbits += ctx->bb.n;

	// flush whole bytes
	while (nbits >= 8)
	{
		nbits -= 8;
		ctx->buff[ctx->lzwn++] = ctx->bb.buf >> nbits;

		if (ctx->lzwn == sizeof(ctx->buff)) {
			ctx->lzwn = 0;
			lzw_writebuf(ctx->stream, ctx->buff, sizeof(ctx->buff));
		}
	}

	ctx->bb.n = nbits;
}

__inline static int lzw_hash(const int code, const unsigned char c)
{
	return (code ^ ((int)c << 6)) & (HASH_SIZE-1);
}

void lzw_enc_init(lzw_enc_t *ctx, void *stream)
{
	unsigned i;

	ctx->code     = CODE_NULL; // non-existent code
	ctx->max      = 255;
	ctx->codesize = 8;
	ctx->stream   = stream;
	ctx->bb.n     = 0; // bit-buffer init
	ctx->lzwn     = 0; // output code-buffer init

	// clear hash table
	for (i = 0; i < HASH_SIZE; i++)
		ctx->hash[i] = CODE_NULL;

	for (i = 0; i < 256; i++)
	{
		int hash = lzw_hash(CODE_NULL, i);

		ctx->dict[i].prev  = CODE_NULL;
		ctx->dict[i].next  = ctx->hash[hash];
		ctx->dict[i].ch    = i;
		ctx->hash[hash]    = i;
	}
}

static void lzw_enc_reset(lzw_enc_t *const ctx)
{
	unsigned i;

#if DEBUG
	printf("reset\n");
#endif

	ctx->max      = 255;
	ctx->codesize = 8;

	for (i = 0; i < HASH_SIZE; i++)
		ctx->hash[i] = CODE_NULL;

	for (i = 0; i < 256; i++)
	{
		int hash = lzw_hash(CODE_NULL, i);

		ctx->dict[i].next  = ctx->hash[hash];
		ctx->hash[hash]    = i;
	}
}

static int lzw_enc_findstr(lzw_enc_t *const ctx, int code, unsigned char c)
{
	int nc;

	// hash search
	for (nc = ctx->hash[lzw_hash(code,c)]; nc != CODE_NULL; nc = ctx->dict[nc].next)
	{
		if (ctx->dict[nc].prev == code && ctx->dict[nc].ch == c) {
			break;
		}
	}

	return nc;
}

static int lzw_enc_addstr(lzw_enc_t *const ctx, int code, unsigned char c)
{
	int hash;

	if (++ctx->max == CODE_NULL)
		return CODE_NULL;

	hash = lzw_hash(code, c);

	// add new code
	ctx->dict[ctx->max].prev  = code;
	ctx->dict[ctx->max].next  = ctx->hash[hash];
	ctx->dict[ctx->max].ch    = c;
	// add the new code into hash table
	ctx->hash[hash] = ctx->max;
#if DEBUG
	printf("add code %x = %x + %c\n", ctx->max, code, c);
#endif

	return ctx->max;
}

int lzw_encode(lzw_enc_t *ctx, char buf[], unsigned size)
{
	unsigned i;

	if (!size) return 0;

	for (i = 0; i < size; i++)
	{
		unsigned char c = buf[i];
		int           nc = lzw_enc_findstr(ctx, ctx->code, c);

		if (nc == CODE_NULL)
		{
			// the string was not found - write <prefix>
			lzw_enc_writebits(ctx, ctx->code, ctx->codesize);
#if DEBUG
			printf("code %x (%d)\n", ctx->code, ctx->codesize);
#endif
			// increase the code size (number of bits) if needed
			if (ctx->max+1 == (1 << ctx->codesize))
				ctx->codesize++;

			// add <prefix>+<current symbol> to the dictionary
			if (lzw_enc_addstr(ctx, ctx->code, c) == CODE_NULL)
			{
				// dictionary is full - reset encoder
				lzw_enc_reset(ctx);
			}

			ctx->code = c;
		}
		else
		{
			ctx->code = nc;
		}
	}

	return size;
}
void lzw_enc_end(lzw_enc_t *ctx)
{
#if DEBUG
	printf("code %x (%d)\n", ctx->code, ctx->codesize);
#endif
	// write last code
	lzw_enc_writebits(ctx, ctx->code, ctx->codesize);
	// flush bits in the bit-buffer
	if (ctx->bb.n)
		lzw_enc_writebits(ctx, 0, 8 - ctx->bb.n);
	lzw_writebuf(ctx->stream, ctx->buff, ctx->lzwn);
}
