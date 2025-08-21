#include "loaders.h"
#include "loaders_io.h"

#include <stdlib.h>
<<<<<<< HEAD
#include <string.h>

riff_ctx* riff_init_ctx(void) {
	riff_ctx* ctx = (riff_ctx*)malloc(sizeof(riff_ctx));
	if (!ctx) return NULL;
	memset(ctx, 0, sizeof(riff_ctx));
	return ctx;
}

void open_riff(CHAR* path, riff_ctx* ctx, size_t buf_size) {
	ERR_CODE code;
	_io_file* file = _io_lopen(path, buf_size, &code);

	if (code != SUCCESS) {
		ctx->err = code;
		if (file) ctx->err_code = file->err;
		free(file);
		return;
	}

	ctx->file = file;
	ctx->err = SUCCESS;
}
=======

>>>>>>> origin/dev


