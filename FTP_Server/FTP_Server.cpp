// FTP_Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Utility.h"
#include "Connection.h"

std::queue<std::string_view> ctl::Log::m_buf;
bool g_verbose = false;

using asio::ip::tcp;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		std::cerr << "Usage: <port> <directory> [flags]\n"
			<< "<port> port on which to host the server\n"
			<< "<directory> directory which will be managed\n"
			<< "[flags] -v program prints its operations.";

		return 1;
	}

	for (size_t i = 3; i < argc; ++i)
		if (::strcmp(argv[3], "-v") == 0)
			g_verbose = true;

	try
	{
		//Create io and accepter
		asio::io_context io;

		Connection con(io, ::atoi(argv[1]));

		while (true)
		{
			//Refresh connector
			verboseLog("Connector started.");
			con.open();
			verboseLog("Connection with: " + con.IP().to_string());

			//Read input from console
			verboseLog("Reading from socket.");
			const auto opt = con.read();
			verboseLog("Reading finished with: "s + opt);

			//Seperate option and data
			const auto sep = opt.find(';');
			std::pair<std::string, std::string> option(opt.substr(0, sep), sep == opt.size() ? ""s : opt.substr(sep + 1));

			//If list is requested
			if (option.first == "GET_LIST")
				handleList(con, argv[2]);

			//If files are requested
			else if (option.first == "GET_FILE" && !option.second.empty())
				handleFile(con, option.second);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
		return 1;
	}

	return 0;
}
