#include <Servo.h>
#include <DHT.h>

Servo motor;
char sensorpin = A0;              // set the analog input pin to read the rain sensor output
int motorpin = 5;                 // set the digital output pin to control the motor
int val;                          // declare a variable to store the rain sensor output
int dhtpin = 2;                   // define the digital pin for the DHT11 sensor
int fogthreshold = 90;            // set the threshold humidity level for detecting fog

DHT dht(dhtpin, DHT11);           // create a DHT11 object named "dht"

void setup() {
  pinMode(sensorpin, OUTPUT);     // set the rain sensorpin as an output
  digitalWrite(sensorpin, LOW);   // set the rain sensorpin to low
  Serial.begin(9600);             // initialize serial communication
  pinMode(sensorpin, INPUT);      // set the rain sensorpin as an input
  motor.attach(motorpin);         // attach the motor to the motorpin
  motor.write(180);               // set the initial position of the motor to 180 degrees
  dht.begin();                    // initialize the DHT11 sensor on the specified digital pin
}

void loop() {
  val = readSensor();             // read the rain sensor output and store it in the "val" variable
  float temperature = dht.readTemperature();  // read the temperature from the DHT11 sensor
  float humidity = dht.readHumidity();  // read the humidity value from the DHT11 sensor
  if (isnan(humidity)) {                // check if the humidity reading is invalid
    return;                             // if so, exit the loop
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  
  if (val) {                      // if the rain sensor output is high (it's not raining)
    Serial.println("Status: The weather is clear");  // print "Status: Clear" to the serial monitor
  } 
  else {                          // if the rain sensor output is low (it's raining)
    Serial.println("Status: It's raining");  // print "Status: It's raining" to the serial monitor
    for (int i = 0; i < 5; i++) { // start a for loop that runs 5 times
      motor.write(45);            // set the motor position to 45 degrees
      delay(500);                 // wait for 500 milliseconds
      motor.write(120);            // set the motor position to 90 degrees
      delay(500);                 // wait for 500 milliseconds
    }
  }
  
  if (humidity < fogthreshold) { // if the humidity value is below the fog threshold
    Serial.println("Status: It's not foggy");
  }
  else {  // check if the humidity value is above the fog threshold
    Serial.println("Status: It's foggy");
    for (int i = 0; i < 5; i++) { // start a for loop that runs 5 times
      motor.write(45);            // set the motor position to 45 degrees
      delay(500);                 // wait for 500 milliseconds
      motor.write(120);            // set the motor position to 90 degrees
      delay(500);                 // wait for 500 milliseconds
    } 
  }
  delay(1000);                    // wait for 1000 milliseconds before repeating the loop
  Serial.println();               // print a blank line to the serial monitor
}

int readSensor() {
  digitalWrite(sensorpin, HIGH);  // turn on the rain sensor
  delay(10);                      // wait for the rain sensor to settle
  int val = digitalRead(sensorpin); // read the rain sensor output and store it in the "val" variable
  digitalWrite(sensorpin, LOW);   // turn off the rain sensor
  return val;                     // return the rain sensor output
}