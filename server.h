#pragma once
#include <asio.hpp>
#include "router.h"
#include "request.h"
#include "response.h"

using asio::ip::tcp;

class Server
{
private:
	asio::io_context& m_io;
	tcp::acceptor m_acceptor;
	Router router;

	constexpr static int PORT = 5555;

public:
	Server(asio::io_context& ioContext);
	void setRouter(const Router& router);
	void run();
};