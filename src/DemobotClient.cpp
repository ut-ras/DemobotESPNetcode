/**
 * File: DemobotClient.h
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Demobots General
 * Organization: UT IEEE RAS
 * Description: Implements definitions for the DemobotClient class, which
 * allows the ESP32 to send requests to a DemobotServer.
 */
#include "DemobotClient.h"


DemobotClient::DemobotClient() {
    _request = NULL;
}

int DemobotClient::pingServer(const String url) {
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
        _request->open("GET", url.c_str());
        _request->send();
    }

    while (!isFinished) delay(100);
    return responseCode;
}

void DemobotClient::sendGETRequest(
    const String url,
    const String keys[],
    const String vals[],
    const int argSize,
    const httpRequestCallbackPtr_t handler) {
    String queryPath = url;
    queryPath += '?';

    /* Append keys and values to the path. */
    for (int i = 0; i < argSize; i++) {
        queryPath += keys[i];
        queryPath += '=';
        queryPath += vals[i];
        if (i < (argSize - 1)) {
            queryPath += '&';
        }
    }

    /* Request must be aborted if it was previously generated, since successive
     * calls to the same endpoint don't seem to ever close. */
    if (_request != NULL) _request->abort();
    else _request = new asyncHTTPrequest();

    /* Set the response handler and send the request. */
    _request->onReadyStateChange(handler);
    if (_request->readyState() == 0 || _request->readyState() == 4) {
        _request->open("GET", queryPath.c_str());
        _request->send();
    }
}

void DemobotClient::sendPOSTRequest(
    const String url,
    const String keys[],
    const String vals[],
    int argSize,
    const httpRequestCallbackPtr_t handler) {
    String data = "";

    /* Append keys and values to the path. */
    for (int i = 0; i < argSize; i++) {
        data += keys[i];
        data += '=';
        data += vals[i];
        if (i < (argSize - 1)) {
            data += '&';
        }
    }

    /* Request must be aborted if it was previously generated, since successive
     * calls to the same endpoint don't seem to ever close. */
    if (_request != NULL) _request->abort();
    else _request = new asyncHTTPrequest();

    /* Set the response handler and send the request. */
    _request->onReadyStateChange(handler);
    if (_request->readyState() == 0 || _request->readyState() == 4) {
        _request->open("POST", url.c_str());
        _request->setReqHeader("Content-Type", "application/x-www-form-urlencoded");
        _request->setReqHeader("Content-Length", data.length());
        _request->send(data);
    }
}

DemobotClient::~DemobotClient() {
    delete _request;
}