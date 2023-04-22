/**
 * @file DemobotServer.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages server communication between demobots.
 * @version 0.2.0
 * @date 2023-04-20
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
        /** Route used for processing client requests. */
        struct Route {
            /** Endpoint (e.g. '/'. '/hello'). */
            const char *uri;

            /** Type of request to process (WebRequestMethod). */
            const WebRequestMethodComposite type;

            /** Callback to process server request. */
            const serverCallbackPtr_t cb;
        };

        /** Network mode that affects static IP address setup. */
        enum StaticIPMode {
            MODE_STA,
            MODE_AP
        };

        /**
         * @brief Construct a new Demobot Server object and captures the target
         * IP address of the robot.
         *
         * @param ip Robot ip address.
         */
        DemobotServer(const IPAddress ip);

        /**
         * @brief Construct a new Demobot Server object and captures the target
         * IP address of the robot ID.
         *
         * @param ip Robot ip address.
         * @param port Server port listening on.
         */
        DemobotServer(const IPAddress ip, const uint8_t port);

        /**
         * @brief Sets up the server on the given IP and hooks up any provided
         * routes.
         *
         * @param routes Routes to set up.
         * @param num_routes Number of routes to set up.
         * @param mode Network mode.
         * @return true Server was initialized.
         * @return false Server was not initialized.
         */
        bool setup_server(const Route routes[], const uint8_t num_routes, const StaticIPMode mode);

        /**
         * @brief Starts the server.
         *
         * @return true The server has been started.
         * @return false The server has not been started.
         */
        bool start_server(void);

        /**
         * @brief Stops the server.
         */
        void stop_server(void);

        /**
         * @brief Deallocates the server.
         */
        void kill_server(void);

        /**
         * @brief Manually add a server route.
         *
         * @param route Route to add.
         * @return true Route has been added.
         * @return false Route has not been added.
         */
        bool add_server_route(const Route route);

        /**
         * @brief Get the IP address assigned to the server.
         *
         * @return const IPAddress Address assigned.
         */
        const IPAddress get_server_ip(void) const;

        /**
         * @brief Destroy the Demobot Server object
         */
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
        /** Server IP address. */
        IPAddress *_ip = nullptr;

        /** Server instance. */
        AsyncWebServer *_server = nullptr;

        /** Server port. */
        uint8_t _port = DEFAULT_PORT;
};
