#pragma once

#include <iostream>

class FlatBufferFileIo {

public:
	FlatBufferFileIo() {};

	virtual ~FlatBufferFileIo() {};

public:
	bool saveFile(const char* name, const char* buf, size_t len, bool binary);

	bool loadFile(const char* name, bool binary, std::string* buf);

};
