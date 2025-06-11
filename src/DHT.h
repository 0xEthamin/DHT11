#ifndef DHT_SIMPLIFIED_H
#define DHT_SIMPLIFIED_H

#include "Arduino.h"
#include <stdint.h>

/**
 * @brief DHT11 temperature and humidity sensor driver
 * 
 * This class provides a simple interface to read temperature and humidity
 * from a DHT11 sensor. It handles all the low-level communication and
 * data conversion, providing easy access to the sensor's measurements.
 */
class DHT 
{
  public:
    /**
     * @brief Structure containing a complete sensor reading
     * 
     * This structure holds both temperature and humidity values from a single
     * sensor reading, along with a validity flag indicating if the reading
     * was successful.
     */
    struct Reading 
    {
      float temperature;     // Temperature in Celsius
      float humidity;        // Humidity in percentage
      bool isValid;         // Indicates if the reading is valid
    };
  
    /**
     * @brief Structure containing sensor specifications
     * 
     * This structure defines the operational parameters and limitations
     * of the DHT11 sensor, including measurement ranges and resolution.
     */
    struct Specs 
    {
      const char* name;     // Sensor name
      float temp_min;       // Minimum temperature (°C)
      float temp_max;       // Maximum temperature (°C)
      float temp_res;       // Temperature resolution (°C)
      float hum_min;        // Minimum humidity (%)
      float hum_max;        // Maximum humidity (%)
      float hum_res;        // Humidity resolution (%)
      uint32_t min_delay;   // Minimum delay between readings (ms)
    };
  
    explicit DHT(uint8_t pin);
    
    void begin();
    Reading read();
    const Specs& getSpecs() const { return SENSOR_SPECS; }
  
  private:
    static constexpr uint32_t MIN_INTERVAL = 2000;    // Minimum time between readings
    static constexpr uint32_t TIMEOUT = UINT32_MAX;   // Timeout value for pulse reading
    
    static constexpr Specs SENSOR_SPECS = 
    {
      "DHT11",    // name
      0.0F,       // temp_min
      50.0F,      // temp_max
      2.0F,       // temp_res
      20.0F,      // hum_min
      80.0F,      // hum_max
      5.0F,       // hum_res
      1000        // min_delay (ms)
    };
  
    uint8_t _pin;                 // GPIO pin number
    uint8_t _data[5];             // Raw data from sensor
    uint32_t _lastReadTime;       // Last reading timestamp
    bool _lastReadSuccess;        // Last reading status
  
    bool readRawData();
    uint32_t expectPulse(bool level);
    float computeTemperature() const;
    float computeHumidity() const;
};

#endif
