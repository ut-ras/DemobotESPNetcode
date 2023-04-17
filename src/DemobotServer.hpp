/**
 * @file DemobotServer.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages server communication between demobots.
 * @version 0.2.0
 * @date 2023-04-14
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <ESPAsyncWebServer.h>
#include <IPAddress.h>

#define DEFAULT_PORT 80
#define STA 0
#define AP 1

typedef void (*serverCallbackPtr_t)(AsyncWebServerRequest *request);

class DemobotServer {
    public:
        enum DemobotID {
            DANCEBOT,
            POLARGRAPH,
            MARQUEE,
            TOWER_OF_POWER
        };

        struct Route {
            const char *uri;
            const WebRequestMethodComposite type;
            const serverCallbackPtr_t cb;
        };

        enum StaticIPMode {
            MODE_STA,
            MODE_AP
        };

        DemobotServer(const DemobotID id);

        DemobotServer(const DemobotID id, const uint8_t port);

        bool setup_server(const Route routes[], const uint8_t num_routes, const StaticIPMode mode);

        bool start_server(void);

        void stop_server(void);

        void kill_server(void);

        bool add_server_GET_endpoint(const char *endpoint, serverCallbackPtr_t handler);

        bool add_server_POST_endpoint(const char *endpoint, serverCallbackPtr_t handler);

        const IPAddress get_server_ip(void) const;

        ~DemobotServer();

    private:
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
        AsyncWebServer *_server = nullptr;
        uint8_t _port = DEFAULT_PORT;
};


