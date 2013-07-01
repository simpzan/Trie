#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <stdint.h>

struct Entry {
	std::string key;
	uint32_t offset;

	Entry(std::string aKey, uint32_t aOffset) : key(aKey), offset(aOffset) {}
};

#endif
