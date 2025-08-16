#ifndef REN_LOADER_H
#define REN_LOADER_H

#include <stdint.h>

/*

	ENUMS

*/

// NoR

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
	uint16_t extra_format_butes;
} riff_fmt_data;




#endif
