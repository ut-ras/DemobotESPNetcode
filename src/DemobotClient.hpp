/**
 * @file DemobotClient.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages cient communication between demobots.
 * @version 0.2.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <ESPAsyncWebServer.h>
#include <IPAddress.h>

#define SYNC
#ifdef SYNC
#include <HTTPClient.h>
#elif
#include <AsyncHttpClient.h>
#endif


class DemobotClient {
    public:
        /**
         * @brief Construct a new Demobot Client object and captures the target
         * IP address of the robot.
         *
         * @param ip Robot ip address.
         */
        DemobotClient(const IPAddress ip);

        /**
         * @brief Pings the root of the server looking for a 200 OK HTTP
         * response. Blocking.
         *
         * @return true Positive 200 OK HTTP response.
         * @return false Negative 200 OK HTTP response.
         */
        bool ping_server(void);

        bool send_HTTP_request(
            const char *endpoint,
            const WebRequestMethod mode,
            const char *keys[],
            const char *vals[],
            const int num_args,
            String *response);

        /**
         * @brief Destroy the Demobot Client object.
         */
        ~DemobotClient();

    private:
    #ifndef SYNC
        void response_handler(Response response);

        void data_handler(char *data, size_t len);

        void error_handler(int error);
    #endif

        /**
         * Converts an IPAddress to a string.
         * @author: apicquot from https://forum.arduino.cc/index.php?topic=228884.0
         *
         * @param[in] ipAddress IPAddress object.
         * @return String concatenation of the IP address.
         */
        String IpAddress2String(const IPAddress ipAddress) const;

    private:
        IPAddress *_ip = nullptr;
        #ifdef SYNC
        HTTPClient *_client = nullptr;
        #elif
        AsyncHttpClient *_client = nullptr;
        #endif
};
