#include "Utility.h"

void verboseLog(const std::string_view& str)
{
	if (g_verbose)
		ctl::Log::log(str);
}

void handleList(Connection& con)
{
	//Write the arg
	con.write("GET_LIST;");
	verboseLog("Request sent.");

	//Read the answer
	const auto data = con.read();
	verboseLog("File list received.");

	//Print the answer
	std::cout << data;
}

void handleFile(Connection& con, char* fileName)
{
	//Write file request
	con.write("GET_FILE;"s + fileName);
	verboseLog("File request sent.");

	for (auto content = con.read(); content != "EOF"; content = con.read())
	{
		verboseLog("Content received.");

		//Extract file name
		const auto sep = content.find(';');
		auto contentName = content.substr(0, sep);
	
		//Create containing folders
		const auto backSlash = contentName.rfind('\\');
		std::filesystem::create_directories(contentName.substr(0, backSlash));
		verboseLog("Directories created.");

		//Write to file
		std::ofstream fileOut(contentName, std::ios::binary | std::ios::out);
		verboseLog("File created.");

		fileOut.write(content.c_str() + sep + 1, content.size() - contentName.size() - 1);
		verboseLog("Writing done.");
	}
}
