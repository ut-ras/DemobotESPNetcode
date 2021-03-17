/**
 * File: DemobotClient.h
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Demobots General
 * Organization: UT IEEE RAS
 * Description: Implements definitions for the DemobotClient class, which
 * allows the ESP32 to send requests to a DemobotServer.
 */
#pragma once

#include <asyncHTTPrequest.h>


typedef void (httpRequestCallbackPtr_t)(
    void *optParm, asyncHTTPrequest *request, int readyState);

class DemobotClient {
    /**
     * The DemobotClient class allows the Demobot to send HTTP requests to
     * DemobotServers.
     */
    public:
        /** Creates a new DemobotClient. */
        DemobotClient();

        /**
         * Pings the configured ip address to see if we can get a response.
         * Useful for checking if we need to setup a server or not.
         *
         * @param[in] url The combined IP Address and port to ping. (i.e.
         *                http://192.168.2.1:80)
         * @return Response code after sending a GET request to the server root
         *         endpoint ('/').
         * @note This is a blocking call.
         */
        int pingServer(const String url);

        /**
         * Submits a GET request and looks for a response. Asynchronous.
         *
         * @param[in] url The combined IP Address, port, and endpoint to ping.
         *                (i.e. http://192.168.2.1:80/hi)
         * @param[in] keys Pointer to an array of strings that each contain a key.
         * @param[in] vals Pointer to an array of strings that each contain a value.
         * @param[in] argSize Number of key-value entries to go through.
         * @param[in] handler User defined function pointer that specifies what
         *                    happens when a server response is received.
         */
        void sendGETRequest(
            const String url,
            const String keys[],
            const String vals[],
            const int argSize,
            const httpRequestCallbackPtr_t handler);

        /**
         * Submits a POST request and looks for a response. Asynchronous.
         *
         * @param[in] url The combined IP Address, port, and endpoint to ping.
         *                (i.e. http://192.168.2.1:80/hi)
         * @param[in] keys Pointer to an array of strings that each contain a key.
         * @param[in] vals Pointer to an array of strings that each contain a value.
         * @param[in] argSize Number of key-value entries to go through.
         * @param[in] handler User defined function pointer that specifies what
         *                    happens when a server response is received.
         */
        void sendPOSTRequest(
            const String url,
            const String keys[],
            const String vals[],
            const int argSize,
            const httpRequestCallbackPtr_t handler);

        ~DemobotClient();

    private:
        /** Request object. */
        asyncHTTPrequest *_request;
};