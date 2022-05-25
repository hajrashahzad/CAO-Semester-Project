const int sensorPin = 15;
int moistureValue =0;
float moisturePercent = 0;
const int airMoisture = 3620; //standard value for air moisture
const int waterMoisture = 1680;  
String plant = "cactus";
void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
   moistureValue = analogRead(sensorPin);
   moisturePercent = map(moistureValue,airMoisture, waterMoisture, 0, 100);
  // Serial.println(moisturePercent);
  // delay(500);
   if(moisturePercent < 0){
    Serial.println("water your plants");
    delay(500);
   }
   if(plant.equals("cactus")){
      if (moisturePercent >=0 && moisturePercent <=30){
         Serial.println("water your plants");
         delay(100);
      } 
      else if (moisturePercent > 30 && moisturePercent <= 50){
         Serial.println("Your cacti dont need water yet, give it 1-2 more days");
         delay(100);
      }
      else if (moisturePercent > 50){
         Serial.println("Your cacti have enough water right now!");
         delay(100);
      }
   }
   
}
