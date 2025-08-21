#include <stdlib.h>
#include <string.h>
#include "riff_io.h"
#include "riff_parse.h"
#include "share/rentypes.h"

/*

	ADD endian swap func

*/

#define DEFBUF_SIZE 4096

riff_ctx* riff_init_ctx(void) {
	void* ctx = malloc(sizeof(riff_ctx));
	if (!ctx) return NULL;
	memset(ctx, 0, sizeof(riff_ctx));
	return (riff_ctx*)ctx;
}

void riff_ck_head(riff_ctx*	ctx) {
	byte* hdr_p = io_read(ctx->file, 64, &ctx->err);
	if (ctx->err != SUCCESS) {
		return;
	}
	ctx->ck_id = *(uint32_t*)hdr_p;
	ctx->ck_size = *(uint32_t*)(hdr_p + 4); 
}

void riff_get_hdr(riff_ctx*	ctx) {
	byte* hdr_p = io_read(ctx->file, 96, &ctx->err);
	if (ctx->err != SUCCESS) {
		return;
	}
	ctx->ck_id = *(uint32_t*)hdr_p;
	if (ctx->ck_id != RIFF) {
		ctx->err = ERR_RIFF_FORMAT;
		return;
	}
	ctx->ck_size = *(uint32_t*)(hdr_p + 4);
	ctx->rtype = *(uint32_t*)(hdr_p + 8); 
}

void* riff_parse(CHAR* path, riff_ctx*	ctx) {
	if (!ctx) return NULL;

	// open file
	io_file* file = io_open(path, DEFBUF_SIZE, &ctx->err);
	if (ctx->err != SUCCESS) {
		return NULL;
	}
	ctx->file = file;

	// get riff header
	riff_get_hdr(ctx);
	if (ctx->err != SUCCESS) {
		return NULL;
	}

	ctx->depth++;

	
}

