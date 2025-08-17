#ifndef REN_LIO_H
#define REN_LIO_H

#include <stddef.h>

/*

	FIX WIN

*/

typedef unsigned char _byte;

typedef enum {
	SUCCESS,
	ERR_IOBAD_ALLOC,
	ERR_IO_READ,
	ERR_IO_OPEN,
	ERR_IO_EOF,
	ERR_IO_SBUF
} IO_CODE;

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
	DWORD buf_size;
	DWORD rdb;
	DWORD err;
	_byte end;
	_byte* _cur_ptr;	// NU
	_byte* _buf;
} _io_file;

#else 
#include <sys/types.h>
typedef char CHAR;
typedef struct {
	int fd;
	ssize_t buf_size;
	ssize_t nrsize;
	int err;
	_byte* _cur_ptr;	// NU
	_byte* _buf;
} _io_file;

#endif

/*
	* @brief Open file only for read. 
	* @param path Path to file
	* @param buf_size Buffering size
	* @param code Return error code
	* @return File read handle
*/

_io_file*	_io_lopen	(	const CHAR*		path, 	const size_t	buf_size,	IO_CODE*	code);

/*
	* @brief Reads from a file buf_size bytes
	* @param file File read handle
	* @param code Return error code
	* @return void
*/

void	_io_lread	(	_io_file*	file,	IO_CODE*	code	);

/*
	* @brief Reads bytes from a file, saving unused ones
	* @param file File read handle
	* @param code Return error code
	* @return void
*/

void	_io_lread_c	(	_io_file* file,	IO_CODE*	code	);

/*
	* @brief Returns a pointer to a buffer that contains exactly the desired size.
	* @param file File read handle
	* @param size Required size
	* @param code Return error code
	* @param sel_bytes Number of bytes provided
	* @return buf pointer
*/

_byte*	_io_lselect	(	_io_file*	file,	unsigned int size,	IO_CODE* code, ssize_t* sel_bytes	);

/*
	* @brief Close file read handle & free allocated memory. Set file pointer NULL
	* @param file File read handle
	* @return void
*/

void _io_lclose	(	_io_file**	file	);


#endif
