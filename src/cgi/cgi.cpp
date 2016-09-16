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




		std::cout << "Content-type: text/html" << std::endl << std::endl;

		std::cout << "<html><head><title>Estaci&oacuten  meteorol&oacutegica</title>";
		//std::cout << " <META HTTP-EQUIV="refresh" CONTENT="15">"; esto hace que la pag se actualice sola
		std::cout <<"</head> \n \n <body><div align=\"center\"><br/><br/><h1> Estaci&oacuten Meteorol&oacutegica </h1>";
		std::cout <<"<br/><br/> \n \n <br/><hr>";
		std::cout <<"<table><tr><td><a href=\"./show\">Recargar Sitio</a>";
		std::cout <<"</td><td style=\"padding-left:80px;\"><p>Frecuencia de Muestreo (Segundos):</p>	<select>";



		auto valores 	= {1,2,5,10,30,60};
		for(auto i : valores)
		{
            std::cout <<"  <option value=\"" << i << "\"";
			if (i == freq) std::cout << "selected";
			std::cout <<">" << i << "</option>";
		}

		std::cout <<" </select></td><td>";
		std::cout <<"<table><tr><td>	<a href=\"./change_freq_1.sh\">Setear Frecuencia en: 1</a>";
		std::cout <<"</td></tr><tr><td>	<a href=\"./change_freq_2.sh\">Setear Frecuencia en: 2</a></td></tr><tr><td>";
		std::cout <<"	<a href=\"./change_freq_5.sh\">Setear Frecuencia en: 5</a></td></tr><tr><td>	<a href=\"./change_freq_10.sh\">Setear Frecuencia en: 10</a></td></tr><tr><td>";
		std::cout <<"	<a href=\"./change_freq_30.sh\">Setear Frecuencia en: 30</a></td></tr></td></tr><tr><td>	<a href=\"./change_freq_60.sh\">Setear Frecuencia en: 60</a></td></tr></table>";
		std::cout <<"</td></tr></table> \n <hr><table><th>Muestra</th><th>Temperatura</th><th>Presi&oacuten</th><th>Humedad</th>	<th>Viento</th>";


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
		std::cout << "</table><hr><p>Se tomaron " << N
					<< " muestras.</p></div></body></html>" << std::endl;
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
