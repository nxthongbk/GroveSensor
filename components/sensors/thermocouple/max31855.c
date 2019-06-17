#include <legato.h>
#include <interfaces.h>

#include "periodicSensor.h"

/**
| D[31:18] | 14-Bit Thermocouple Temperature Data | These bits contain the signed 14-bit thermocouple temperature value. See Table 4.
| D17      | Reserved                             | This bit always reads 0.
| D16      | Fault                                | This bit reads at 1 when any of the SCV, SCG, or OC faults are active. Default value is 0.
| D[15:4]  | 12-Bit Internal Temperature Data     | These bits contain the signed 12-bit value of the reference junction temperature.
| D3       | Reserved                             | This bit always reads 0.
| D2       | SCV Fault                            | This bit is a 1 when the thermocouple is short-circuited to VCC. Default value is 0.
| D1       | SCG Fault                            | This bit is a 1 when the thermocouple is short-circuited to GND. Default value is 0.
| D0       | OC Fault                             | This bit is a 1 when the thermocouple is open (no connections). Default value is 0
*/

#define MAX31855_FAULT_MASK			0x10000
#define MAX31855_THERM_TEMP_MASK		0x7FFC0000
#define MAX31855_THERM_TEMP_SIGN_MASK		0x80000000
#define MAX31855_INTERNAL_TEMP_MASK		0x7FF0
#define MAX31855_INTERNAL_TEMP_SIGN_MASK	0x8000
#define MAX31855_SCV_FAULT_MASK			0x4
#define MAX31855_SCG_FAULT_MASK			0x2
#define MAX31855_OC_FAULT_MASK			0x1

le_spi_DeviceHandleRef_t spiHandle;

#if defined(WP85)
#define THERMOCOUPLE_SPI_BUS "spidev0.0"
#elif defined(WP76XX)
#define THERMOCOUPLE_SPI_BUS "spidev1.0"
#else
#error "Please define your target"
#endif

le_result_t thermocouple_init(void)
{
	le_result_t res = LE_OK;
	res = le_spi_Open(THERMOCOUPLE_SPI_BUS, &spiHandle);
	if (res != LE_OK)
	{
		LE_ERROR("le_spi_Open failed: Result = %s", LE_RESULT_TXT(res));
		exit(res);
	}
	else
	{
		LE_INFO("le_spi_Open sucess");
	}

	LE_INFO("Config the SPI");
	le_spi_Configure(spiHandle, 0, 8, 4000000, 0);

	return res;
}

le_result_t thermocouple_deinit(void)
{
	LE_INFO("le_spi_Close");
	le_spi_Close(spiHandle);
	return LE_OK;
}

uint32_t getData(void)
{
	uint32_t data = 0;
	uint8_t buffer[4];
	size_t data_size = 4;
	le_result_t res;
	res = le_spi_WriteReadFD(spiHandle,
				 buffer,
				 data_size,
				 buffer,
				 &data_size);

#ifdef DEBUG
	LE_INFO("Received Size: %d", data_size);
	LE_INFO("Received Data: ");
	for (int i = 0; i < data_size; i++)
	{
		LE_INFO("%2X", buffer[i]);
	}
#endif

	if (res != LE_OK) {
		LE_ERROR("le_spi_WriteReadFD failed: Result = %s",
			 LE_RESULT_TXT(res));
		return -1;
	} else {
		LE_INFO("le_spi_WriteReadFD sucess");
	}
	data = (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
	return data;
}

le_result_t checkError(uint32_t data)
{
	if (data & MAX31855_FAULT_MASK)
	{
		LE_INFO("Thermocouple data fault: ");
		if (data & MAX31855_SCV_FAULT_MASK)
		{
			LE_INFO("\tShort-circuited to VCC");
		}
		if (data & MAX31855_SCG_FAULT_MASK)
		{
			LE_INFO("\tShort-circuited to GND");
		}
		if (data & MAX31855_OC_FAULT_MASK)
		{
			LE_INFO("\tNo Connection");
		}
		return LE_FAULT;
	}
	else
	{
		LE_INFO("Thermocouple data OK");
	}
	return LE_OK;
}

le_result_t thermocouple_getCelsius(double * value)
{
	le_result_t res = LE_OK;
	double resC = 0.0;
	uint32_t rawData = getData();
	if (checkError(rawData) == LE_OK) {
		resC = ((rawData & MAX31855_THERM_TEMP_MASK) >> 18);
		if (rawData & MAX31855_THERM_TEMP_SIGN_MASK) {
			// negative value
			resC *= -0.25;
		} else {
			// positive value
			resC *= 0.25;
		}
	} else {
		res = LE_FAULT;
	}
	*value = resC;
	return res;
}

le_result_t thermocouple_getFarenheit(double * value)
{
	le_result_t res = LE_OK;
	double resF = 0;
	res = thermocouple_getCelsius(&resF);
	resF *= 1.8;
	resF += 32;
	*value = resF;
	return res;
}

static void SampleTemperature
(
	psensor_Ref_t ref
)
{
	double sample;
	le_result_t result = thermocouple_getCelsius(&sample);

	if (result == LE_OK)
	{
		psensor_PushNumeric(ref, 0 /* now */, sample);
	} else {
		LE_ERROR("Failed to read sensor (%s).", LE_RESULT_TXT(result));
	}
}

COMPONENT_INIT
{
	LE_INFO("Create period sensor");
	thermocouple_init();

	// testing procedure
	// double temp = 0;
	// while(true) {
	// 	thermocouple_getCelsius(&temp);
	// 	LE_INFO("Thermocouple value: %0.2f Celsius", temp);
	// 	sleep(1);
	// }

	psensor_Create("thermocouple",
		       DHUBIO_DATA_TYPE_NUMERIC,
		       "",
		       SampleTemperature);
}
