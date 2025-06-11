#include "DHT.h"

#define MIN_INTERVAL 2000
#define TIMEOUT UINT32_MAX

/**
 * @brief Initializes a new DHT11 sensor instance
 * @param pin The GPIO pin number where the sensor is connected
 */
DHT::DHT(uint8_t pin) : _pin(pin), _lastReadTime(0), _lastReadSuccess(false) 
{
  _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;
}

/**
 * @brief Prepares the sensor for reading by setting up the GPIO pin
 */
void DHT::begin() 
{
  pinMode(_pin, INPUT_PULLUP);
  _lastReadTime = millis() - MIN_INTERVAL;
}

/**
 * @brief Reads temperature and humidity from the sensor
 * @return Reading structure containing temperature (°C), humidity (%) and validity flag
 * @note If less than MIN_INTERVAL has passed since last read, returns cached values
 */
DHT::Reading DHT::read() 
{
  Reading result = {NAN, NAN, false};
  
  // Check if enough time has passed since last reading
  uint32_t currentTime = millis();
  if ((currentTime - _lastReadTime) < MIN_INTERVAL) 
  {
    result.isValid = _lastReadSuccess;
    if (result.isValid) 
    {
      result.temperature = computeTemperature();
      result.humidity = computeHumidity();
    }
    return result;
  }
  
  _lastReadTime = currentTime;
  
  if (readRawData()) 
  {
    result.temperature = computeTemperature();
    result.humidity = computeHumidity();
    result.isValid = true;
  }
  
  return result;
}

/**
 * @brief Performs the low-level communication with the DHT11 sensor
 * @return true if data was successfully read and checksum is valid, false otherwise
 * @note This function temporarily disables interrupts during critical timing sections
 */
bool DHT::readRawData() 
{
  // Reset data array
  _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;

  // Send start signal
  pinMode(_pin, INPUT_PULLUP);
  delay(1);
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);  // DHT11 start signal

  // Switch to input mode and read response
  uint32_t cycles[80];
  pinMode(_pin, INPUT_PULLUP);
  delayMicroseconds(55);

  // Disable interrupts during timing-critical section
  noInterrupts();
  
  // Read initial response
  if (expectPulse(LOW) == TIMEOUT || expectPulse(HIGH) == TIMEOUT) 
  {
    interrupts();
    _lastReadSuccess = false;
    return false;
  }

  // Read 80 pulses (40 bits of data)
  for (int i = 0; i < 80; i += 2) 
  {
    cycles[i] = expectPulse(LOW);
    cycles[i + 1] = expectPulse(HIGH);
  }
  
  interrupts();

  // Decode 40 bits of data
  for (int i = 0; i < 40; ++i) 
  {
    uint32_t lowCycles = cycles[2 * i];
    uint32_t highCycles = cycles[2 * i + 1];
    
    if (lowCycles == TIMEOUT || highCycles == TIMEOUT) 
    {
      _lastReadSuccess = false;
      return false;
    }
    
    _data[i / 8] <<= 1;
    if (highCycles > lowCycles) 
    {
      _data[i / 8] |= 1;
    }
  }

  // Verify checksum
  uint8_t checksum = _data[0] + _data[1] + _data[2] + _data[3];
  _lastReadSuccess = (_data[4] == checksum);
  return _lastReadSuccess;
}

/**
 * @brief Measures the duration of a pulse at a specific logic level
 * @param level The logic level to measure (HIGH or LOW)
 * @return Number of cycles the pulse lasted, or TIMEOUT if no pulse detected
 * @note This is a timing-critical function used by readRawData()
 */
uint32_t DHT::expectPulse(bool level) 
{
  uint32_t count = 0;
  uint32_t maxCycles = microsecondsToClockCycles(1000);
  
  while (digitalRead(_pin) == level) 
  {
    if (count++ >= maxCycles) 
    {
      return TIMEOUT;
    }
  }
  return count;
}

/**
 * @brief Converts raw sensor data to temperature in Celsius
 * @return Temperature in degrees Celsius
 * @note For DHT11: range 0-50°C, resolution 2°C
 */
float DHT::computeTemperature() const 
{
  float temp = _data[2];
  if (_data[3] & 0x80) 
  {
    temp = -1 - temp;
  }
  temp += (_data[3] & 0x0f) * 0.1f;
  return temp;
}

/**
 * @brief Converts raw sensor data to relative humidity
 * @return Relative humidity percentage
 * @note For DHT11: range 20-80%, resolution 5%
 */
float DHT::computeHumidity() const 
{
  return _data[0] + _data[1] * 0.1f;
}