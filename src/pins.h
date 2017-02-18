//
// Created by Mike Mitterer on 18.02.17.
//

#ifndef ESP8266_TEST1_PINS_H
#define ESP8266_TEST1_PINS_H

static const uint8_t IOPIN_D0   = 16;
static const uint8_t IOPIN_D1   = 5;
static const uint8_t IOPIN_D2   = 4;

//static const uint8_t IOPIN_D3   = 0;

static const uint8_t IOPIN_D4   = 2;
//static const uint8_t IOPIN_D5   = 14;
//static const uint8_t IOPIN_D6   = 12;
//static const uint8_t IOPIN_D7   = 13;
//static const uint8_t IOPIN_D8   = 15;
//static const uint8_t IOPIN_D9   = 3;
//static const uint8_t IOPIN_D10  = 1;

const uint8_t pinButton = IOPIN_D0;
const uint8_t pinNMosfet = IOPIN_D1;
const uint8_t pinMainSwitchPMosfet = IOPIN_D2;
const uint8_t pinLed = IOPIN_D4;

#endif //ESP8266_TEST1_PINS_H
