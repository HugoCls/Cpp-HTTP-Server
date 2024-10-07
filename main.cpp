#include "server.h"
#include "request.h"
#include "response.h"

int main()
{
	asio::io_context ioContext;
	Server server{ioContext};
    
	server.run();
}