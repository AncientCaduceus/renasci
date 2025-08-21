#ifndef REN_TYPES_H
#define REN_TYPES_H

	#include <stddef.h>

	/*

		SHARE TYPES
		
	*/

	typedef unsigned char byte;

	#ifdef _WIN32
		#ifdef UNICODE
			typedef wchar_t CHAR;
		#else
			typedef char CHAR;
		#endif
		typedef DWORD BUFSIZE_T
	#else
		typedef char CHAR;
		typedef size_t BUFSIZE_T;
	#endif

	/*

		ERRORS
		
	*/

	typedef enum {
		SUCCESS,
		
		ERR_BAD_ALLOC,
		ERR_OUT_OF_RANGE,
		
		ERR_IO_OPEN,
		ERR_IO_EOF,
		ERR_IO_READ,
		ERR_IO_SEEK,
		
		ERR_RIFF_FORMAT,
		ERR_RIFF_TRUNCATED,
		ERR_RIFF_INVALID_CK,
		ERR_RIFF_UNSUP
	} renerr_t;

#endif
