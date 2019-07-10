// FTP_Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Utility.h"
#include "Connection.h"

std::queue<std::string_view> ctl::Log::m_buf;
bool g_verbose = false;

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cerr << "Usage: <host> <port> <operation> [file/flags]\n"
			<< "<host> host machine with running server program\n"
			<< "<port> port server program is listing to\n"
			<< "<operaton> \"get\" request a file to be downloaded, \"list\" request list of downloadable files\n"
			<< "[file] file name of the file for download\n"
			<< "[flags] -v gives verbose information";

		return 1;
	}

	for (size_t i = 3; i < argc; ++i)
		if (::strcmp(argv[i], "-v"))
			g_verbose = true;

	try
	{
		//Create io context
		asio::io_context io;

		//Connect to server
		verboseLog("Connecting...");
		Connection con(io, argv[1], argv[2]);
		verboseLog("Connection with: " + con.IP().to_string());

		//If arg is list
		if (::strcmp(argv[3], "list") == 0)
			handleList(con);

		//If arg is get
		else if (::strcmp(argv[3], "get") == 0)
			handleFile(con, argv[4]);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
		return 1;
	}

	return 0;
}