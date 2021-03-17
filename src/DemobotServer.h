/**
 * File: DemobotServer.h
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Demobots General
 * Organization: UT IEEE RAS
 * Description: Implements definitions for the DemobotServer class, which
 * allows the ESP32 to set up and manage a web server.
 */
#pragma once

#include <ESPAsyncWebServer.h>
#include <asyncHTTPrequest.h>


typedef void (serverCallbackPtr_t)(AsyncWebServerRequest *request);

class DemobotServer {
    /**
     * The DemobotServer class allows the Demobot to set up a web server and
     * manage HTTP requests by clients.
     */
    public:
        /** Creates a new DemobotServer at port 80. */
        DemobotServer();

        /**
         * Creates a new DemobotServer at the specified port.
         * @param[in] port Port of new server.
         */
        DemobotServer(const unsigned int port);

        /**
         * Starts the DemobotServer. Add endpoints prior to this.
         *
         * @return True if the server exists and is turned on. False otherwise.
         */
        bool startServer();

        /**
         * Stops the DemobotServer.
         *
         * @return True if the server exists and is turned off. False otherwise.
         */
        bool stopServer();

        /**
         * Pings the configured ip address to see if we can get a response.
         * Useful for checking if we need to setup a server or not.
         *
         * @param[in] ipAddress The root identifier of the server location.
         * @return Response code after sending a GET request to the server root
         *      endpoint ('/').
         * @note This is a blocking call.
         */
        int pingServer(String ipAddress);

        /**
         * Adds an endpoint, if it does not already exist, for responding to
         * client GET requests.
         *
         * @param[in] endpoint URL request endpoint.
         * @param[in] handler User defined function pointer that specifies what
         *                    happens when a GET request is received.
         * @return True if endpoint was set up. False otherwise.
         */
        bool addGETEndpoint(const String endpoint, const serverCallbackPtr_t handler);

        /**
         * Adds an endpoint, if it does not already exist, for responding to
         * client POST requests.
         *
         * @param[in] endpoint URL request endpoint.
         * @param[in] handler User defined function pointer that specifies what
         *                    happens when a POST request is received.
         * @return True if endpoint was set up. False otherwise.
         */
        bool addPOSTEndpoint(const String endpoint, const serverCallbackPtr_t handler);

        /**
         * Adds an endpoint, if it does not already exist, for responding to 404
         * endpoints that don't exist.
         *
         * @param[in] handler User defined function pointer that specifies what
         *                    happens when a resource was not found.
         */
        bool addOnNotFound(const serverCallbackPtr_t handler);

        ~DemobotServer();

    private:
        /** Port for webserver to open on. */
        unsigned int _port;

        /** Server object to manage. */
        AsyncWebServer *_server;

        /** Request object. Used for pinging if a server is alive. */
        asyncHTTPrequest *_request;
};
