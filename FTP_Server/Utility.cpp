#include "Utility.h"

void verboseLog(const std::string_view& str)
{
	if (g_verbose)
		ctl::Log::log(str);
}

void handleList(Connection& con, char* fileName)
{
	std::string content;

	//Iterate though files
	verboseLog("Gathering file list.");
	for (auto& i : std::filesystem::recursive_directory_iterator(fileName))
		content.append(i.path().string() + '\n');
	verboseLog("Gathering complete.");

	//Write files to client
	verboseLog("Sending file list.");
	con.write(content);
	verboseLog("File list sent.");
}

void handleFile(Connection& con, const std::string& fileName)
{
	static auto writeFile = [&con](std::ifstream& file, const std::string& name)
	{
		auto content = ctl::getInputStreamContent(file);
		auto size = std::to_string(content.size());

		con.write(name + ';' + size + ';' + std::move(content));
		verboseLog(name + " with " + std::move(size) + " sent.");
	};

	std::ifstream file(fileName, std::ios::binary | std::ios::in);
	std::string content;

	//If input is a normal file
	if (file)
	{
		verboseLog("Single file copy procedure started.");
		writeFile(file, fileName);
	}

	//If input is a directory
	else if (std::filesystem::exists(fileName))
	{
		verboseLog("Multiple file copy procedure started.");

		for (auto& i : std::filesystem::recursive_directory_iterator(fileName))
		{
			std::ifstream fileIter(i, std::ios::binary | std::ios::in);

			if (fileIter)
				//Write to client
				writeFile(fileIter, i.path().string());
		}

		return;
	}

	//If it doesn't exist
	else
	{
		content = "ERROR"s;
		verboseLog("Requested file couldn't be read.");
	}

	//Write to client
	verboseLog("Sending " + fileName);
	con.write(content);
	verboseLog(fileName + " sent.");
}