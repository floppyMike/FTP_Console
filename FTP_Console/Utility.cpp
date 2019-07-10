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
	std::cout << data << '\n';
}

void handleFile(Connection& con, char* fileName)
{
	//Write file request
	con.write("GET_FILE;"s + fileName);
	verboseLog("File request sent.");

	//Read pending till connection is broken
	for (auto content = con.read(); content != "EOF"; content = con.read())
	{
		verboseLog("Content received.");
		//Iterate files from content
		for (auto ptr = content.begin(); ptr != content.end();)
		{
			//Extract file name
			const auto sep1 = content.find(';');
			auto contentName = content.substr(0, sep1);

			//Create containing folders
			const auto backSlash = contentName.rfind('\\');
			std::filesystem::create_directories(contentName.substr(0, backSlash));
			verboseLog("Directories created.");

			//Extract file size
			const auto sep2 = content.find(';', sep1 + 1);
			size_t size = std::stoul(content.substr(sep1 + 1, sep2 - sep1));

			//Write to file
			std::ofstream fileOut(contentName, std::ios::binary | std::ios::out);
			verboseLog("File created.");

			fileOut.write(content.c_str() + sep2 + 1, size);
			verboseLog("Writing done.");

			//Continue onto next file
			ptr += size;
		}
	}
}
