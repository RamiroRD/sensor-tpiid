#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <exception>
#include "common/Record.h"
#include "common/Log.h"
#include "common/paths.h"

void fallar();

int main()
{
    Log log;

	float acumW = 0;
    float acumH = 0;
    float acumT = 0;
    float acumP = 0;

	const int N = std::min((int)log.getCount(),10);


	/*
	 * Sacamos aca por si se escribe en el archivo durante la ejecución del
	 * programa CGI.
	*/
	const int lastInd = log.getLastIndex();

    if (!log.isEmpty())
    {
        for(int i=0; i<N; i++)
        {
			const Record r = log.get(lastInd-i);
            acumW += r.windSpeed();
            acumH += r.hum;
            acumT += r.temp;
            acumP += r.pres;
        }

		acumW = acumW / N;
		acumH = acumH / N;
        acumT = acumT / N;
        acumP = acumP / N;


		int freq;
		try
		{
			std::ifstream confFile(CONFIG_PATH);
			confFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
			confFile >> freq;
		}catch(std::exception e)
		{
			std::cout << "Error: ";
			std::cout << e.what() << std::endl;
			return -1;
 		}




		std::cout << "Content-Type: text/html;charset=utf8" << std::endl << std::endl << std::endl;;

		std::cout << "<html><head><title>Estaci&oacuten  meteorol&oacutegica</title></head>";
		
		std::cout <<"<body><div align=\"center\"><br/><br/><h1> Estaci&oacuten Meteorol&oacutegica </h1> <br><br>";
		
		std::cout <<"<br><hr>";
		
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
		
		std::cout <<"<table><th>Muestra</th><th>Temperatura</th><th>Presi&oacuten</th><th>Humedad</th>	<th>Viento</th>";


		const Record ultimo = log.get(lastInd);
	    std::time_t tim = ultimo.timestamp;
        std::tm * ptm = std::localtime(&tim);
        char hora[32];
        // Format: Mo, 15.06.2009 20:20:00
        std::strftime(hora, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
		std::cout << "<tr><td>" << hora << "</td><td>" << (int)ultimo.temp << "</td><td>" << (int)ultimo.pres << "</td><td>" << (int)ultimo.hum << "</td><td>" << ultimo.windSpeed() << "</td></tr>";
		std::cout << "<tr><td>Promedio 10 &uacuteltimas</td><td>" << acumT
				<< "</td><td>" << acumP
				<< "</td><td>" << acumH
				<< "</td><td>" << acumW
				<< " km/h</td></tr>";
		std::cout << "</table><hr>";
		std::cout << "<p>Se tomaron " << N << " muestras." << std::endl;
		std::cout << "</p></div></body></html>" << std::endl;
		return 0;
   }
   else
   {
		fallar();
		return -1;
   }

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
