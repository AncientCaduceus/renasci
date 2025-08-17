#include <stdlib.h>
#include <string.h>
#include "loaders_io.h"


/*

	FIX WiN. FIX ifdef. MEM LEAKS

*/


#ifdef _WIN32

#include <windows.h>
#include <wchar.h>

#else 

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#endif

#ifdef _WIN32

static void _io_read(HANDLE h_file, void* buf, DWORD buf_size, DWORD* rdb, IO_CODE* code, int* err) {		
		if (!ReadFile( h_file, buf, buf_size, rdb, NULL )) {
			*code = ERR_IO_READ;
		}
	
		if (rdb == 0 && GetLastError() != ERROR_HANDLE_EOF) {
			*code = ERR_IO_READ;
			*err = GetLastError();
		}
}

#else

static void _io_read(int fd, void* buf, ssize_t buf_size, ssize_t* rdb, IO_CODE* code, int* err) {	
		*rdb = read(fd, buf, buf_size);
		if (*rdb == -1) {
			*code = ERR_IO_READ;
			*err = errno;
		}
	
		#endif
}

_io_file* _io_lopen(const CHAR* path, const size_t buf_size, IO_CODE* code) {		// CHECK WIN
	_io_file* file = (_io_file*)malloc(sizeof(_io_file));
	if (!file) {
		*code = ERR_IOBAD_ALLOC;
		return NULL;
	}
	
	#ifdef _WIN32
	
	file->h_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NIRMAL, NULL);
	

	if (file->h_file == INVALID_HANDLE_VALUE) {
		*code = ERR_IO_OPEN;
		file->err = GetLastError();
		return file;
	}
	
	#else
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		*code = ERR_IO_OPEN;
		file->err = errno;
		return file;
	}

	file->fd = fd;
	#endif

	_byte* buf = (_byte*)malloc(buf_size);
	if (!buf) {
		free(file);
		*code = ERR_IO_OPEN;
		return NULL;
	}

	file->buf_size = buf_size;
	file->_buf = buf;
	file->nrsize = 0;
	file->_cur_ptr = buf;	// NU
	*code = SUCCESS;
	return file;
}

void _io_lread(_io_file* file, IO_CODE* code) {	
	#ifdef _WIN32
	_io_read(file->h_file, file->_buf, file->buf_size, &file->nrsize, code, &file->err);
	#else
	_io_read(file->fd, file->_buf, file->buf_size, &file->nrsize, code, &file->err);
	#endif
	
	file->_cur_ptr = file->_buf;
	*code = SUCCESS;
}

static _byte* _io_move_buf(_io_file* file) {
	memmove(file->_buf, file->_cur_ptr, file->nrsize);
	file->_cur_ptr = file->_buf;
	return file->_buf + file->nrsize;
}

void _io_lread_c(_io_file* file, IO_CODE* code) {		// SLOW FOR MBs
	if (file->nrsize == 0) {
		_io_lread(file, code);
		return;
	}

	_byte* start_ptr = _io_move_buf(file);
	#ifdef _WIN32
	DWORD nrsize = file->nrsize;
	_io_read(file->h_file, start_ptr, file->buf_size - file->nrsize, &file->nrsize, code, &file->err);
	file->nrsize += nrsize;
	#else
	ssize_t nrsize = file->nrsize;
	_io_read(file->fd, start_ptr, file->buf_size - file->nrsize, &file->nrsize, code, &file->err);
	file->nrsize += nrsize;
	#endif
	*code = SUCCESS;
}

_byte* _io_lselect(_io_file* file, unsigned int size, IO_CODE* code, ssize_t* sel_bytes) {
	if (file->nrsize < size) {
	 	_io_lread_c(file, code);
	 	if (*code != SUCCESS) {
	 		return NULL;
	 	}
	 	if (file->nrsize == 0) {
	 		*code = ERR_IO_EOF;
	 		return NULL;
		}
	}
	if (file->nrsize < size) {
 		*sel_bytes = file->nrsize;
 	} else {
 		*sel_bytes = size;	
 	}
	_byte* res = file->_cur_ptr;
	file->nrsize -= size;
	file->_cur_ptr += size;
	return res;
}

void _io_lclose(_io_file** file) {
	#ifdef _WIN32
	CloseHandle((*file)->h_file);
	#else
	close((*file)->fd);
	#endif
	free((*file)->_buf);
	free(*file);
	*file = NULL;
}

