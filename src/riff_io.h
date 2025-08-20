#ifndef REN_IO_H
#define REN_IO_H


	#include <stddef.h>
	#include "share/rentypes.h"


	#ifdef _WIN32
		#include <Windows.h>
		typedef struct {
			HANDLE* h_file; 
			DWORD buf_size;
			DWORD nrsize;
			byte* _cur_ptr;
			byte* _buf;
		} io_file;
	#else 
		#include <sys/types.h>
		typedef struct {
			int fd;
			size_t buf_size;
			size_t nrsize;
			byte* _cur_ptr;
			byte* _buf;
		} io_file;
	#endif	

	/*
		* @brief Open file only for read. 
		* @param path Path to file
		* @param buf_size Buffering size
		* @param code Return error code
		* @return File read handle
	*/
	
	io_file*	io_open	(	const CHAR*		path, 	size_t	buf_size,	renerr_t*	error	);
	
	/*
		* @brief Reads from a file buf_size bytes && returns a pointer to a buffer that contains exactly the desired size
		* @param file File read handle
		* @param size Required size
		* @param code Return error code
		* @return void
	*/
	
	byte*	io_read	(	io_file*	file,	unsigned int	size,	renerr_t*	code	);
	
	/*
		* @brief Close file read handle & free allocated memory. Set file pointer NULL
		* @param file File read handle
		* @return void
	*/
	
	void io_lclose	(	io_file**	file	);

#endif
