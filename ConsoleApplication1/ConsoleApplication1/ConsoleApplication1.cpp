#include <Bounce2.h>

const char* eventDoorOpened = "{ \"event\": \"door_opened\" }";
const char* eventDoorClosed = "{ \"event\": \"door_closed\" }";

const int inPin = D1;
const int buzzerPin = D2;  // Define the pin connected to the buzzer

void setup()
{
    pinMode(inPin, INPUT);
    pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output
    digitalWrite(buzzerPin, LOW); // Ensure the buzzer is off initially
    Serial.begin(115200); // Initialize serial communication at 115200 baud
}

Debounce doorSwitch(inPin); // the number of the input pin, D1 is set for the ESP8266.
int previousState = -1;
unsigned long buzzerStartTime = 0;
bool buzzerActive = false;

void loop()
{
    int newState = doorSwitch.read();
    if (previousState == -1) {
        // On the first iteration, just set it to the latest value and don't trigger an event.
        previousState = newState;
    }
    if (newState != previousState) {
        if (newState == HIGH) {
            Serial.println("Door was closed");
            postEvent(eventDoorClosed);
            stopBuzzer();
        }
        else {
            Serial.println("Door was opened");
            postEvent(eventDoorOpened);
            startBuzzer();
        }
    }

    if (buzzerActive && (millis() - buzzerStartTime >= 30000)) {
        stopBuzzer();
    }

    previousState = newState;
}

void postEvent(const char* event)
{
    Serial.println("Sending event...");
    Serial.println(event); // Send the event string to the connected device
}

void startBuzzer()
{
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    buzzerStartTime = millis(); // Record the start time
    buzzerActive = true;
}

void stopBuzzer()
{
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    buzzerActive = false;
}
