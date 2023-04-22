/**
 * @file DemobotClient.cpp
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Manages cient communication between demobots.
 * @version 0.2.0
 * @date 2023-04-20
 * @copyright Copyright (c) 2023
 *
 */
#include "DemobotClient.hpp"
#include <ArduinoLog.h>

DemobotClient::DemobotClient(const IPAddress ip) {
    Log.trace("[DemobotClient]");

    _ip = new IPAddress(ip);

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

bool DemobotClient::send_HTTP_request(
    const char *endpoint,
    const WebRequestMethod mode,
    const char *keys[],
    const char *vals[],
    const int num_args,
    String *response) {

    Log.trace("[send_HTTP_request]");
    if (_ip == nullptr) {
        Log.trace("\tServer IP was improperly configured.");
        return false;
    }

    /* Send the request. */
    bool success = false;
    #ifdef SYNC
    int http_code;
    if (mode == WebRequestMethod::HTTP_GET) {
        String url = String("http://") + IpAddress2String(*_ip) + String(":80") + String(endpoint); // TODO do not hardcode port
        url += '?';

        /* Append keys and values to the path. */
        for (int i = 0; i < num_args; i++) {
            url += keys[i];
            url += '=';
            url += vals[i];
            if (i < (num_args - 1)) {
                url += '&';
            }
        }

        Log.trace("\tGET URL %s", url.c_str());
        _client->begin(url.c_str());
        http_code = _client->GET();

    } else if (mode == WebRequestMethod::HTTP_POST) {
        String url = String("http://") + IpAddress2String(*_ip) + String(":80") + String(endpoint); // TODO do not hardcode port
        String payload;

        Log.trace("\tPOST URL %s", url.c_str());
        _client->begin(url.c_str());
        _client->addHeader("Content-Type", "application/x-www-form-urlencoded");

        /* Append keys and values to the payload. */
        for (int i = 0; i < num_args; i++) {
            payload += keys[i];
            payload += '=';
            payload += vals[i];
            if (i < (num_args - 1)) {
                payload += '&';
            }
        }

        Log.trace("\tPayload %s", payload.c_str());
        http_code = _client->POST(payload.c_str());
    } else {
        Log.trace("\tWeb request method %i not supported.", mode);
        return false;
    }

    if (http_code > 0) {
        Log.trace("\tGot http response code %i.", http_code);
        if (http_code == HTTP_CODE_OK) {
            *response = _client->getString();
            Log.trace("\tPayload %s.", response->c_str());
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