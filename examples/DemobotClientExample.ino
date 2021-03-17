/**
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Dancebot
 * File: DemobotClientExample.ino
 * Description: Example sketch for setting up a client and sending a message to
 * a server.
 * Organization: UT IEEE RAS
 */
#include <Arduino.h>
#include <DemobotNetwork.h>
#include <DemobotServer.h>
#include <DemobotClient.h>


/** Network instantiation */
DemobotNetwork *network;
DemobotServer *server;
DemobotClient *client;


void onRoot(AsyncWebServerRequest *request) {
    Serial.println("[SERVER] The server got a client trying to access root!");
    request->send(200, "text/plain", "Hello, world");
}

void onNotFound(AsyncWebServerRequest *request) {
    Serial.println("[SERVER] The server got a client trying to access a not found.");
    request->send(404, "text/plain", "404 Not Found.");
}

void onRobotJoinServer(AsyncWebServerRequest *request) {
    Serial.println("[SERVER] A robot is attempting to join the server.");

    if (request->hasParam("ID")) {
        Serial.print("GET Request with payload ID: ");
        Serial.print(request->getParam("ID")->value());
        Serial.println(".");
    }

    if (request->hasParam("ID", true)) {
        Serial.print("POST Request with payload ID: ");
        Serial.print(request->getParam("ID", true)->value());
        Serial.println(".");
    }

    request->send(200, "text/plain", "OK");
}

void onRobotJoinClient(void *optParm, asyncHTTPrequest *request, int readyState) {
    if (readyState == 4) {
        if (request->responseHTTPcode() == 200) {
            Serial.println("[CLIENT] Robot joined the server.");
        } else {
            Serial.println("[CLIENT] Robot did not join the server.");
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nDemobotClientExample.ino.");
    /* Give some time to open up the serial monitor. */
    delay(3000);

    /* Start up the network. */
    Serial.println("\nStarting network configuration.");
    network = new DemobotNetwork(DemobotNetwork::DANCEBOT_1);
    network->connectNetwork();
    delay(100);

    /* Check that the robot is connected to the network. */
    Serial.print("Is network connected [1=T|0=F]: ");
    Serial.println(network->isNetworkConnected());

    /* Set up the server. */
    Serial.println("\nStarting server setup.");
    server = new DemobotServer();
    server->addGETEndpoint(String("/"), onRoot);
    server->addOnNotFound(onNotFound);

    /* Add an endpoint for robotJoin for GET and POST requests; we'll use this
     * again later. */
    server->addGETEndpoint(String("/robotJoin"), onRobotJoinServer);
    server->addPOSTEndpoint(String("/robotJoinPOST"), onRobotJoinServer);
    server->startServer();
    delay(100);

    /* Check that the server is set up and can be pinged. */
    IPAddress ip = network->getIPAddress();
    int response = server->pingServer(network->IpAddress2String(ip));
    Serial.print("HTTP response pinging the server: ");
    Serial.println(response);

    /* Set up the client. */
    Serial.println("\nStarting client setup.");
    String url = String("http://" + network->IpAddress2String(ip) + ":80/");
    client = new DemobotClient();
    response = client->pingServer(url);
    Serial.print("HTTP response of client pinging the server: ");
    Serial.println(response);

    /* Send a GET request to robotJoin, looking for a response. */
    Serial.println("\nAttempting to join a robot to the server.");
    int numArgs = 2;
    String keys[numArgs] = {String("ID"), String("BATTERY")};
    String vals[numArgs] = {String("0"), String("50")};
    client->sendGETRequest(url + "robotJoin", keys, vals, numArgs, onRobotJoinClient);

    /* An asynchronous call may need a little bit of time to get ready. */
    delay(100);

    /* Send a POST request to robotJoinPOST, looking for a response. */
    Serial.println("\nAttempting to join a robot to the server with POST.");
    client->sendPOSTRequest(url + "robotJoinPOST", keys, vals, 1, onRobotJoinClient);

    /* An asynchronous call may need a little bit of time to get ready. */
    delay(100);
}

void loop() {}
