#include <Servo.h>
#include <LiquidCrystal.h>

// Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// State of the trash can
enum Lid {
  OPENED, // The trash lid is currently open
  CLOSED, // The trash lid is currently closed
};

Servo servo; // Creating a servo object
Lid lidState = CLOSED; // Initial state of the trash can is closed

// Set pins for the motion sensor 
const int trigPin = 9;
const int echoPin = 10;

// Used for recording data from the motion sensor
long duration;
int distance;

void setup() {
  
  // Initializing LCD
  lcd.begin(16,2); // These are the dimensions
  lcd.setCursor(3, 0);
  lcd.print("TURNING ON"); // Make this print to the LED screen
  
  // Initializing the servo
  servo.attach(13); // Using pin 13 for signals
  servo.write(0); // Sets the starting angle of the servo

  // Initializing the sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  // Setup the Trash Can
  delay(3000); // Wait 3 seconds before beginning
  lcd.setCursor(3, 0);
  lcd.print("               "); // Clear
  lcd.setCursor(2, 0);
  lcd.print("READY TO USE"); 
}

void loop() {

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Calculate the current state of the motion sensor
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;

  // See if the distance changed dirastically
  if(distance < 10){
    // The trash can lid needs to either be opened of closed

    if(lidState == CLOSED){
      // Open the lid and change the state
      Serial.println("OPEN THE LID!");
      servo.write(100); // Changes the servo angle
      //delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("                    "); // Clear
      lcd.setCursor(5, 0);
      lcd.print("OPENED");
      lidState = OPENED;
    }
    else if(lidState == OPENED){
      Serial.println("CLOSE THE LID!");
      servo.write(0); // Changes the servo angle
      //delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("                     "); // Clear
      lcd.setCursor(5, 0);
      lcd.print("CLOSED");
      lidState = CLOSED;
    }
    else{
      // This should never happen
      Serial.println("ERROR! The lid states are malfunctioning!");
    }
    
    delay(2000); // Gives time in between the open and close
  }
  
}
