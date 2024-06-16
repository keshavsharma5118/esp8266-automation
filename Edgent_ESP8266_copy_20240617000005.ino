/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
 *************************************************************
  Blynk.Edgent implements:
  - Blynk.Inject - Dynamic WiFi credentials provisioning
  - Blynk.Air    - Over The Air firmware updates
  - Device state indication using a physical LED
  - Credentials reset using a physical Button
 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
#define BLYNK_TEMPLATE_ID "TMPL3tvX2r5F1"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#define BLYNK_FIRMWARE_VERSION        "0.1.1"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"

// Define the GPIO connected with Relays and switches
#define RelayPin1 5  // D1
#define RelayPin2 4  // D2
#define RelayPin3 14 // D5
#define RelayPin4 12 // D6

#define SwitchPin1 0  // D3 
#define SwitchPin2 10 // SD3
#define SwitchPin3 13 // D7
#define SwitchPin4 3  // RX

#define wifiLed 16 // D0

#define VPIN_BUTTON_1 V2 
#define VPIN_BUTTON_2 V1
#define VPIN_BUTTON_3 V3 
#define VPIN_BUTTON_4 V4

int toggleState_1 = 1; // Define integer to remember the toggle state for relay 1
int toggleState_2 = 1; // Define integer to remember the toggle state for relay 2
int toggleState_3 = 1; // Define integer to remember the toggle state for relay 3
int toggleState_4 = 1; // Define integer to remember the toggle state for relay 4

int wifiFlag = 0;

BlynkTimer timer;

void relayOnOff(int relay) {
    switch (relay) {
        case 1: 
            toggleState_1 = !toggleState_1;
            digitalWrite(RelayPin1, toggleState_1);
            Serial.println(toggleState_1 ? "Device1 OFF" : "Device1 ON");
            delay(100);
            break;
        case 2: 
            toggleState_2 = !toggleState_2;
            digitalWrite(RelayPin2, toggleState_2);
            Serial.println(toggleState_2 ? "Device2 OFF" : "Device2 ON");
            delay(100);
            break;
        case 3: 
            toggleState_3 = !toggleState_3;
            digitalWrite(RelayPin3, toggleState_3);
            Serial.println(toggleState_3 ? "Device3 OFF" : "Device3 ON");
            delay(100);
            break;
        case 4: 
            toggleState_4 = !toggleState_4;
            digitalWrite(RelayPin4, toggleState_4);
            Serial.println(toggleState_4 ? "Device4 OFF" : "Device4 ON");
            delay(100);
            break;
        default: 
            break;      
    }
}

void with_internet() {
    // Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW) {
        delay(200);
        relayOnOff(1); 
        Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); // Update Button Widget  
    }
    else if (digitalRead(SwitchPin2) == LOW) {
        delay(200);
        relayOnOff(2);      
        Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); // Update Button Widget
    }
    else if (digitalRead(SwitchPin3) == LOW) {
        delay(200);
        relayOnOff(3);
        Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); // Update Button Widget
    }
    else if (digitalRead(SwitchPin4) == LOW) {
        delay(200);
        relayOnOff(4);
        Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); // Update Button Widget
    }
}

void without_internet() {
    // Manual Switch Control
    if (digitalRead(SwitchPin1) == LOW) {
        delay(200);
        relayOnOff(1);      
    }
    else if (digitalRead(SwitchPin2) == LOW) {
        delay(200);
        relayOnOff(2);
    }
    else if (digitalRead(SwitchPin3) == LOW) {
        delay(200);
        relayOnOff(3);
    }
    else if (digitalRead(SwitchPin4) == LOW) {
        delay(200);
        relayOnOff(4);
    }
}

BLYNK_CONNECTED() {
    // Request the latest state from the server
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
}

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1) {
    toggleState_1 = param.asInt();
    digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
    toggleState_2 = param.asInt();
    digitalWrite(RelayPin2, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
    toggleState_3 = param.asInt();
    digitalWrite(RelayPin3, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4) {
    toggleState_4 = param.asInt();
    digitalWrite(RelayPin4, toggleState_4);
}

void checkBlynkStatus() {
    bool isconnected = Blynk.connected();
    if (isconnected == false) {
        wifiFlag = 1;
        digitalWrite(wifiLed, HIGH); // Turn off WiFi LED
    }
    if (isconnected == true) {
        wifiFlag = 0;
        digitalWrite(wifiLed, LOW); // Turn on WiFi LED
    }
}

void setup() {
    Serial.begin(115200);
    delay(100);

    pinMode(RelayPin1, OUTPUT);
    pinMode(RelayPin2, OUTPUT);
    pinMode(RelayPin3, OUTPUT);
    pinMode(RelayPin4, OUTPUT);
    pinMode(wifiLed, OUTPUT);
    pinMode(SwitchPin1, INPUT_PULLUP);
    pinMode(SwitchPin2, INPUT_PULLUP);
    pinMode(SwitchPin3, INPUT_PULLUP);
    pinMode(SwitchPin4, INPUT_PULLUP);

    // During Starting all Relays should TURN OFF
    digitalWrite(RelayPin1, toggleState_1);
    digitalWrite(RelayPin2, toggleState_2);
    digitalWrite(RelayPin3, toggleState_3);
    digitalWrite(RelayPin4, toggleState_4);

    BlynkEdgent.begin();
    timer.setInterval(3000L, checkBlynkStatus); // check if Blynk server is connected every 3 seconds
}

void loop() {
    BlynkEdgent.run();
    timer.run(); // Initiates SimpleTimer

    if (wifiFlag == 0)
        with_internet();
    else
        without_internet();
}