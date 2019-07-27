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
 * @brief   Calibrate sensor in clean air.
 *          As given in datasheet, Rs/R0 = 1 in clean area.
 *          Based on: https://www.teachmemicro.com/mq-3-alcohol-sensor/ and
 *          datasheet https://cdn.sparkfun.com/datasheets/Sensors/Biometric/MQ-3%20ver1.3%20-%20Manual.pdf
 * @param   None
 * @retval  Measured R0 value in Ohm
 */
float MQ3sensor::calibrate()
{
    float rs = readRs();
    _r0 = rs;
    return _r0;
}

/*
 * @brief   Measure and calculate alcohol concentration in air in ppm
 *          https://cdn.sparkfun.com/datasheets/Sensors/Biometric/MQ-3%20ver1.3%20-%20Manual.pdf
 * @param   None
 * @retval  Alcohol concentration in ppm
 */
float MQ3sensor::readAlcoholPpm()
{
    /* f(x) = 4.7360598103*x^(-1.1125227969) */
    return (4.7 * pow(readRatio(), -1.11));
}

/*
 * @brief   Measure and calculate alcohol concentration in air in mg/L.
 * @param   None
 * @retval  Alcohol concentration in mg/L
 */
float MQ3sensor::readAlcoholMgL()
{
    return (readAlcoholPpm() / 500.0);
}

/*
 * @brief   Measure and calculate blood alcohol concentration (BAC)
 *          based on breath alcohol concentration (BrAC).
 *          Calculation based on
 *          https://www.lionlaboratories.com/testing-for-alcohol/the-lion-units-converter/
 *          Used Blood:Breath Ratio 2000:1
 * @param   None
 * @retval  BAC in mg/ml (per mille)
 */
float MQ3sensor::readAlcoholBAC()
{
    /* 1 mg/L = 0.001 mg/ml
       BAC:BrAC = 2000:1
       BAC = (BrAC[mg/L] / 1000) * 2000
    */
    return (2.0 * readAlcoholMgL());
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
    for (uint8_t i = 0u; i < MQ_SAMPLE_TIMES; i++)
    {
        adc_result += analogRead(_ain);
        delay(MQ_SAMPLE_INTERVAL);
    }
    return ( ((float)MQ_ARDUINO_ADC_MAX * MQ_LOAD_RESISTANCE / (float)adc_result * MQ_SAMPLE_TIMES) - MQ_LOAD_RESISTANCE );
}
