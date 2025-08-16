#include <stdlib.h>
#include "loaders_io.h"

/*

	CHeCK WiN

*/


#ifdef _WIN32

#include <windows.h>
#include <wchar.h>

#else 

#include <fcntl.h>
#include <unistd.h>

#endif

_io_file* _io_lopen(const CHAR* path, const size_t buf_size) {		// CHECK WIN
	#ifdef _WIN32

	_io_file* file = (_io_file*)malloc(sizeof(_io_file));
	if (!file) return NULL;

	_byte* buf = (_byte*)malloc(buf_size);
	if (!buf) {
		free(file);
		return NULL;
	}
	
	file->h_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NIRMAL, NULL);
	

	if (file->h_file == INVALID_HANDLE_VALUE) {
		file->_err = GetLastError();
		file->flags |= F_IO_ERR;
		free(buf);
		return file;
	}
	
	#else

	_io_file* file = (_io_file*)malloc(sizeof(_io_file));
	if (!file) return NULL;
	
	_byte* buf = (_byte*)malloc(buf_size);
	if (!buf) {
		free(file);
		return NULL;
	}
	
	int fd = open(path, O_RDONLY);
	if (fd == -1) return NULL;

	file->fd = fd;
	#endif

	file->buf_size = buf_size;
	file->_buf = buf;
	file->_base_ptr = buf;	// NU
	file->_cur_ptr = buf;	// NU
	file->_flags = 0;
	return file;
}

ssize_t _io_lread(_io_file* file) {
	#ifdef _WIN32

	DWORD rdb;
	
	if (!ReadFile( file->h_file, file->_buf, file->buf_size, &rdb, NULL )) {
		file->_flags |= F_IO_ERR;
	}
	
	if (rdb < file->buf_size) {
		file->_flags |= F_IO_END;
	}
	
	#else

	ssize_t rdb = read(file->fd, file->_buf, file->buf_size);
	if (rdb == -1) {
		file->_flags |= F_IO_ERR;
	} else if (rdb < file->buf_size) {
		file->_flags |= F_IO_END;
	}

	#endif

	return rdb;
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

