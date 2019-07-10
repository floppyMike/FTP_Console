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
	asio::write(*m_socket, asio::buffer(std::string(str) + "\r\n\r\n"), err);

	//If error occured
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
		auto len = m_socket->read_some(asio::buffer(buffer), err);

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