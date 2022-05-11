#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Hostels Wi-Fi";
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
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
            
            // HTML page 
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='device-width, intial-scale=1.0'>");
            client.println(" <style>");
            client.println("*{font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;}");
            client.println("img{width: 20vw; aspect-ratio: 1; border-radius: 0.4rem;}");
            client.println(".flexbox{display: flex;} .flexbox>div{padding: 0.5rem;} button{width: 20vw;margin-top: 0.5rem;padding: 0.5rem;background-color: rgb(21, 36, 5);color: white;border: none;border-radius: 1rem;}h1, h2, h3{align-content: center;align-items: center;text-align: center;}");
            client.println("</style>");
            client.println("</head><body>");
            client.println("<div class='top'><h1>CAO Semester Project</h1></div>");
            client.println("<h1>Welcome to your E-Garden</h1><h2>Select your plant type: </h2>");
            client.println(" <div class='flexbox'>");
            client.println("<div><div><img src='https://images.unsplash.com/photo-1554631221-f9603e6808be?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxzZWFyY2h8MXx8Y2FjdHVzJTIwcGxhbnR8ZW58MHx8MHx8&w=1000&q=80' alt=''></div><div><a href='/cacti'><button>Cacti</button></a></div></div>");
            client.println(" <div><div><img src='https://www.thespruce.com/thmb/NEKpRkMfWMZszMcMGn6MpKCLAH8=/3401x2721/filters:no_upscale():max_bytes(150000):strip_icc()/grow-areca-palms-indoors-1902876-vertical-a18bd0c06f8f4d39893f401e762f617e.jpg' alt=''></div><div><a href='/palm'><button>Palms</button></a></div></div>");
            client.println(" <div><div><img src='https://www.thespruce.com/thmb/nWrcag-Lpnjv6JW9tDy6WHS-zm8=/2667x2000/smart/filters:no_upscale()/neon-pothos-plant-profile-5206135-hero-c6b7295fe1aa472d8e5905e85d287a5b.jpg' alt=''></div><div><a href='/pothos'><button>Pothos</button></a></div></div>");
            client.println("<div><div><img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR1XCPY6F7HVTPhtzhcRbfFtaoZ5OqreFRy8WMlelri9uUkyULuMaK3q2qO2eUUQmVzqSI&usqp=CAU' alt=''></div><div><a href='/ivy'><button>Ivy</button></a></div></div></div>");
            client.println(" </body></html>");
            // The HTTP response ends with another blank line
            client.println();
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
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
