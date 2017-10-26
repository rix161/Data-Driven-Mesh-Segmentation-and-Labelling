#include "FileReader.h"

std::string FileReader::getFile(const char* fileName) {

	if (fileName == NULL) return NULL;

	std::ifstream fileStream;
	std::string outString;
	std::string nextLine;

	fileStream.open(fileName);
	if (!fileStream)  return "";

	while (!fileStream.eof()) {
		std::getline(fileStream, nextLine);
		nextLine.append("\n");
		outString.append(nextLine);
	}
	fileStream.close();
	return outString;

}