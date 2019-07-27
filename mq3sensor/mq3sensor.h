

#ifndef __MWQ3SENSOR_H
#define __MWQ3SENSOR_H

#include <Arduino.h>
#include <math.h>

#define MQ_SAMPLE_TIMES     (8u)
#define MQ_SAMPLE_INTERVAL  (20u)
#define MQ_HEATING_TIME     (6000u)
#define MQ_LOAD_RESISTANCE  ((float)1000.0)
#define MQ_ARDUINO_ADC_MAX  (1024u)
#define MQ_BAC_LIMIT        ((float) 0.20) /* Value for Ukraine */


class MQ3sensor {
public:
    MQ3sensor(uint8_t analog_in);

    float calibrate();
    float readAlcoholPpm();
    float readAlcoholMgL();
    float readAlcoholBAC();
    float readRatio();
    float getRo();
    void  setRo(float r0);

// private:
    float readRs();

private:
    uint8_t _ain;
    float _r0;
};


#endif  /* __MWQ3SENSOR_H */