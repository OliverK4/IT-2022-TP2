#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   

//HTML
String pagina = 
"<!DOCTYPE html>"
"<head>"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<title>Arduino LED</title>"
"<style>"
"html {"
"box-sizing: border-box;"
"font-size: 62.5%; /* 1 rem = 10px */ }"

"h1"
"{font-size: 5rem;"
"margin: 0;"
"font-family: Arial, Helvetica, sans-serif;"
"text-decoration-line: underline;}"

"body"
"{font-size: 10rem;"
"line-height: 2.15;}"
    
"div{"
"width:100%;"
"text-align: center;}"

"button {"
"background-color:#201b69;"
"color: rgb(214, 9, 255);"
"border:none;"
"border-radius:50px;"
"min-width: 150px;"
"width: 100px;"
"border: solid;"
"font-size: 2rem;}"

"@media(max-width: 780px)"
"{h1{"
"font-size: 9rem;}"

"button"
"{height: 80px;"
"font-size: 4rem;"
"font-family: Arial, Helvetica, sans-serif;"
"margin: 15px;"
"font-weight: bold;}}"
"</style>"
"</head>"
"<body>"
"<div>"
"<h1>Arduino LED</h1>"
"<nav>"
"<a href=\"/H\"><button >ON</button></a>"
"<a href=\"/L\"><button >OFF</button></a>"
"</nav>"
"</div>"      
"</body>"
"</html>";
//


const char *ssid = "Oliver";
const char *password = "123456789";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print(pagina);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}