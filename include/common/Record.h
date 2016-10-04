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
	Record(Temperature t =0,Humidity h=0,Pressure p=0 ,Velocity wx=0, Velocity wy=0,
			time_t time=0)
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