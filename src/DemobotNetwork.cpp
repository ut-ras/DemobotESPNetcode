/**
 * @file DemobotNetwork.cpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages network configuration for various Demobots.
 * @version 0.2.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#include "DemobotNetwork.hpp"
#include <ArduinoLog.h>
#include <WiFi.h>

DemobotNetwork::DemobotNetwork(const char *demobot_name) {
    Log.trace("[DemobotNetwork]");
    _credentials_log = new Credential[_num_credentials] {
        Credential{"mono_no_aware", "mudkip27"},
        Credential{"Demobot", "Demobots1234"},
        Credential{"DemobotsNetwork", "Dem0b0tsRu1e!"},
        Credential{"", ""},
    };

    _demobot_name = const_cast<char*>(demobot_name);
    WiFi.setHostname(_demobot_name);

    /* Set internal SSID and password based on available networks. */
    if (get_available_network((char **)&_ssid, (char **)&_pass)) {
        Log.trace("\tFound matching available network: %s.", _ssid);
        Log.trace("\tNetwork configured for STA mode.");
    } else {
        /* Take the first entry (assuming that it's filled) of the credentials log. */
        _ssid = const_cast<char*>(_credentials_log[0].SSID);
        _pass = const_cast<char*>(_credentials_log[0].PASS);

        Log.trace(
            "\tNo matching available networks found. "
            "Defaulting to the first entry in the credentials log: %s.", _ssid
        );
        Log.trace("\tNetwork configured for AP mode.");
    }
}

bool DemobotNetwork::setup_network(void) {
    Log.trace("[setup_network]");
    if ((_ssid == nullptr) || (_pass == nullptr)) {
        Log.trace("\tNo network credentials have been set and thus we cannot setup a network.");
        return false;
    }

    Log.trace("\tSetting up our AP network at: %s", _ssid);
    if (WiFi.softAP(_ssid, _pass)) {
        Log.trace("\tNetwork set up.");
        return true;
    } else {
        Log.trace("\tFailed to set up network.");
        return false;
    }
}

bool DemobotNetwork::connect_network(void) {
    Log.trace("[connect_network]");
    /* If we never found a network with credentials, don't attempt to connect. */
    if (_ssid == nullptr || _pass == nullptr) {
        Log.trace("\tNo network credentials have been set and thus we cannot connect to a network.");
        return false;
    }

    Log.trace("\tConnecting to %s.", _ssid);
    WiFi.begin(_ssid, _pass);

    /* Poll until we get connected or timeout. */
    int retry = 0;
    do {
        Serial.print(".");
        ++retry;
        if (retry > RETRY_AMOUNT) break;
        delay(RETRY_WAIT);
    } while (WiFi.status() != WL_CONNECTED);
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Log.trace("\tConnected to the network.");
        return true;
    } else {
        Log.trace("\tFailed to connect to the network.");
        return false;
    }
}

void DemobotNetwork::disconnect_network(void) {
    Log.trace("[disconnect_network]");
    WiFi.disconnect();
}

int DemobotNetwork::get_network_status(void) const {
    Log.trace("[get_network_status]");
    return WiFi.status();
}

const char * DemobotNetwork::get_network_ssid(void) const {
    Log.trace("[get_network_ssid]");
    if (_ssid != nullptr) return "N/A";
    else return _ssid;
}

const char * DemobotNetwork::get_network_pass(void) const {
    Log.trace("[get_network_pass]");
    if (_pass != nullptr) return "N/A";
    else return _pass;
}

DemobotNetwork::~DemobotNetwork(void) {
    Log.trace("[~DemobotNetwork]");
    WiFi.disconnect();
    free((void *)_credentials_log);
}

bool DemobotNetwork::get_available_network(char **ssid, char **password) {
    Log.trace("[get_available_network]");
    /* 1. scan networks. */
    int networks = WiFi.scanNetworks();
    if (networks == 0) {
        Log.trace("\tNo available networks were found during the scan.");
        return false;
    }

    /* 2. Assign a network to the robot based on credential listing order. */
    for (int i = 0; (i < _num_credentials); i++) {
        for (int j = 0; j < networks; j++) {
            if (WiFi.SSID(j).equals(String(_credentials_log[i].SSID))) {
                *ssid = const_cast<char*>(_credentials_log[i].SSID);
                *password = const_cast<char*>(_credentials_log[i].PASS);
                Log.trace("\tA network matching an entry in our credentials log was found.");
                Log.trace("\tInternal network config set to %s, %s.", *ssid, *password);
                return true;
            }
        }
    }

    /* 2b. if we didn't find a network, set it to default. */
    Log.trace("\tNo networks were found that matched any entries in our credentials log.");
    return false;
}
