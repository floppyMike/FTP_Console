#include "Connection.h"

Connection::Connection(asio::io_context& io, const char* host, const char* port)
	: m_socket(io)
{
	//Create endpoint iterator
	tcp::resolver r(io);
	tcp::resolver::query q(host, port);
	m_endpoint = r.resolve(q);

	//Connect with server
	asio::connect(m_socket, m_endpoint);
}

void Connection::write(const std::string_view& str)
{
	//Write to connection
	std::error_code err;
	asio::write(m_socket, asio::buffer(std::string(str) + "\r\n\r\n"), err);
	
	if (err)
		ctl::Log::log(err.message(), ctl::Log::Sev::WARNING);
}

std::string Connection::read()
{
	std::string stream;

	while (true)
	{
		std::array<char, 128> buffer;

		//Fill buffer with message
		std::error_code err;
		auto len = m_socket.read_some(asio::buffer(buffer), err);

		//Append it to the stream
		stream.append(buffer.data(), len);

		//If connection broke
		if (err == asio::error::eof)
		{
			stream = "EOF";
			break;
		}

		//If error occured
		else if (err)
			throw ctl::Log(err.message());

		//If seperator is reached
		else if (std::equal(stream.end() - 4, stream.end(), "\r\n\r\n"))
		{
			//Erase seperator
			stream.erase(stream.end() - 4, stream.end());
			break;
		}
	}

	return stream;
}
