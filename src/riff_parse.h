#ifndef REN_RIFF_PARSE_H
#define REN_RIFF_PARSE_H

	#include "share/rentypes.h"
	#include "riff_io.h"
	#include <stddef.h>
	#include <stdint.h>
	

	/*

		ENUMS.
		
	*/

	typedef enum {
		WAVE = 0x57415645,
		AVI  = 0x41564920,
		RMID = 0x524D4944,
		CDDA = 0x43444441,
		ANIM = 0x414E494D,
	} RIFF_TYPE;

	typedef enum {
		INFO = 0x494E464F,
		HRDL = 0x6864726C,
		MOVI = 0x6D6F7669,
		ODML = 0x6F646D6C,
		ADTL = 0x6164696C,
		TRAK = 0x7472616B,
	} LIST_CHUNK_TYPE;

	typedef enum {
		UNKN = 0x0,
		RIFF = 0x52494646,
		RIFX = 0x52494658,
		LIST = 0x4C495354,
		JUNK = 0x4A554E4B,
		FMT  = 0x666D7420,
		DATA = 0x64617461,
		FACT = 0x66616374,
		CUE  = 0x63756520,
		SMPL = 0x736D706C,
		INST = 0x696E7374,
		AVIH = 0x61766968,
		STRH = 0x73747268,
		STRF = 0x73747266,
		IDX1 = 0x69647831
	} CHUNK_ID;

	typedef enum {
		IART = 0x49415254,
		ICRD = 0x49435244,
		IGNR = 0x49474E52,
		INAM = 0x494E414D,
		ISRC = 0x49535243,
		ITRK = 0x4954524B,
	} INFO_CHUNK_ID;


	/*

		STRUCTS

	*/

	typedef struct {
		io_file* file;
		int depth;
		char pad;
		char data_only;
		RIFF_TYPE rtype;
		uint32_t riff_size;
		uint32_t riff_rdb;
		uint32_t rdb;
		uint32_t ck_size;
		uint32_t ck_id;
		renerr_t err;
	} riff_ctx;
	
	/*

		FUNCS.SHARE

	*/
	
	/*
		* @brief Allocates memory for the context
		* @param file File read handle
		* @return Riff context pointer
	*/
	
	riff_ctx*	riff_init_ctx	(void);


	/*
		* @brief Allows to skip a chunk
		* @param ctx Riff context
	*/

	void	riff_skip_ck	(	riff_ctx*	ctx	);
	

	/*
		* @brief Reads chunk_id & chunk_size
		* @param ctx Riff context
	*/

	void	riff_ck_head	(	riff_ctx*	ctx	);


	/*
		* @brief Reads chunk data
		* @param ctx Riff context
		* @return Pointer to allocated array
	*/

	byte*	riff_return_data	(	riff_ctx*	ctx	);
	

	/*
		* @brief
		* @param ctx Riff context
	*/

	void*	riff_parse	(	CHAR* path, riff_ctx*	ctx	);

	/*

		FUNCS. WAVE

	*/

	

#endif
