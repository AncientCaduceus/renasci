#ifndef REN_LOADER_H
#define REN_LOADER_H

#include <stdint.h>

/*

	ENUMS		! remove unnecessary

*/

typedef enum {
	SUCCESS,
	ERR_BAD_ALLOC,
} ERR_CODE;

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
	ICMT = 0x49434D54,
	ICRD = 0x49435244,
	IGNR = 0x49474E52,
	INAM = 0x494E414D,
	ISRC = 0x49535243,
	ITRK = 0x4954524B,
} INFO_CHUNK_ID;

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
	DS64 = 0x64733634,
	VIDS = 0x76696473,
	AUDS = 0x61756473,
	MIDS = 0x6D696473,
	TXTS = 0x74787473
} STREAM_TYPE;

/*

	STRUCTS. IO-RIFF

*/

#pragma pack(push, 1)

typedef struct {
	uint32_t ck_id;
	uint32_t ck_size;
	uint32_t format;
} io_riff_hdr;

typedef struct {
	uint32_t ck_id;
	uint32_t ck_size;
} io_riff_chunk_hdr;

#pragma pack(pop)

/*

	STRUCTS. RIFF CHUNKS.

*/

typedef struct {
	uint16_t compr_code;
	uint16_t channels;
	uint32_t samples_per_sec;
	uint32_t bytes_per_sec;
	uint16_t block_align;
	uint16_t valid_bits_per_sample;
} riff_fmt_data;

/*

	STRUCTS. WAVE

*/

struct wave_hdl {
	riff_fmt_data fmt;
	unsigned char* data;
};

#endif
