#ifndef REN_LIO_H
#define REN_LIO_H

#include <stddef.h>

#define F_IO_ERR (1 << 0)
#define F_IO_END (1 << 1)

typedef unsigned char _byte;

#ifdef _WIN32

#include <Windows.h>

#ifdef UNICODE
typedef wchar_t CHAR;
#define CreateFile CreateFileW
#else
typedef char CHAR;
#define CreateFile CreateFileA
#endif

typedef struct {
	HANDLE* h_file; 
	ssize_t buf_size;
	DWORD _err;
	_byte* _base_ptr;	// NU
	_byte* _cur_ptr;	// NU
	_byte* _buf;
	_byte _flags;
} _io_file;

#else 
#include <sys/types.h>
typedef char CHAR;
typedef struct {
	int fd;
	ssize_t buf_size;
	_byte* _base_ptr;	// NU
	_byte* _cur_ptr;	// NU
	_byte* _buf;
	_byte _flags;
} _io_file;

#endif

/*
	* @brief Open file only for read. 
	* @param path Path to file
	* @param buf_size Buffering size
	* @return File read handle
*/

_io_file*	_io_lopen	(	const CHAR*		path, 	const size_t	buf_size	);

/*
	* @brief Read from file buf_size bytes
	* @param file File read handle
	* @return Number of bytes read
*/

ssize_t	_io_lread	(	_io_file*	file	);

/*
	* @brief Close file read handle & free allocated memory. Set file pointer NULL
	* @param file File read handle
	* @return void
*/

void _io_lclose	(	_io_file**	file	);


#endif
