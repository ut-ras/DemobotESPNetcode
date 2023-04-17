/**
 * @file DemobotServer.cpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages server communication between demobots.
 * @version 0.2.0
 * @date 2023-04-14
 * @copyright Copyright (c) 2023
 *
 */
#include "DemobotServer.hpp"
#include <ArduinoLog.h>

/** Redirect any traffic with an unknown address to here. */
IPAddress gateway(192, 168, 1, 1);

/** 255-245-1 = 9 allowed IP addresses on the subnet. */
IPAddress subnet(255, 255, 255, 0);

/** Optional Domain Name Systems. */
// IPAddress primary_dns(8, 8, 8, 8);
// IPAddress secondary_dns(8, 8, 4, 4);

DemobotServer::DemobotServer(const DemobotID id) {
    Log.trace("[DemobotServer]");
    /* Set server ip based on robot type. */
    switch(id) {
        case DANCEBOT:
            _ip = new IPAddress(192,168,2,1);
            break;
        case POLARGRAPH:
            _ip = new IPAddress(192,168,2,2);
            break;
        case MARQUEE:
            _ip = new IPAddress(192,168,2,3);
            break;
        case TOWER_OF_POWER:
            _ip = new IPAddress(192,168,2,4);
            break;
        default:
            _ip = new IPAddress(192,168,2,0);
            break;
    }
    Log.trace(
        "\tFor your robot, the server will be hosted at %p.",
        *_ip
    );
}

DemobotServer::DemobotServer(const DemobotID id, const uint8_t port) : DemobotServer(id) {
    /* Set server ip based on robot type. */
    _port = port;
}

bool DemobotServer::setup_server(const Route routes[], uint8_t num_routes, const StaticIPMode mode) {
    Log.trace("[setup_server]");
    if (_ip == nullptr) {
        Log.trace("\tServer IP was improperly configured.");
        return false;
    }
    if (_server != nullptr) {
        Log.trace("\tA server already exists! Kill it first.");
        return false;
    }

    Log.trace("\tConfiguring static IP at: %p", *_ip);
    if (mode == STA) {
        if (!WiFi.config(*_ip, gateway, subnet)) {
            Log.trace("\tUnable to setup STA server static IP.");
            return false;
        }
        Log.trace("\tIP ADDRESS: %p.", WiFi.localIP());
    } else {
        if (!WiFi.softAPConfig(*_ip, gateway, subnet)) {
            Log.trace("\tUnable to setup AP server static IP.");
            return false;
        }
        Log.trace("\tIP ADDRESS: %p.", WiFi.softAPIP());
    }

    Log.trace("\tInitializing server to listen to port %u.", _port);
    _server = new AsyncWebServer(_port);

    /* Setup routing. */
    for (uint8_t i = 0; i < num_routes; ++i) {
        Log.trace("[setup_server] Adding endpoint %s", routes[i].uri);
        _server->on(routes[i].uri, routes[i].type, routes[i].cb);
    }

    _server->onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not found");
    });

    return true;
}

bool DemobotServer::start_server(void) {
    Log.trace("[start_server]");
    if (_server == nullptr) {
        Log.trace("\tNo server has been setup.");
        return false;
    } else {
        _server->begin();
        Log.trace("\tServer is now running!");
        return true;
    }
}

void DemobotServer::stop_server(void) {
    Log.trace("[stop_server]");
    if (_server != nullptr) {
        _server->end();
    }
}

void DemobotServer::kill_server(void) {
    Log.trace("[kill_server]");
    if (_server != nullptr) {
        free(_server);
    }
}

bool DemobotServer::add_server_GET_endpoint(const char *endpoint, serverCallbackPtr_t handler) {
    Log.trace("[add_server_GET_endpoint]");
    if (_server != nullptr) {
        _server->on(endpoint, HTTP_GET, handler);
        return true;
    }
    return false;
}

bool DemobotServer::add_server_POST_endpoint(const char *endpoint, serverCallbackPtr_t handler) {
    Log.trace("[add_server_POST_endpoint]");
    if (_server != nullptr) {
        _server->on(endpoint, HTTP_POST, handler);
        return true;
    }
    return false;
}

const IPAddress DemobotServer::get_server_ip(void) const {
    Log.trace("[get_server_ip]");
    if (_ip != nullptr) {
        return IPAddress(*_ip);
    } else {
        return IPAddress();
    }
}

DemobotServer::~DemobotServer() {
    Log.trace("[~DemobotServer]");
    if (_ip != nullptr) {
        free(_ip);
    }
    if (_server != nullptr) {
        free(_server);
    }
}

String DemobotServer::IpAddress2String(const IPAddress ipAddress) const {
    return String(ipAddress[0]) + String(".") +\
        String(ipAddress[1]) + String(".") +\
        String(ipAddress[2]) + String(".") +\
        String(ipAddress[3]) ;
}