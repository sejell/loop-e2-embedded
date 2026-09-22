#include <Arduino.h>
#include <ESP32Servo.h>

int redLED = 18;
int greenLED = 17; // define the pin corresponding to LED name
int blueLED = 16;

const int yellowLED = 19;
const int buzzerPin = 25;
const int servoPin = 27;

Servo myServo;

int servoPosition = 0;
int servoDirection = 1;

unsigned long previousServoTime = 0;

bool buzzerOn = false;

unsigned long buzzerStartTime = 0;
unsigned long buzzerDuration = 0;

void setup() {

  Serial.begin(115200); // starts the serial communication once between compute
                        // and ESP32

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT); // GPIO pins can only send electrical signals
  pinMode(blueLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Turn all LEDs off at the start
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(buzzerPin, LOW);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  myServo.setPeriodHertz(50);
  myServo.attach(servoPin, 500, 2500);

  myServo.write(0);
}

void loop() {
  if (Serial.available() > 0) {

    String input = Serial.readStringUntil('\n');
    input.trim() if (input.startsWith("#")) {

      String hex = input;

      // Remove # from the HEX value
      if (hex.startsWith("#")) {
        hex.remove(0, 1); // removes one character starting at position 0
      }

      // Convert HEX into RGB
      long color = strtol(hex.c_str(),
                          NULL 16); // converts HEX to a number ESP32 can read

      int red = (color >> 16) & 255;
      int green = (color >> 8) & 255; // move to the end, keep last 8 bits
      int blue = color & 255;

      // Calculate distance from red
      int redDistance = (255 - red) + green + blue;
      // Calculate distance from green
      int greenDistance = (255 - green) + red + blue;
      // Calculate distance from blue
      int blueDistance = (255 - blue) + green + red;
      // Turn all LEDs off
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
      digitalWrite(blueLED, LOW);
      // Turn on the closest color
      if (redDistance <= greenDistance && redDistance <= blueDistance) {

        digitalWrite(redLED, HIGH);
        // <= because two can be the same
      } else if (greenDistance <= redDistance &&
                 greenDistance <= blueDistance) {
        digitalWrite(greenLED, HIGH);
      } else {
        digitalWrite(blueLED, HIGH);
      }

      Serial.print("HEX received: ");
      Serial.println(input);

      else if (input.startsWith("Y")) {

        // Remove the Y
        String number = input.substring(1);

        // Convert the input to an integer
        int value = number.toInt();

        // Check that it is within the allowed range
        if (value >= 0 && value <= 255) {

          // Set LED brightness using PWM
          analogWrite(yellowLED, value);

          Serial.print("Yellow LED brightness: ");
          Serial.println(value)

        }

        else if (input.startsWith("B")) {

          // Remove the B
          String number = input.substring(1);

          // Convert the input to an integer
          int duration = number.toInt();

          if (duration > 0) {

            Serial.print("Buzzing for ");
            Serial.print(duration);
            Serial.println(" ms");

            digitalWrite(buzzerPin, HIGH);

            buzzerOn = true;
            buzzerStartTime = millis();
            buzzerDuration = duration
          }
        }
      }

      unsigned long currentTime = millis();

      // Move the servo every 20 milliseconds
      if (currentTime - previousServoTime >= 20) {

        previousServoTime = currentTime;

        servoPosition += servoDirection;

        // Reached 90 degrees
        if (servoPosition >= 90) {

          servoPosition = 90;
          servoDirection = -1;
        }

        // Reached 0 degrees
        if (servoPosition <= 0) {

          servoPosition = 0;
          servoDirection = 1;
        }

        myServo.write(servoPosition);
      }

      if (buzzerOn) {

        if (millis() - buzzerStartTime >= buzzerDuration) {

          digitalWrite(buzzerPin, LOW);

          buzzerOn = false
        }
      }
    }
  }
}
