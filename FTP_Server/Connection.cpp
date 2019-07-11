#include "Connection.h"

Connection::Connection(asio::io_context& io, const size_t& port)
	: m_io(io)
	, m_acceptor(io, tcp::endpoint(tcp::v4(), port))
{
}

void Connection::open()
{
	//Socket must be reconstructed for it to function propably again
	m_socket = std::make_unique<tcp::socket>(m_io);
	m_acceptor.accept(*m_socket);
}

void Connection::write(const std::string_view& str)
{
	//Write to connection
	std::error_code err;
	m_socket->wait(tcp::socket::wait_write);
	asio::write(*m_socket, asio::buffer(std::string(str) + "\n\r\n\r"), err);

	//If error occured
	if (err)
		ctl::Log::log(err.message(), ctl::Log::Sev::WARNING);
}

std::string Connection::read()
{
	asio::streambuf buffer;

	//Fill buffer with message
	std::error_code err;
	auto len = asio::read_until(*m_socket, buffer, "\n\r\n\r", err);

	//If connection broke
	if (err == asio::error::eof)
		return "EOF"s;

	//If error occured
	else if (err)
		throw ctl::Log(err.message());

	//Create string message minus the delimiter
	return std::string(asio::buffers_begin(buffer.data()), asio::buffers_begin(buffer.data()) + len - 4);
}