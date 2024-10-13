//
// Created by Alex on 12.10.2024.
//

#include "network.h"
#include "config.h"
#include "led.h"
#include <ESP8266WiFi.h>

WiFiServer server(80);

const char *apSsid = "ESP8266-Access-Point";
const char *apPassword = "123456789";
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
String header;


void configure() {
    WiFi.softAP(apSsid, apPassword);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    Serial.println("Waiting for connection ...");
    server.begin();
    bool configuring = true;
    while (configuring) {
        WiFiClient client = server.accept();   // Listen for incoming clients
        if (client) {                             // If a new client connects,
            currentTime = millis();
            previousTime = currentTime;
            String currentLine = "";                // make a String to hold incoming data from the client
            while (client.connected() &&
                   currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
                currentTime = millis();
                if (client.available()) {             // if there's bytes to read from the client,
                    char c = client.read();             // read a byte, then
                    header += c;
                    if (c == '\n') {                    // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0) {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            // Display the HTML web page
                            client.println("<!DOCTYPE html>");
                            client.println("<html lang=\"de\">");
                            client.println("<head>");
                            client.println(
                                    "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
                            client.println("<title>Wifi Config</title>");
                            client.println("<style>from {display:flex; flex-direction: column; gap:8px;}</style>");
                            client.println("</head>");
                            client.println("<body>");
                            client.println("<form method=\"post\">");
                            client.println(R"(<div>SSID: <input type="text" placeholder="SSID" name="ssid" /></div>)");
                            client.println(
                                    R"(<div>Password: <input type="text" placeholder="Password" name="pw" /></div>)");
                            client.println(
                                    R"(<div>Hostname: <input type="text" placeholder="Hostname" name="hostname" /></div>)");
                            client.println(R"(<button type="submit">Save</button>)");
                            client.println("</form>");
                            client.println("</body>");
                            client.println("</html>");
                            client.println();

                            if (header.indexOf("POST") >= 0) {
                                String body = "";
                                char bodyChar = ' ';

                                // Read the request body from the client
                                while (client.available() &&
                                       bodyChar != '\n') { // if there's bytes to read from the client,
                                    bodyChar = client.read();
                                    body += bodyChar; // Append the character to the body string
                                }

                                // Parse the indices for each parameter
                                int idxSsid = body.indexOf("ssid=") + 5; // Find the start of the SSID value
                                int idxPw = body.indexOf("pw=") + 3;     // Find the start of the password value
                                int idxHost = body.indexOf("hostname=") + 9; // Find the start of the hostname value

                                // Ensure indices are valid and extract the values
                                String ssid = (idxSsid > 4 && idxPw > 3) ? body.substring(idxSsid,
                                                                                          body.indexOf('&', idxSsid))
                                                                         : "";
                                String pw = (idxPw > 3 && idxHost > 9) ? body.substring(idxPw, body.indexOf('&', idxPw))
                                                                       : "";
                                String host = (idxHost > 9) ? body.substring(idxHost, body.length()) : "";
                                ssid.replace("+", " ");
                                pw.replace("+", " ");
                                host.replace("+", " ");
                                // Print the extracted values
                                Serial.print("SSID: ");
                                Serial.println(ssid);
                                Serial.print("PW: ");
                                Serial.println(pw);
                                Serial.print("Hostname: ");
                                Serial.println(host);

                                config::setSsid(ssid);
                                config::setPassword(pw);
                                config::setHostname(host);

                                configuring = false;
                            }
                            // Break out of the while loop
                            break;
                        } else { // if you got a newline, then clear currentLine
                            currentLine = "";
                        }
                    } else if (c != '\r') {  // if you got anything else but a carriage return character,
                        currentLine += c;      // add it to the end of the currentLine
                    }
                }
            }
            // Clear the header variable
            header = "";
            // Close the connection
            client.stop();
        }
    }
    WiFi.disconnect();
}


void network::checkWebServer() {
    WiFiClient client = server.accept();   // Listen for incoming clients
    if (client) {                             // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected() &&
               currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
            currentTime = millis();
            if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                header += c;
                if (c == '\n') {                    // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        // Display the HTML web page
                        client.println("<!DOCTYPE html>");
                        client.println("<html lang=\"de\">");
                        client.println("<head>");
                        client.println("    <meta charset=\"UTF-8\">");
                        client.println(
                                R"(    <meta name="viewport" content="width=device-width, initial-scale=1.0">)");
                        client.println("    <title>Light control</title>");
                        client.println("    <style>");
                        client.println(
                                "        body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }");
                        client.println("        input[type=\"color\"] { width: 100px; height: 100px; border: none; }");
                        client.println("        button { padding: 10px 20px; margin-top: 10px; cursor: pointer; }");
                        client.println("    </style>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("    <h1>I feel like ...</h1>");
                        client.println(
                                R"(    <input type="color" id="color" value="#FFFFFF" onchange="const c = color.value.slice(1); fetch(`/?r${parseInt(c.slice(0,2),16)}g${parseInt(c.slice(2,4),16)}b${parseInt(c.slice(4),16)}`, { method: 'GET' }))");
                        client.println(
                                "            .then(res => res.ok ? console.log('Color changed!') : alert('Error sending'))");
                        client.println("            .catch(err => alert(err));\">");
                        client.println("    <br>");
                        client.println("</body>");
                        client.println("</html>");
                        client.println();

                        if (header.indexOf("GET /?r") >= 0) {
                            int pos1 = header.indexOf('r');
                            int pos2 = header.indexOf('g');
                            int pos3 = header.indexOf('b');
                            int pos4 = header.indexOf('&');
                            int targetRed = header.substring(pos1 + 1, pos2).toInt();
                            int targetGreen = header.substring(pos2 + 1, pos3).toInt();
                            int targetBlue = header.substring(pos3 + 1, pos4).toInt();
                            led::setColor(targetRed, targetGreen, targetBlue, 1000);
                        }
                        if (header.indexOf("GET /reset") >= 0) {
                            config::clearWifiConfig();
                        }
                        // Break out of the while loop
                        break;
                    } else { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
    }
}

void network::connect() {
    led::setColorBlocking(0, 0, 0, 1000);
    do {
        if (!config::hasWifiConfig()) {
            led::setColorBlocking(255, 0, 0, 1000);
            configure();
        }
        unsigned long start = millis();
        WiFi.begin(config::getSsid(), config::getPassword());
        while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
            delay(500);
        }
        if (WiFi.status() != WL_CONNECTED) {
            config::clearWifiConfig();
        }
    } while (WiFi.status() != WL_CONNECTED);
    WiFi.begin();
    server.begin();
    WiFi.hostname(config::getHostname());
    Serial.println(WiFi.localIP());
    led::setColorBlocking(0, 255, 0, 1000);
    led::setColorBlocking(0, 0, 0, 1000);
}



