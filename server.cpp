#include "server.h"
#include <iostream>

Server::Server(asio::io_context& ioContext):
	m_io{ioContext},
	m_acceptor{ioContext, tcp::endpoint{tcp::v4(), PORT}}
{
}

void Server::setRouter(const Router& router) {
    this->router = router;
}

void Server::run()
{	
	std::cout << "Server listening" << std::endl;

	for (;;) // infinite loop, as long as clients connect or something crashes
	{
		tcp::iostream stream;
		m_acceptor.accept(stream.socket());

		std::cout << "Client connected" << std::endl;

		try
		{
			for (;;) // boucle infinie, tant que le client est connecté
			{	
				Request rq;

				if (!(stream >> rq)) {
					break;
				}
				
				Response response = router.serveRequest(rq);
				stream << response.toString();

				// Log dans la console les actions du server
				std::cout << "Request received:\n";
				std::cout << rq; // Echo de la requête (ou traitement)
				std::cout << "\nResponse:\n";
				std::cout << response.toString();
				std::cout << "\n";
				break;
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
		
		std::cout << "Client disconnected" << std::endl;
	}

	std::cout << "Server stopped" << std::endl;
}
