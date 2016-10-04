#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <exception>
#include "common/Record.h"
#include "common/Log.h"
#include "common/paths.h"

void fallar();
const int AMOUNT = 10;

int main()
{
    Log log;
	const int N = std::min((int)log.getCount(),AMOUNT);
	const Index lastInd = log.getLastIndex();
	int freq;

	std::vector<Record> records;
	for(int i=0; i<N; i++)
			records.push_back(log.get(lastInd-i));

	try
	{
		std::ifstream confFile(CONFIG_PATH);
		confFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		confFile >> freq;
	}catch(std::exception e)
	{
		std::cout << "Content-Type: text/plain;charset=utf8" << std::endl << std::endl << std::endl;
		std::cout << "Error: ";
		std::cout << e.what() << std::endl;
		return -1;
 	}

	std::cout << "Content-Type: text/html;charset=utf8" << std::endl << std::endl << std::endl;
	std::cout << "<html><head><title>Estaci&oacuten  meteorol&oacutegica</title></head>";
	std::cout <<"<body><div align=\"center\"><br/><br/><h1> Estaci&oacuten Meteorol&oacutegica </h1> <br><br>";
	std::cout <<"<br><hr>";
		
		
		
	/*
	 * Pre-tabla
	 */
	std::cout <<"<table><tr><td><a href=\"./show\">Recargar Sitio</a></td>";		
	std::cout <<"<td>" << "Frecuencia de muestreo (s):" << "</td>";
	std::cout << "<td>";
	std::cout << "<form action=\"change_freq\">";
	std::cout << "<select name=\"freqs\">";
	auto valores 	= {1,2,5,10,30,60};
	for(auto i : valores)
	{
		std::cout << "<option value=\"" << i << "\"";
		if (freq == i)
			std::cout << " selected";
		std::cout << ">" << i << "</option>";
	}
		
	std::cout << "</select><input type=\"submit\"/></form>";
	std::cout << "</td></tr></table><br>";
		
		
	/*
	 * Tabla
	*/	
	std::cout <<"<table><th>Muestra</th><th>Temperatura</th><th>Presi&oacuten</th><th>Humedad</th>	<th>Viento</th>";


	float 	acumW = 0,
			acumH = 0,
			acumT = 0,
			acumP = 0;
	for(const auto &item : records)
	{	
		const std::time_t tim = item.timestamp;
		const std::tm * ptm = std::localtime(&tim);
		char hora[32];
		std::strftime(hora, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
		std::cout 	<< "<tr><td>" << hora << "</td><td>"
					<< (int)item.temp << "</td><td>"
					<< (int)item.pres << "</td><td>" 
					<< (int)item.hum << "</td><td>" 
					<< item.windSpeed() << "</td></tr>";
						
		acumW += item.windSpeed();
		acumH += item.hum;
		acumT += item.temp;
		acumP += item.pres;
	}
	
		std::cout 	<< "<tr><td>Promedio 10 &uacuteltimas</td><td>" << acumT / N
					<< "</td><td>" << acumP / N
					<< "</td><td>" << acumH / N
					<< "</td><td>" << acumW / N
					<< " km/h</td></tr>";
				
				
		std::cout << "</table>";
		std::cout << "<hr><p>Se tomaron " << N << " muestras de " << log.getCount() << ".</p>" ;
		
		
		std::cout << "</body></html>" << std::endl;
		return 0;


}



void fallar()
{
	std::cout << "Content-type: text/html" << std::endl << std::endl;
	std::cout << "<html><head><title>Estaci&oacumten meteorol&oacumtegica</title>";
	// std::cout << " <META HTTP-EQUIV="refresh" CONTENT="15">"; esto hace que la pag se actualice sola
	std::cout <<"</head><body><div align=\"center\"><br/><br/><h1> Estacion Meteorologica </h1>";
	std::cout <<"<br/><br/><br/><hr>";
	std::cout <<"<table><tr><td><a href=\"./show\">Recargar Sitio</a>";
	std::cout << "</td></tr></table><hr><p>Falló la lectura de los registros.</p></div></body></html>" << std::endl;
}
