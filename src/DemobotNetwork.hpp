/**
 * @file DemobotNetwork.hpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages network configuration for various Demobots.
 * @version 0.2.0
 * @date 2023-04-10
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#define RETRY_WAIT 1000      // 1 s
#define RETRY_AMOUNT 10
#define NUM_CREDENTIALS 4

class DemobotNetwork {
    public:
        /** List of all possible Demobots the DemobotNetwork supports. */
        enum DemobotID {
            DANCEBOT,
            POLARGRAPH,
            MARQUEE,
            TOWER_OF_POWER
        };

        explicit DemobotNetwork(const char *demobot_name, DemobotID id);

        bool setup_network(void);

        bool connect_network(void);

        void disconnect_network(void);

        int get_network_status(void) const;

        const char *get_network_ssid(void) const;

        const char *get_network_pass(void) const;

        ~DemobotNetwork();
    private:
        bool get_available_network(char **ssid, char **password);

    private:
        /** Robot name. */
        char *_demobot_name;

        /** Network info */
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
