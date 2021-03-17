/**
 * File: DemobotServer.cpp
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Demobots General
 * Organization: UT IEEE RAS
 * Description: Implements definitions for the DemobotServer class, which
 * allows the ESP32 to set up and manage a web server.
 */
#include "DemobotServer.h"


/** Public methods. */

DemobotServer::DemobotServer() {
    _server = new AsyncWebServer(80);
    _request = NULL;
    _port = 80;
}

DemobotServer::DemobotServer(const unsigned int port) {
    _server = new AsyncWebServer(port);
    _request = NULL;
    _port = port;
}


bool DemobotServer::startServer() {
    if (_server != nullptr) {
        _server->begin();
        return true;
    }
    return false;
}

bool DemobotServer::stopServer() {
    if (_server != nullptr) {
        _server->end();
        return true;
    }
    return false;
}

int DemobotServer::pingServer(String ipAddress) {
    /* Send a root level GET request to see if the server exists. */
    bool isFinished = false;
    int responseCode = -1;

    /* Request must be aborted if it was previously generated, since successive
     * calls to the same endpoint don't seem to ever close. */
    if (_request != NULL) _request->abort();
    else _request = new asyncHTTPrequest();

    _request->onReadyStateChange(
        [&responseCode, &isFinished](void *optParam, asyncHTTPrequest *request, int readyState) {
            if (readyState == 4) {
                isFinished = true;
                responseCode = request->responseHTTPcode();
            }
        }
    );
    if (_request->readyState() == 0 || _request->readyState() == 4) {
        String address = ipAddress + ":" + _port + "/";
        _request->open("GET", address.c_str());
        _request->send();
    }

    while (!isFinished) {
        delay(100);
    }

    return responseCode;
}

bool DemobotServer::addGETEndpoint(const String endpoint, const serverCallbackPtr_t handler) {
    if (_server != nullptr) {
        _server->on(endpoint.c_str(), HTTP_GET, handler);
        return true;
    }
    return false;
}

bool DemobotServer::addPOSTEndpoint(const String endpoint, const serverCallbackPtr_t handler) {
    if (_server != nullptr) {
        _server->on(endpoint.c_str(), HTTP_POST, handler);
        return true;
    }
    return false;
}

bool DemobotServer::addOnNotFound(const serverCallbackPtr_t handler) {
    if (_server != nullptr) {
        _server->onNotFound(handler);
        return true;
    }
    return false;
}

DemobotServer::~DemobotServer() {
    stopServer();
    delete _server;
    delete _request;
}