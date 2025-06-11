# DHT11 Sensor Library

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Arduino%20%7C%20Energia-blue)]()
[![Sensor](https://img.shields.io/badge/sensor-DHT11-green)]()

A lightweight and efficient C++ library to read temperature and humidity values from the DHT11 sensor, compatible with both Arduino and Tiva C (Energia).

---

## Overview

This library provides a minimal, robust implementation for interfacing with the DHT11 sensor. It handles timing constraints, data parsing, and error checking with a focus on simplicity and clarity. Ideal for embedded systems or academic projects that require precise and validated readings.

---

## Features

- Clean and minimal implementation
- Validated readings with checksum support
- Sensor specifications accessible via API
- Lightweight footprint (no dynamic memory allocation)
- Fully compatible with Energia (Tiva C) and Arduino IDE

---

## Hardware Compatibility

- **Microcontrollers**: Arduino, Tiva C TM4C123GXL (via Energia)
- **Sensor**: DHT11 (Digital Temperature and Humidity Sensor)

---

## Installation

1. Download or clone this repository.
2. Copy the files `DHT.h` and `DHT.cpp` into your Arduino or Energia project.
3. Include the library at the top of your sketch:

```cpp
#include "DHT.h"
```

---

## Wiring Guide

| DHT11 Pin | Function      | Connect to                          |
|-----------|---------------|-------------------------------------|
| 1 (VCC)   | Power Supply  | 5V                                  |
| 2 (DATA)  | Data Signal   | Any digital I/O (e.g. PA_2 on Tiva) |
| 4 (GND)   | Ground        | GND                                 |

---

## Example Usage

```cpp
#include "DHT.h"

#define DHTPIN PA_2

DHT dht(DHTPIN);

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  
  // Print sensor specifications
  const DHT::Specs& specs = dht.getSpecs();
  
  Serial.println("DHT11 Sensor Specifications");
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print("Range:        "); 
  Serial.print(specs.temp_min); Serial.print(" to "); 
  Serial.print(specs.temp_max); Serial.println(" *C");
  Serial.print("Resolution:   "); 
  Serial.print(specs.temp_res); Serial.println(" *C");
  
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print("Range:        "); 
  Serial.print(specs.hum_min); Serial.print(" to "); 
  Serial.print(specs.hum_max); Serial.println(" %");
  Serial.print("Resolution:   "); 
  Serial.print(specs.hum_res); Serial.println(" %");
  Serial.println("------------------------------------");
  
  Serial.print("Reading Interval: ");
  Serial.print(specs.min_delay);
  Serial.println(" ms");
  Serial.println("------------------------------------\n");
}

void loop() 
{
  DHT::Reading reading = dht.read();
  
  if (!reading.isValid) 
  {
    Serial.println("Failed to read from DHT sensor!");
  } 
  else 
  {
    Serial.print("Temperature: ");
    Serial.print(reading.temperature);
    Serial.println(" *C");
    
    Serial.print("Humidity: ");
    Serial.print(reading.humidity);
    Serial.println(" %");
  }
  
  delay(dht.getSpecs().min_delay);
}
```

---

## API Reference

### Class: `DHT`

| Method                    | Description                            |
|---------------------------|----------------------------------------|
| `DHT(uint8_t pin)`        | Constructor: specify data pin          |
| `void begin()`            | Initialize the sensor                  |
| `Reading read()`          | Returns current temperature & humidity |
| `const Specs& getSpecs()` | Returns sensor specification metadata  |

### Structs

#### `Reading`
```cpp
struct Reading {
  float temperature;
  float humidity;
  bool isValid;
};
```

#### `Specs`
```cpp
struct Specs {
  const char* name;
  float temp_min, temp_max, temp_res;
  float hum_min, hum_max, hum_res;
  uint32_t min_delay;
};
```
---

## Error Handling

Readings are validated with a checksum. If invalid, the `Reading::isValid` flag will be set to `false`. This may occur due to:
- Electrical noise or bad wiring
- Sensor read too frequently (<1s interval)
- Incomplete or corrupted data signal

---

## Limitations

- Only supports **DHT11** (not DHT22 or AM2302)

---

## Tested Platforms

- Energia 1.8 with Tiva C TM4C123GXL

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Author

**0xEthamin**  
Feel free to open issues or contribute improvements.
