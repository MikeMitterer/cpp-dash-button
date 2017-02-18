#include <Arduino.h>
#include "pins.h"
#include "wifi.h"


int buttonState = LOW;
bool isButtonPressed = false;
bool prevButtonState = isButtonPressed;

bool toggleRelay = false;

enum class States {
    Idle,
    ButtonPressed,
    LedOn, LedOff,
    RelayOn, RelayOff,
    MainSwitchOn, MainSwitchOFF,
    HTTPRequest
};

/// Hauptstatus
States currentState = States::MainSwitchOn;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
void setup() {
    // Power - ON über den Mosfet!

    // NodeMCU schaltet sich selbst ein
    // P-Channel LOW - Source -> Drain  (ON)
    // N-Channel HIGH - Source -> Drain (ON)
    pinMode(pinMainSwitchPMosfet, OUTPUT);
    digitalWrite(pinMainSwitchPMosfet, LOW);

    // Setup Pins
    pinMode(pinButton, INPUT);
    pinMode(pinNMosfet, OUTPUT);
    pinMode(pinLed, OUTPUT);

    digitalWrite(pinNMosfet, LOW);
    digitalWrite(pinLed, LOW);

    // Setup Serial
    Serial.begin(115200);
    delay(10);

    // Setup WIFI
    setupWIFI(ssid, password);
}
#pragma clang diagnostic pop

int switchCounter = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
void loop() {
    buttonState = digitalRead(pinButton) == HIGH;

    // debounce Button
    delay(30);
    if(digitalRead(pinButton) == buttonState) {
        isButtonPressed = buttonState == HIGH;
    }

    // Button changed
    if(prevButtonState != isButtonPressed) {
        prevButtonState = isButtonPressed;

        currentState = isButtonPressed ? States::ButtonPressed : States::LedOff;
    }

    switch(currentState){
        case States::ButtonPressed:
            currentState = States::LedOn;
            break;

        case States::LedOn:
            digitalWrite(pinLed, HIGH);  // Turn the LED on by making the voltage HIGH

            toggleRelay = !toggleRelay;
            currentState = toggleRelay ? States::RelayOn : States::RelayOff;

            switchCounter++;
            if(switchCounter > 5) {
                currentState = States::MainSwitchOFF;
            } 

            break;

        case States::LedOff:
            digitalWrite(pinLed, LOW);  // Turn the LED off by making the voltage LOW
            currentState = States::MainSwitchOn;
            break;

        case States::RelayOn:
            // Serial.println("State: RelayOn");

            //digitalWrite(pinMainSwitchPMosfet, HIGH);
            digitalWrite(pinNMosfet, HIGH);
            currentState = States::HTTPRequest;
            break;

        case States::RelayOff:
            //digitalWrite(pinMainSwitchPMosfet, LOW);
            digitalWrite(pinNMosfet, LOW);
            currentState = States::Idle;
            break;

        case States::MainSwitchOn:
            digitalWrite(pinMainSwitchPMosfet, LOW);
            currentState = States::Idle;
            break;

        case States::MainSwitchOFF:
            digitalWrite(pinMainSwitchPMosfet, HIGH);
            switchCounter = 0;
            currentState = States::Idle;
            break;

        case States::HTTPRequest:
            sendGetRequest(host,url,port);
            currentState = States::Idle;
            break;

        case States::Idle :
            yield();
    }

    yield();
}

#pragma clang diagnostic pop