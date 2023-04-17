/**
 * @file DemobotClient.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages cient communication between demobots.
 * @version 0.2.0
 * @date 2023-04-14
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <IPAddress.h>

#define SYNC
#ifdef SYNC
#include <HTTPClient.h>
#elif
#include <AsyncHttpClient.h>
#endif


class DemobotClient {
    public:
        enum DemobotID {
            DANCEBOT,
            POLARGRAPH,
            MARQUEE,
            TOWER_OF_POWER
        };

        DemobotClient(const DemobotID id);

        bool ping_server(void);

        // void sendGETRequest(
        //     const char *endpoint,
        //     const String keys[],
        //     const String vals[],
        //     const int argSize,
        //     const httpRequestCallbackPtr_t handler);

        // void sendPOSTRequest(
        //     const char *endpoint,
        //     const String keys[],
        //     const String vals[],
        //     const int argSize,
        //     const httpRequestCallbackPtr_t handler);

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