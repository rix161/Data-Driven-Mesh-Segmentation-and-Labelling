#ifndef __FILE_READER__
#define __FILE_READER__
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
class FileReader {

public:
	FileReader() {};
	std::string getFile(const char*);

};
#endif // !__FILE_READER__
