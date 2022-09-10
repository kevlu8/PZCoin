#include "ecoh.hpp"

// Pad the message inplace
uint64_t _pad(char **message, uint64_t len) {
	// Number of 0 bits to add
	int paddinglen = 256 - (len * 8 + 1) % 256;
	uint64_t outlen = len + (paddinglen + 1) / 8;
	char *paddedmessage = (char *)realloc(*message, outlen);
	memset(paddedmessage + len, 0, outlen - len);
	paddedmessage[len] = '\x80';
	return outlen;
}

std::string hash(std::string input) {
	// Move the message from the input to a seperate buffer
	char *message = (char *)malloc(input.length() + 1);
	memcpy(message, input.c_str(), input.length() + 1);
	// Pad the message
	uint64_t len = _pad(&message, input.length());
}
