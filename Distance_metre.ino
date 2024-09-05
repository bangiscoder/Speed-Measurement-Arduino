#include <LiquidCrystal.h>

#define TRIG_PIN 9
#define ECHO_PIN 10

// Initialize the LCD with the appropriate pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String names[3] = {"ABRAHAM FAVOUR", "AYISA AONDOFA"}; //Replace with the actual names
String regNum[3] = {"20/137509", "20/138814"}; //Replace with the actual registrantion numbers

// Variables to store distance and time measurements
float distance1, distance2, speed;
long duration1;

void introduction(){
  lcd.clear(); //clear the LCD
  
  //Print an intro text
  lcd.print("Fed Poly Bauchi");
  lcd.setCursor(0, 1);
  lcd.print("MTET 2024 BY");
  delay(1500); //Wait for 1.5 Second
  lcd.clear(); //Clear the LCD

  //Display the names and registration number of undertaking studings
   for (int i = 0; i <= 2; i++){
    lcd.setCursor(0, 0);
    lcd.print(names[i]);
    
    lcd.setCursor(0, 1);
    lcd.print(regNum[i]);
    delay(3000); // Wait for 3000 millisecond(s)   
   }
}

void setup() {
  Serial.begin(9600); // Start serial communication
  lcd.begin(16, 2); // Initialize the LCD
  lcd.print("Speed Measurement"); // Initial message
  delay(1000);

  introduction(); //A function that introduce the system and the undertaking students
  
  // Set up the HC-SR04 sensor pins
  pinMode(TRIG_PIN, OUTPUT); // Set the trigger pin as output
  pinMode(ECHO_PIN, INPUT); // Set the echo pin as input
}

void loop() {
  
  // Measure the first distance
  distance1 = measureDistance(); 
  delay(1000); // Wait for 1000 milisecond

  // Measure the second distance
  distance2 = measureDistance();
  
  //Check whether any of the distances received and error code
  if (distance1 == -1 || distance2 == -1){
    lcd.clear(); //clear the LCD
  
    //Print Error code and Message
    lcd.print("Error: -1");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Not responding");
    delay(1000); //Wait for 1 Second
    return;
  } else if(distance1 == -2 || distance2 == -2){
    lcd.clear(); //clear the LCD
  
    //Print Error code and Message
    lcd.print("Error: -2");
    lcd.setCursor(0, 1);
    lcd.print("Obj out of range");
    delay(1000); //Wait for 1 Second
    return;
  }

  // Calculate speed (in cm/s)
  speed = (distance2 - distance1) / 1.0; //Time interval is 1 second
  
  if(speed == 0){
    lcd.clear();
    lcd.setCursor(0, 0); // Set cursor to the first line
    lcd.print("No Movement");
    
    // Display the distance and speed on the LCD
    lcd.setCursor(0, 1); // Set cursor to the second line
    lcd.print("Dist: ");
    lcd.print(distance2);
    lcd.print(" cm  ");
    return;
  } else if(distance2 < distance1){
   speed = -speed; // Remove negative sign
 }

  // Display the distance and speed on the LCD
  lcd.setCursor(0, 1); // Set cursor to the second line
  lcd.print("Dist: ");
  lcd.print(distance2);
  lcd.print(" cm  ");
  
  lcd.setCursor(0, 0); // Set cursor to the first line
  lcd.print("Speed: ");
  lcd.print(speed);
  lcd.print(" cm/s");

  // Print to Serial Monitor for debugging
  Serial.print("Distance: ");
  Serial.print(distance2);
  Serial.print(" cm, Speed: ");
  Serial.print(speed);
  Serial.println(" cm/s");

  delay(1000); // Wait for 1000 milisecond before the next measurement
}

long measureDistance() {
  long duration, distance;

  // Clear the trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Set the trigger to HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin, and get the duration of the pulse
  duration = pulseIn(ECHO_PIN, HIGH);
  
  //an IF condition to detect sensor failure or out of range sensing
  if (duration == 0){
    // No echo received, sensor may be faulty
    return -1; // the error code to return
    
  } else if(duration > 30000){
    // Echo duration is too long, indicating no object detected
    return -2; // Error code to return
  }
  
  // Calculate the distance in metres
  distance = (duration / 2) * 0.0343; // Divide by 2 because the pulse travels to the object and back and convert it to Metres
  

  return distance;
}
