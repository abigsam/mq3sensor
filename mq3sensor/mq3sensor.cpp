#include "mq3sensor.h"

/*
 * @brief   MQ3sensor class constructor
 * @param   analog_in: number of Arduino analog input
 */
MQ3sensor::MQ3sensor(uint8_t analog_in)
{
    _ain = analog_in;
}

/*
 * @brief   Calibrate sensor: current Rs set as R0
 * @param   None
 * @retval  Measured R0 value in Ohm
 */
float MQ3sensor::calibrate()
{
    _r0 = readRs();
    return _r0;
}

/*
 * @brief   Measure and calculate alcohol concentration in ppm
 * @param   None
 * @retval  Alcohol concentration in ppm
 */
float MQ3sensor::readAlcoholPpm()
{
    return (500.0 * readAlcoholMgL());
}

/*
 * @brief   Measure and calculate alcohol concentration in mg/L
 * @param   None
 * @retval  Alcohol concentration in mg/L
 */
float MQ3sensor::readAlcoholMgL()
{
    float ratio = readRatio();
    /* f(x) = 0.400292495*x^(-1.4369198088) */
    return (0.4 * pow(ratio, -1.437));
}

/*
 * @brief   Measure and calculate ratio Rs/R0
 * @param   None
 * @retval  Ratio resistance Rs/R0
 */
float MQ3sensor::readRatio()
{
    return (readRs() / _r0);
}

/*
 * @brief   Return previously measured R0 resistance in Ohms
 * @param   None
 * @retval  R0 in Ohms
 */
float MQ3sensor::getRo()
{
    return _r0;
}

/*
 * @brief   Set R0 in ohms
 * @param   r0: value of R0 in Ohms
 * @retval  None
 */
void MQ3sensor::setRo(float r0)
{
    _r0 = r0;
}

/*
 * @brief   Read current Rs
 * @param   None
 * @retval  Current Rs value in Ohms
 */
float MQ3sensor::readRs()
{
    int adc_result = 0;
    float result = 0.0;
    for (uint8_t i = 0u; i < MQ_SAMPLE_TIMES; i++)
    {
        adc_result += analogRead(_ain);
        delay(MQ_SAMPLE_INTERVAL);
    }
    return ( ((float)MQ_ARDUINO_ADC_MAX * MQ_LOAD_RESISTANCE / (float)adc_result * MQ_SAMPLE_TIMES) - MQ_LOAD_RESISTANCE );
}
