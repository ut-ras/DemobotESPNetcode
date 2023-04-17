/**
 * @file DemobotClient.cpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages cient communication between demobots.
 * @version 0.2.0
 * @date 2023-04-14
 * @copyright Copyright (c) 2023
 *
 */
#include "DemobotClient.hpp"
#include <ArduinoLog.h>

DemobotClient::DemobotClient(const DemobotID id) {
    Log.trace("[DemobotClient]");

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

    #ifdef SYNC
    _client = new HTTPClient();
    #elif
    _client = new AsyncHttpClient(*_ip, 80);
    _client->onResponse(response_handler);
    _client->onData(data_handler);
    _client->onError(error_handler);
    #endif
}

bool DemobotClient::ping_server(void) {
    Log.trace("[ping_server]");
    if (_ip == nullptr) {
        Log.trace("\tServer IP was improperly configured.");
        return false;
    }

    /* Send a root level GET request to see if the server exists. */
    String url = String("http://") + IpAddress2String(*_ip) + String(":80"); // TODO do not hardcode port
    Log.trace("\tPinging our target server %s", url.c_str());

    bool success = false;
    #ifdef SYNC
    _client->begin(url.c_str());
    int http_code = _client->GET();
    if (http_code > 0) {
        Log.trace("\tGot http response code %i.", http_code);
        if (http_code == HTTP_CODE_OK) {
            String payload = _client->getString();
            Log.trace("\tPayload %s.", payload.c_str());
            success = true;
        }
    } else {
        Log.trace("\tBad response [%s].", _client->errorToString(http_code).c_str());
    }
    _client->end();
    #elif
    _client->get(url);
    /* TODO: block until response. */
    #endif

    return success;
}

DemobotClient::~DemobotClient() {
    Log.trace("[~DemobotClient]");
    if (_ip != nullptr) {
        free(_ip);
    }
    if (_client != nullptr) {
        free(_client);
    }
}

#ifndef SYNC
void DemobotClient::response_handler(Response response) {
    Log.trace("[response_handler] Got response with status code %i.", response.statusCode);

}

void DemobotClient::data_handler(char *data, size_t len) {
    Log.trace("[data_handler] Got data %s.", data);

}

void DemobotClient::error_handler(int error) {
    Log.trace("[error_handler] Got error code %i.", error);

}
#endif

String DemobotClient::IpAddress2String(const IPAddress ipAddress) const {
    return String(ipAddress[0]) + String(".") +\
        String(ipAddress[1]) + String(".") +\
        String(ipAddress[2]) + String(".") +\
        String(ipAddress[3]) ;
}