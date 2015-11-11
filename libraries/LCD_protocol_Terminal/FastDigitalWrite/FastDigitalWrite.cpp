//
// FastDigitalWrite.cpp 
// Library C++ code
// ----------------------------------
// Developed with embedXcode 
// http://embedXcode.weebly.com
//
// Project SPI_23LC1024
//
// Created by Rei VILO, Jul 08, 2013
//	
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See FastDigitalWrite.h and ReadMe.txt for references
//

#if defined(__LM4F120H5QR__) || defined(__TM4C123GH6PM__) // LaunchPad Stellaris and Tiva specific)

// Library header
#include "FastDigitalWrite.h"

// Macros
#define portOutputRegister(x) (regtype)portBASERegister(x)
#define cbi_macro(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi_macro(reg, mask) GPIOPinWrite(reg, mask, mask)
#define gbi_macro(reg, mask) GPIOPinRead(reg, mask)
//#define pulse_high_macro(reg, bitmask) { sbi_macro(reg, bitmask); cbi_macro(reg, bitmask); }
//#define pulse_low_macro(reg, bitmask) { cbi_macro(reg, bitmask); sbi_macro(reg, bitmask); }


// Code
fastDigitalOutputPin::fastDigitalOutputPin()
{
    ;
}

void fastDigitalOutputPin::begin(uint8_t outputPin)
{
    _port = portOutputRegister(digitalPinToPort(outputPin));
    _bit  = digitalPinToBitMask(outputPin);
    pinMode(outputPin, OUTPUT);
}

void fastDigitalOutputPin::setHIGH()
{
    sbi_macro(_port, _bit);                                                           // HIGH
}

void fastDigitalOutputPin::setLOW()
{
    cbi_macro(_port, _bit);                                                           // LOW
}

bool fastDigitalOutputPin::get()
{
    return gbi_macro(_port, _bit);
}

void fastDigitalOutputPin::set(uint8_t value)
{
    if (value>0) setHIGH();
    else setLOW();
}

void fastDigitalOutputPin::pulseHIGH()
{
    setHIGH();
    setLOW();
}

void fastDigitalOutputPin::pulseLOW()
{
    setLOW();
    setHIGH();
}

#endif

