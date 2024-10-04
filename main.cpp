#include "server.h"
#include "router.h"
#include "request.h"
#include "response.h"
#include "route.h"
#include <fstream>
#include <sstream>

int main()
{
	asio::io_context ioContext;
	Server server{ioContext};

	Router router;

	// Route 1: Retourner le nom du cours et les auteurs
    Route route1 = Route("/", [](const Request& req) -> Response {
        Response res;
        res.setBody("Course Name: C++ Web Server\nAuthors: Hugo Colson and James Maistret");
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
            res.setBody("Method Not Allowed");
        }
        return res;
    });

    // Ajout des routes au routeur
    router.addRoute(&route1);
    router.addRoute(&route2);
    router.addRoute(&route3);

    server.setRouter(router);
	server.run();
}