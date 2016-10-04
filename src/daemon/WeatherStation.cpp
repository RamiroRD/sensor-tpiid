#include "daemon/WeatherStation.h"
#include <chrono>
#include <cmath>
#include <random>
#include <functional>

WeatherStation::WeatherStation()
{
	std::default_random_engine			generator;
	std::normal_distribution<double>	distribution(0.5,1);
	auto gen = std::bind(distribution,generator);
	tempAmplitude = gen()*20;
	presAmplitude = gen()*100;
	humAmplitude = gen()*50;
	windAmplitude = gen()*20;

	tempBase = 20;
	presBase = 1000;
	humBase  = 50;
	windBase = 30;
	
}

WeatherStation::~WeatherStation()
{
	
}

Record WeatherStation::getStats()
{

	std::default_random_engine			generator;
	std::normal_distribution<double>	distribution(-0.01,0.01);
	auto gen = std::bind(distribution,generator);
	
	
	const std::time_t timestamp = 
				std::chrono::system_clock::to_time_t
				(std::chrono::system_clock::now());	
	double d = timestamp % (3600*24);
	
	const Temperature temp	= tempAmplitude * (std::abs(std::sin(d/2.0)+gen()))
							+ tempBase;

	const float humAux	= humAmplitude	* (std::abs(std::sin(d/2.0))+gen())
							+  humBase;
	const float hum = static_cast<Humidity>(std::min(100.0f,std::max(0.0f,humAux)));
							

	const Pressure pres		= presAmplitude * (std::abs(std::sin(d/2.0))+gen())
							+ presBase;

	const Velocity windX	= windAmplitude * (std::sin(d/2.0)+gen())
							+ windBase;
	
	const Velocity windY	= windAmplitude * (std::sin(d/2.0)+gen())
							+ windBase;

	return Record(temp,hum,pres,windX,windY,timestamp);
}