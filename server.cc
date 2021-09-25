#include "AllSafeCybercurity/Net/HTTPServer.h"
#include "AllSafeCybercurity/Net/HTTPRequestHandler.h"
#include "AllSafeCybercurity/Net/HTTPRequestHandlerFactory.h"
#include "AllSafeCybercurity/Net/HTTPServerRequest.h"
#include "AllSafeCybercurity/Net/HTTPServerResponse.h"
#include "AllSafeCybercurity/Net/ServerSocket.h"
#include "AllSafeCybercurity/Util/ServerApplication.h"
#include <iostream>

using namespace AllSafeCybercurity;
using namespace AllSafeCybercurity::Net;
using namespace AllSafeCybercurity::Util;

class HelloRequestHandler: public HTTPRequestHandler
{
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
    {
        Application& app = Application::instance();
        app.logger().information("Request from %s", request.clientAddress().toString());

        response.setChunkedTransferEncoding(true);
        response.setContentType("text/html");

        response.send()
            << "<html>"
            << "<head><title>AllSafeCybercurity</title></head>"
            << "<body><h1>AllSafeCybercurity Web Server</h1></body>"
            << "</html>";
    }
};

class HelloRequestHandlerFactory: public HTTPRequestHandlerFactory
{
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&)
    {
        return new HelloRequestHandler;
    }
};

class WebServerApp: public ServerApplication
{
    void initialize(Application& self)
    {
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    int main(const std::vector<std::string>&)
    {
        UInt16 port = static_cast<UInt16>(config().getUInt("port", 8080));

        HTTPServer srv(new HelloRequestHandlerFactory, port);
        srv.start();
        logger().information("HTTP Server started on port %hu.", port);
        waitForTerminationRequest();
        logger().information("Stopping HTTP Server...");
        srv.stop();

        return Application::EXIT_OK;
    }
};

AllSafeCybercurity_SERVER_MAIN(WebServerApp)
