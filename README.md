# DHT11 Sensor Library

A minimalist Arduino library for the DHT11 temperature and humidity sensor. This library provides a clean and efficient interface to read temperature and humidity data from the DHT11 sensor.

## Features

- 🎯 Focused solely on DHT11 sensor support
- 💡 Simple and intuitive API
- ⚡ Efficient implementation
- 📊 Built-in sensor specifications
- ✅ Error checking and validation
- 📝 Comprehensive documentation

## Hardware Requirements

- Any Arduino-compatible board
- DHT11 temperature and humidity sensor
- 4.7kΩ - 10kΩ pull-up resistor (optional, internal pull-up is used)

## Installation

1. Download this repository
2. Copy the `DHT.h` and `DHT.cpp` files to your Arduino project
3. Include the library in your sketch: `#include "DHT.h"`

## Wiring

Connect your DHT11 sensor to your Arduino board:
- VCC → 5V
- DATA → Any digital pin (EX: Tiva PA_2)
- GND → GND

## Usage

Here's a basic example of how to use the library:

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

## API Reference

### Class `DHT`

Main class for interacting with the DHT11 sensor.

#### Methods

- `DHT(uint8_t pin)`: Constructor, initializes the sensor on the specified pin
- `void begin()`: Prepares the sensor for reading
- `Reading read()`: Reads temperature and humidity from the sensor
- `const Specs& getSpecs()`: Returns sensor specifications

#### Structures

```cpp
struct Reading {
  float temperature;     // Temperature in Celsius
  float humidity;        // Humidity in percentage
  bool isValid;         // Indicates if the reading is valid
};

struct Specs {
  const char* name;     // Sensor name
  float temp_min;       // Minimum temperature (°C)
  float temp_max;       // Maximum temperature (°C)
  float temp_res;       // Temperature resolution (°C)
  float hum_min;        // Minimum humidity (%)
  float hum_max;        // Maximum humidity (%)
  float hum_res;        // Humidity resolution (%)
  uint32_t min_delay;   // Minimum delay between readings (ms)
};
```

## Sensor Specifications

The DHT11 has the following specifications:
- Temperature Range: 0°C to 50°C ±2°C
- Humidity Range: 20% to 80% ±5%
- Sampling Rate: 1Hz (one reading every second)
- Resolution: Temperature 1°C, Humidity 1%

## Error Handling

The library provides error checking through the `isValid` flag in the `Reading` structure. A reading might be invalid due to:
- Communication errors
- Checksum validation failures
- Reading too frequently (faster than once per second)

## Contributing

Feel free to open issues or submit pull requests to improve this library.

## License

This library is released under the MIT License. See the LICENSE file for details.

## Author

0xEthamin

## Acknowledgments

This library is a simplified version optimized specifically for the DHT11 sensor. 