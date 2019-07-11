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
	asio::write(m_socket, asio::buffer(std::string(str) + "\n\r\n\r"), err);
	
	if (err)
		ctl::Log::log(err.message(), ctl::Log::Sev::WARNING);
}

std::string Connection::read()
{
	static asio::streambuf buffer;

	//Fill buffer with message
	std::error_code err;
	auto len = asio::read_until(m_socket, buffer, "\n\r\n\r", err);

	//If connection broke
	if (err == asio::error::eof)
		return "EOF"s;

	//If error occured
	else if (err)
		throw ctl::Log(err.message());

	//Extract data (without delimiter)
	std::string data(asio::buffers_begin(buffer.data()), asio::buffers_begin(buffer.data()) + len - 4);
	buffer.consume(len);

	return data;
}
