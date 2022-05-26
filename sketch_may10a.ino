#include <WiFi.h>

const char* ssid = "Hajra Shahzad";
const char* password = "lvox2471";
// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

//sensor vals
const int sensorPin = 34;
int moistureValue = 0;
float moisturePercent = 0;
const int airMoisture = 3620; //standard value for air moisture
const int waterMoisture = 1680;  
//Input
String plant = "Not Selected";
String instruction = "";
void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
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
            //logic
            if (header.indexOf("GET /est/cacti")>=0){
              plant = "cacti";
            }
            else if (header.indexOf("GET /est/palm")>=0){
              plant = "palm";
            }
            else if (header.indexOf("GET /est/pothos")>=0){
              plant = "pothos";
            }
            else if (header.indexOf("GET /est/ivy")>=0){
              plant = "ivy";
            }
            else if (header.indexOf("GET /fresh/cacti")>=0){
              plant = "freshcacti";
            }
            else if (header.indexOf("GET /fresh/palm")>=0){
              plant = "freshpalm";
            }
            else if (header.indexOf("GET /fresh/pothos")>=0){
              plant = "freshpothos";
            }
            else if (header.indexOf("GET /fresh/ivy")>=0){
              plant = "freshivy";
            }
             moistureValue = analogRead(sensorPin);
             moisturePercent = map(moistureValue,airMoisture, waterMoisture, 0, 100);
              Serial.println(moistureValue);
             Serial.println(moisturePercent);
          
            //end logic
            // HTML page 
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='device-width, intial-scale=1.0'>");
            client.println(" <link rel='preconnect' href=\"https://fonts.googleapis.com\">");
            client.println(" <link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>");
            client.println("<link href=\"https://fonts.googleapis.com/css2?family=Poppins&display=swap\" rel=\"stylesheet\"> ");
            client.println("<style>");
            client.println("*{font-family: 'Poppins', sans-serif;}");
            client.println("img{width: 20vw; aspect-ratio: 1; border-radius: 0.4rem;} .flexbox{display: flex; width: min-content;} .flexbox>div{padding: 0.5rem;} button{width: 20vw;margin-top: 0.5rem;padding: 0.5rem;background-color: rgb(21, 36, 5);color: white;border: none;border-radius: 1rem;}");
            client.println("button:hover{background-color: rgb(55, 94, 13); color: white;} h1 {align-content: center;align-items: center;text-align: center;} h4{font-weight: 200; text-align: justify;} h1, h2, h3, h4{margin: 5px;} .card{border-radius: 0.4rem; box-shadow: 4px 4px 2px rgb(141, 127, 127); border: solid 2px black; margin: 5px;} p{font-size: 0.7em; text-align: justify;}");
            client.println(".output p{font-size: 24px; font-weight: 400; color: rgb(175, 53, 53);}");
            client.println("</style>");
             Serial.println(plant);
            
             if(moisturePercent < 0){
              instruction = "Water your plants";
             }
             if(plant.equals("cacti")){
                if (moisturePercent >=0 && moisturePercent <=10){
                   instruction = "Water your plants";
                } 
                else if (moisturePercent > 10 && moisturePercent <= 50){
                  instruction = "Your cactus doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 50){
                  instruction = "Your cactus has enough water right now!";
                }
             }
             else if(plant.equals("palm")){
                 if (moisturePercent >=0 && moisturePercent <=30){
                  instruction = "Water your plants";
                } 
                else if (moisturePercent > 30 && moisturePercent <= 50){
                  instruction = "Your palm doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 50){
                    instruction = "Your palm has enough water right now!";
                }
             }
               else if(plant.equals("pothos")){
                 if (moisturePercent >=0 && moisturePercent <=25){
                  instruction = "Water your plants";
                } 
                else if (moisturePercent > 25 && moisturePercent <= 50){
                    instruction = "Your pothos doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 50){
                 instruction = "Your pothos has enough water right now!";
                }
             }
               else if(plant.equals("ivy")){
                 if (moisturePercent >=0 && moisturePercent <=50){
                   instruction = "Water your plants";
                } 
                else if (moisturePercent > 50){
                   instruction = "Your ivy has enough water right now!";
                }
             }
             //fresh
             else if(plant.equals("freshcacti")){
                if (moisturePercent >=0 && moisturePercent <=25){
                   instruction = "Water your plants";
                } 
                else if (moisturePercent > 25 && moisturePercent <= 50){
                  instruction = "Your cactus doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 50){
                  instruction = "Your cactus has enough water right now!";
                }
             }
             else if(plant.equals("freshpalm")){
                 if (moisturePercent >=0 && moisturePercent <=50){
                  instruction = "Water your plants";
                } 
                else if (moisturePercent > 50 && moisturePercent <= 60){
                  instruction = "Your palm doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 60){
                    instruction = "Your palm has enough water right now!";
                }
             }
               else if(plant.equals("freshpothos")){
                 if (moisturePercent >=0 && moisturePercent <=35){
                  instruction = "Water your plants";
                } 
                else if (moisturePercent > 35 && moisturePercent <= 60){
                    instruction = "Your pothos doesn't need water yet, give it 1-2 more days";
                }
                else if (moisturePercent > 60){
                 instruction = "Your pothos has enough water right now!";
                }
             }
               else if(plant.equals("freshivy")){
                 if (moisturePercent >=0 && moisturePercent <=50){
                   instruction = "Water your plants";
                } 
                else if (moisturePercent > 50){
                   instruction = "Your ivy has enough water right now!";
                }
             }
            //end logic
            client.println("</head><body>");
            client.println("<div class='top'><h1>CAO Semester Project</h1></div>");
            client.println("<h1>Welcome to your E-Garden</h1>");
             client.println("<div class = 'output'>");
            client.println("<p>");
            client.println(instruction);
            client.println("</p>");
             client.println("</div>");
            client.println("<h2>Select your plant type: </h2>");
            client.println(" <h3>Established Pot Plants</h3><h4>If your plant has been a inside its pot for more than six weeks, this is where you need to choose from. These plants are basically well established in their pots and their roots are used their enviornments, hence making them less sensitive to a good amount of water.</h4>");
            client.println(" <div class='flexbox'>");
            client.println("<div class='card'><div><img src='https://images.unsplash.com/photo-1554631221-f9603e6808be?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxzZWFyY2h8MXx8Y2FjdHVzJTIwcGxhbnR8ZW58MHx8MHx8&w=1000&q=80' alt=''></div><div><a href='/est/cacti'><button>Cacti</button></a></div>");
            client.println("<p>Cactus, a desert plant which stores excess water that it finds in the soil in its own body, hence requiring very less water.</p></div>");
            client.println("<div class='card'><div><img src='https://www.thespruce.com/thmb/NEKpRkMfWMZszMcMGn6MpKCLAH8=/3401x2721/filters:no_upscale():max_bytes(150000):strip_icc()/grow-areca-palms-indoors-1902876-vertical-a18bd0c06f8f4d39893f401e762f617e.jpg' alt=''></div><div><a href='/est/palm'><button>Palms</button></a></div>");
            client.println("<p>A palm is a tropical rainforest plant, and its roots are used to absorbing hefty amounts of water. Hence it needs more moisture than most indoor plants.</p></div>");
            client.println(" <div class='card'><div><img src='https://www.thespruce.com/thmb/nWrcag-Lpnjv6JW9tDy6WHS-zm8=/2667x2000/smart/filters:no_upscale()/neon-pothos-plant-profile-5206135-hero-c6b7295fe1aa472d8e5905e85d287a5b.jpg' alt=''></div><div><a href='/est/pothos'><button>Pothos</button></a></div>");
            client.println("<p>While pothos are not deemed as very sensitive plants, overwatering can damage them greatly by encouraging growth of fungus.</p></div>");
            client.println("<div class='card'><div><img src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcR1XCPY6F7HVTPhtzhcRbfFtaoZ5OqreFRy8WMlelri9uUkyULuMaK3q2qO2eUUQmVzqSI&usqp=CAU' alt=''></div><div><a href='/est/ivy'><button>Ivy</button></a></div>");
            client.println("<p>Ivies tend to grow long, and hence their roots require a lot water to remain healthy.</p></div></div>");
            client.println(" <h3>Freshly Propogated Plants</h3><h4>Freshly propogated plants are harder to take care of, since they have been cut off from their parent stems and need to develop new roots in a completely different enviornment. They require more water than esablished plants. </h4>");
            client.println(" <div class='flexbox'>");
            client.println(" <div class='card'><div><img src='https://i.pinimg.com/originals/04/a0/5d/04a05d545a1dff796d7ea5a867ea6f54.jpg' alt=''></div><div><a href='/fresh/cacti'><button>Cacti</button></a></div></div>");
            client.println(" <div class='card'><div><img src='https://www.epicgardening.com/wp-content/uploads/2017/04/Chamaedorea-elegans.jpg' alt=''></div><div><a href='/fresh/palm'><button>Palms</button></a></div></div>");
            client.println("<div class='card'><div><img src='https://content.instructables.com/ORIG/F2Y/BF0G/JHKTRT7I/F2YBF0GJHKTRT7I.jpg?auto=webp' alt=''></div><div><a href='/fresh/pothos'><button>Pothos</button></a></div></div>");
            client.println(" <div class='card'><div><img src='https://www.gardeningknowhow.com/wp-content/uploads/2013/04/propagate-ivy.jpg' alt=''></div><div><a href='/fresh/ivy'><button>Ivy</button></a></div></div></div>");
             
            client.println("</body></html>");
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
