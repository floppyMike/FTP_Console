#pragma once

#include "Globals.h"

class Connection
{
public:
	Connection(asio::io_context&, const size_t&);

	void open();

	void write(const std::string_view&);
	std::string read();

	asio::ip::address IP() const { return m_socket->remote_endpoint().address(); }

private:
	asio::io_context& m_io;

	std::unique_ptr<tcp::socket> m_socket;
	tcp::acceptor m_acceptor;
};

