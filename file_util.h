#ifndef FILE_UTIL_H
#define FILE_UTIL_H
 
#include <openssl/sha.h>
#include "data_structs.h"


const short SHA_DIGEST_LENGTH = 20;


unsigned char* get_unique_id(unsigned char payload[]){ 

	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(payload, sizeof(payload), hash);
	return hash;
}

#endif
