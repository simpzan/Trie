#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include <assert.h>
#include <queue>
#include <string>
#include <vector>
#include <iostream>

void assertWithString(bool assert, const std::string &str);

void reportLength(std::istream &is, uint32_t lastPosition, 
    const std::string &msg);

#endif
