#include "daemon/WeatherStation.h"
#include <chrono>

WeatherStation::WeatherStation()
{
	
}

WeatherStation::~WeatherStation()
{
	
}

Record WeatherStation::getStats()
{
	using namespace std::chrono;
	time_t tim = system_clock::to_time_t(system_clock::now());
	return Record(21,21,3423,0.23,12.2,tim);
}