#include <stdlib.h>
#include <string.h>
#include "share/rentypes.h"
#include "riff_io.h"

/*

	FIX ERROR HANDLERS. TEST

*/


#ifdef _WIN32
	#include <windows.h>
#else 
	#include <fcntl.h>
	#include <unistd.h>
#endif

#ifdef _WIN32
	static void _io_read(io_file* file, void* buf, BUFSIZE_T size, renerr_t* code) {	
		if (!ReadFile( file->h_file, buf, size, &file->nrsize, NULL )) {
			*code = ERR_IO_READ;
			return;
		}
		if (file->nrsize == 0 && GetLastError() != ERROR_HANDLE_EOF) {
			*code = ERR_IO_READ;
			return;
		}
		if (file->nrsize == 0) {
			*code = ERR_IO_EOF;
			return;
		}
		*code = SUCCESS;
	}
#else
	static void _io_read(io_file* file, void* buf, BUFSIZE_T size, renerr_t* code) {	
		ssize_t rdb = read(file->fd, buf, size);
		if (rdb == -1) {
			*code = ERR_IO_READ;
		} else {
			if (rdb == 0) {
				*code = ERR_IO_EOF;
				file->nrsize = 0;
				return;
			}
			file->nrsize = (size_t)rdb;
			*code = SUCCESS;	
		}
	}
#endif


io_file* io_open(const CHAR* path, BUFSIZE_T buf_size, renerr_t* code) {		// CHECK WIN
	if (buf_size == 0) buf_size = 4096;
	
	io_file* file = (io_file*)malloc(sizeof(io_file));
	if (!file) {
		*code = ERR_BAD_ALLOC;
		return NULL;
	}
	
	#ifdef _WIN32	
	
		file->h_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NIRMAL, NULL);
		
		if (file->h_file == INVALID_HANDLE_VALUE) {
			*code = ERR_IO_OPEN;
			free(file);
			return NULL;
		}
		
	#else
	
		int fd = open(path, O_RDONLY);
		if (fd == -1) {
			*code = ERR_IO_OPEN;
			free(file);
			return NULL;
		}

		file->fd = fd;
		
	#endif

	byte* buf = (byte*)malloc(buf_size);
	if (!buf) {
		free(file);
		*code = ERR_BAD_ALLOC;
		return NULL;
	}

	file->buf_size = buf_size;
	file->_buf = buf;
	file->nrsize = 0;
	file->_cur_ptr = buf;
	
	*code = SUCCESS;
	return file;
}

byte* io_read(io_file* file, BUFSIZE_T size, renerr_t* code) {		// OPT if-const
	if (size > file->buf_size || size == 0) {
		*code = ERR_OUT_OF_RANGE;
		return NULL;
	}

	if (file->nrsize >= size) {
		byte* res_p = file->_cur_ptr;
		file->_cur_ptr += size;
		file->nrsize -= size;
		*code = SUCCESS;
		return res_p;
	}

	if (file->nrsize == 0) {
		_io_read(file, file->_buf, file->buf_size, code);
		if (*code != SUCCESS) {
			return NULL;
		}

		if (file->nrsize < size) {
			*code = ERR_IO_EOF;
			return NULL;
		}

		file->_cur_ptr = file->_buf + size;
		file->nrsize -= size;
		return file->_buf;
	}

	if (size > file->nrsize) {
		size_t nrsize = file->nrsize;
		memmove(file->_buf, file->_cur_ptr, file->nrsize);
		_io_read(file, file->_buf + file->nrsize, file->buf_size - nrsize, code);
		if (*code != SUCCESS) {
			return NULL;
		}
		if (file->nrsize + nrsize < size) {
			*code = ERR_IO_EOF;
			return NULL;
		}
		
		file->nrsize += nrsize - size;
		file->_cur_ptr = file->_buf + size;
		return file->_buf;
	}
}

byte* io_get_ck_data(io_file* file, BUFSIZE_T size, renerr_t* code) {
	byte* tmp = (byte*)malloc(size);
	if (!tmp) {
		*code = ERR_BAD_ALLOC;
		return NULL;
	}

	if (size > file->nrsize) {
		if (file->nrsize > 0) {
			memcpy(tmp, file->_cur_ptr, file->nrsize);	
		}
		
		_io_read(file, tmp + file->nrsize, size - file->nrsize, code);

		if (code != SUCCESS) {
			return NULL;
		}

		memcpy(file->_buf, tmp + size - file->buf_size, file->buf_size);
		file->_cur_ptr = file->_buf;
		file->nrsize = file->buf_size;
		
	} else {
		memcpy(tmp, file->_buf, size);
		file->nrsize -= size;
		file->_cur_ptr += size;
	}
	return tmp;
}

void io_seek_f(io_file* file, BUFSIZE_T size, renerr_t* code) {
	#ifdef _WIN32
		LARGE_INTEGER offset;
		offset.QuadPart = size;
		if (!SetFilePointerEx(file->h_file, offset, NULL, FILE_CURRENT)) {
			*code = ERR_IO_SEEK;
			return;
		}
	#else
		if (lseek(file->fd, size, SEEK_CUR) == -1) {
			*code = ERR_IO_SEEK;
			return;
		};
	#endif
	_io_read(file, file->_buf, file->buf_size, code);
}

void io_lclose(io_file** file) {
	#ifdef _WIN32
		CloseHandle((*file)->h_file);
	#else
		close((*file)->fd);
	#endif
	free((*file)->_buf);
	free(*file);
	*file = NULL;
}
