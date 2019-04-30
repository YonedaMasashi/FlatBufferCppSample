#include "FileIo/FlatBufferFileIo.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool FlatBufferFileIo::saveFile(const char* name, const char* buf, size_t len, bool binary) {
	ofstream ofs(name, binary ? ofstream::binary : ofstream::out);
	if (!ofs.is_open()) return false;
	ofs.write(buf, len);
	return !ofs.bad();
}

bool FlatBufferFileIo::loadFile(const char* name, bool binary, string * buf) {
	ifstream ifs(name, binary ? ifstream::binary : ifstream::in);
	if (!ifs.is_open()) {
		return false;
	}
	if (binary) {
		// The fastest way to read a file into a string.
		ifs.seekg(0, ios::end);
		auto size = ifs.tellg();
		(*buf).resize(static_cast<size_t>(size));
		ifs.seekg(0, ios::beg);
		ifs.read(&(*buf)[0], (*buf).size());
	}
	else {
		// This is slower, but works correctly on all platforms for text files.
		ostringstream oss;
		oss << ifs.rdbuf();
		*buf = oss.str();
	}
	return !ifs.bad();
}
