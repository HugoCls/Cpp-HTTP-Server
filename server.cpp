#include "server.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
	Router router;

	// Route 1: Retourner le nom du cours et les auteurs avec une page HTML et un fichier CSS externe
    Route route1 = Route("/", [](const Request& req) -> Response {
        Response res;

        std::string htmlContent = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta http-equiv="X-UA-Compatible" content="IE=edge">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Course Info</title>
            <link rel="stylesheet" type="text/css" href="/style.css">
        </head>
        <body>
            <div class="container">
                <h1>Course: C++ Web Server</h1>
                <p>Authors: Hugo Colson and James Maistret</p>
            </div>
        </body>
        </html>
        )";

        res.addHeader("Content-Type", "text/html");
        res.setBody(htmlContent);

        return res;
    });


    // Route 2: Retourner une image
    Route route2 = Route("/image", [](const Request& req) -> Response {
        Response res;

        std::ifstream imageFile("data/image.png", std::ios::binary);
        if (imageFile) {
            
            std::ostringstream oss;
            oss << imageFile.rdbuf();
            std::string imageData = oss.str();
            
            // En-têtes PNG
            res.addHeader("Content-Type", "image/png");
            res.addHeader("Content-Length", std::to_string(imageData.size()));
            res.setBody(imageData);
        } else {
            res.setStatus(404);
            res.setStatusText("Not Found");
            res.setBody("Image not found.");
        }

        return res;
    });

    // Route 3: Exemple d'une route POST
    Route route3 = Route("/submit", [](const Request& req) -> Response {
        Response res;
        if (req.getverb() == Request::POST) {
            
            res.setBody("Data received: " + req.getbody());
        } else {
            res.setStatus(405);
            res.setStatusText("Method Not Allowed");
        }
        return res;
    });

    // Route 4: Redirection vers la racine
    Route routeRedirect = Route("/redirect", [](const Request& req) -> Response {
        Response res;
        res.setStatus(302);
        res.setStatusText("Found");
        res.addHeader("Location", "/");
        return res;
    });

    // routeFavicon: Servir le fichier favicon.ico
    Route routeFavicon = Route("/favicon.ico", [](const Request& req) -> Response {
        Response res;
        
        std::ifstream faviconFile("data/favicon.ico", std::ios::binary);
        if (faviconFile) {
            std::ostringstream oss;
            oss << faviconFile.rdbuf();
            std::string faviconData = oss.str();
            
            res.addHeader("Content-Type", "image/x-icon");
            res.addHeader("Content-Length", std::to_string(faviconData.size()));
            res.setBody(faviconData);
        } else {
            res.setStatus(404);
            res.setStatusText("Not Found");
            res.setBody("Favicon not found.");
        }

        return res;
    });

    // routeCss: Servir le fichier style.css
    Route routeCss = Route("/style.css", [](const Request& req) -> Response {
        Response res;

        std::ifstream cssFile("data/style.css");
        if (cssFile) {
            std::ostringstream oss;
            oss << cssFile.rdbuf();
            std::string cssContent = oss.str();
            
            res.addHeader("Content-Type", "text/css");
            res.addHeader("Content-Length", std::to_string(cssContent.size()));
            res.setBody(cssContent);
        } else {
            res.setStatus(404);
            res.setStatusText("Not Found");
            res.setBody("CSS file not found.");
        }

        return res;
    });


    // Ajout des routes au routeur
    router.addRoute(&route1);
    router.addRoute(&route2);
    router.addRoute(&route3);
    router.addRoute(&routeFavicon);
    router.addRoute(&routeCss);

	this->router = router;
	
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
                Response response;

				if (!(stream >> rq)) {
                    response.setStatus(400);
                    response.setStatusText("Bad request");
                    std::cout << "Request received unreadable\n";
                }
				
                else {
                    response = router.serveRequest(rq);
                    
                    std::cout << "Request received:\n";
                    std::cout << rq;
                }
				
				stream << response.toString();
				std::cout << "\nResponse:\n";
				std::cout << response.toString();
				std::cout << "\n";
				break;
			}
		}
		catch(const std::exception& e)
		{   
            Response response;
            response.setStatus(500);
            response.setStatusText("Internal Server Error");
            stream << response.toString();
            std::cout << "\nResponse:\n";
            std::cout << response.toString();
            std::cout << "\n";

			std::cerr << e.what() << '\n';
		}
		
		
		std::cout << "Client disconnected" << std::endl;
	}

	std::cout << "Server stopped" << std::endl;
}
