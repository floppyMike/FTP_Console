#pragma once

#include "Globals.h"

class Connection
{
public:
	Connection(asio::io_context&, const char*, const char*);

	void write(const std::string_view&);
	std::string read();

	auto IP() const { return m_socket.remote_endpoint().address(); }

private:
	tcp::resolver::iterator m_endpoint;
	tcp::socket m_socket;
};

