/**
 * Author: Matthew Yu
 * Last Modified: 03/16/21
 * Project: Dancebot
 * File: DemobotServerExample.ino
 * Description: Example sketch for setting up a server and endpoints and pinging
 * it.
 * Organization: UT IEEE RAS
 */
#include <Arduino.h>
#include <DemobotNetwork.h>
#include <DemobotServer.h>


/** Network instantiation */
DemobotNetwork *network;
DemobotServer *server;


void onRoot(AsyncWebServerRequest *request) {
    Serial.println("The server got a client trying to access root!");
    request->send(200, "text/plain", "Hello, world");
}

void onNotFound(AsyncWebServerRequest *request) {
    Serial.println("The server got a client trying to access a not found.");
    request->send(404, "text/plain", "404 Not Found.");
}

void setup() {
    Serial.begin(115200);
    Serial.println("DemobotServerExample.ino.");
    /* Give some time to open up the serial monitor. */
    delay(5000);

    /* Start up the network. */
    Serial.println("Starting network configuration.");
    network = new DemobotNetwork(DemobotNetwork::DANCEBOT_1);
    network->connectNetwork();
    delay(1000);

    /* Check that the robot is connected to the network. */
    Serial.print("Network is connected? ");
    Serial.println(network->isNetworkConnected());

    /* Set up the server. */
    Serial.println("Starting server setup.");
    server = new DemobotServer();
    server->addGETEndpoint(String("/"), onRoot);
    server->addOnNotFound(onNotFound);
    server->startServer();
    Serial.println("Server started.");
    delay(1000);

    /* Check that the server is set up and can be pinged. */
    Serial.println("Is server up?");
    IPAddress ip = network->getIPAddress();
    int response = server->pingServer(network->IpAddress2String(ip));
    Serial.print("Pinged the server and got result: ");
    Serial.println(response);
}

void loop() {}
