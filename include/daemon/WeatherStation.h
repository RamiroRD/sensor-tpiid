#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H
#include "common/Record.h"

class WeatherStation
{
public:
	WeatherStation();
	virtual ~WeatherStation();
	Record getStats();
private:
	float tempAmplitude,tempBase;
	float presAmplitude,presBase;
	float humAmplitude,humBase;
	float windAmplitude,windBase;
};


#endif