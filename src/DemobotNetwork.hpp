/**
 * @file DemobotNetwork.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages network configuration for various Demobots.
 * @version 0.2.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#define RETRY_WAIT 1000      // 1 s
#define RETRY_AMOUNT 10
#define NUM_CREDENTIALS 4

class DemobotNetwork {
    public:
        /**
         * @brief Construct a new Demobot Network object and sets up network
         * configuration.
         *
         * @param demobot_name Name of the demobot and the device hostname.
         */
        explicit DemobotNetwork(const char *demobot_name);

        /**
         * @brief Creates an access point based on the existing network configuration.
         *
         * @return true The AP was setup.
         * @return false The AP was not setup.
         */
        bool setup_network(void);

        /**
         * @brief Connects to an existing network given the existing network
         * configuration.
         *
         * @return true Connected to the network.
         * @return false Not connected to the network.
         */
        bool connect_network(void);

        /**
         * @brief Disconnects to any existing network (AP and STA).
         */
        void disconnect_network(void);

        /**
         * @brief Gets the current connection status to the network defined in
         * the existing network configuration.
         *
         * @return int
         */
        int get_network_status(void) const;

        /**
         * @brief Get the network configuration SSID.
         *
         * @return const char*
         */
        const char *get_network_ssid(void) const;

        /**
         * @brief Get the network configuration password.
         *
         * @return const char*
         */
        const char *get_network_pass(void) const;

        /**
         * @brief Destroy the Demobot Network object and disconnects to any
         * existing network.
         */
        ~DemobotNetwork();
    private:
        /**
         * @brief Find a matching available network in credential order.
         *
         * @param ssid SSID to fill
         * @param password Password to fill
         * @return true Success, populated
         * @return false Failure, not populated
         */
        bool get_available_network(char **ssid, char **password);

    private:
        /** Robot name. */
        char *_demobot_name;

        /** Network configuration. */
        const char *_ssid;
        const char *_pass;

        /**
         * Represents a single possible network that can be connected to or established.
         * Has a SSID and WPA2/PSK password associated with it.
         */
        struct Credential {
            const char *SSID;
            const char *PASS;
        };

        /** Credentials log. */
        const int _num_credentials = NUM_CREDENTIALS;
        const Credential *_credentials_log = nullptr;
};
