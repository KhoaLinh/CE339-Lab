#include "AHT20.h"
void AHT20::begin(){
    Wire.begin();
    Wire.beginTransmission(0x38); // AHT20 address
    Wire.write(0xBE);
    Wire.endTransmission();
}

bool AHT20::startSensor() {
    // Send initialization bytes to AHT20 sensor
    Wire.beginTransmission(0x38);
    Wire.write(0xac);
    Wire.write(0x33);
    Wire.write(0x00);
    Wire.endTransmission();

    // Wait for AHT20 sensor to become ready
    unsigned long start_time = millis();
    while (millis() - start_time <= 200) {
        Wire.requestFrom(0x38, 1);
        if (Wire.available()) {
            unsigned char status_byte = Wire.read();
            if ((status_byte & 0x80) == 0) {
                // AHT20 sensor is ready, return true
                return true;
            }
        }
        delay(20);
    }

    // Timed out waiting for AHT20 sensor to become ready, return false
    return false;
}


bool AHT20::getSensors(float *h, float *t)
{
    // Send start command to sensor
    if (!startSensor()) {
        return false;
    }

    // Wait for data to be ready
    unsigned long startTime = millis();
    while ((millis() - startTime) < 200) {
        Wire.requestFrom(0x38, 6);
        if (Wire.available() >= 6) {
            break;
        }
        delay(10);
    }

    // Read data from sensor
    unsigned char buffer[6];
    if (Wire.available() < 6) {
        return false;
    }
    for (int i = 0; i < 6; i++) {
        buffer[i] = Wire.read();
    }

    // Check if data is valid
    if (buffer[0] & 0x80) {
        return false;
    }

    // Calculate humidity and temperature
    unsigned long __humi = 0;
    unsigned long __temp = 0;

    __humi = buffer[1];
    __humi <<= 8;
    __humi += buffer[2];
    __humi <<= 4;
    __humi += buffer[3] >> 4;

    *h = (float)__humi/1048576.0 * 100.0;

    __temp = buffer[3]&0x0f;
    __temp <<=8;
    __temp += buffer[4];
    __temp <<=8;
    __temp += buffer[5];

    *t = (float)__temp/1048576.0*200.0-50.0;

    // Return success
    return true;
}

bool AHT20::getTemperature(float *t)
{
    float __t, __h;
    
    int ret = getSensors(&__h, &__t);
    if(0 == ret)return 0;
    
    *t = __t;
    return 1;
}

bool AHT20::getHumidity(float *h)
{
    float __t, __h;
    
    int ret = getSensors(&__h, &__t);
    if(0 == ret)return 0;
    
    *h = __h;
    return 1;
}
