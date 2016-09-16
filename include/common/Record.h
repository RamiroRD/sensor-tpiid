#ifndef ENTRADA_REGISTRO_H
#define ENTRADA_REGISTRO_H

#include <cstdint>
#include <ctime>
#include <cmath>
#include <tuple>


typedef int8_t Temperature;
typedef int8_t Humidity;
typedef uint8_t Pressure;
typedef float Velocity;

class Record
{
public:
	Record(Temperature t,Humidity h,Pressure p ,Velocity wx, Velocity wy,
			time_t time)
	: temp(t), hum(h), pres(p), windX(wx), windY(wy), timestamp(time){};
	
	bool operator==(const Record& other) const
	{
		return (temp		== other.temp	&&
				hum			== other.hum	&&
				pres		== other.pres	&&
				windX		== other.windX	&&
				windY		== other.windY	&&
				timestamp	== other.timestamp);
	}
	
	Velocity windSpeed() const {return std::sqrt(windX*windX+windY*windY);};
	
	/* Temperatura entera en grados C */
	const Temperature temp;
	/* Humedad relativa (0-100) */
	const Humidity hum;
	/* Presión atmosférica en hectopascales */
	const Pressure pres;
	/* Viento (vectorial) en km/h */
	const Velocity windX,windY;
	/* Hora del registro (timestamp UNIX) */
	const std::time_t timestamp;
	
	
};

#endif