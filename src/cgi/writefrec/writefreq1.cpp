#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include "common/Record.h"
#include "common/Log.h"
#include "common/paths.h"

void fallar();


int main()
{

		int freq;
    /*  std::ifstream confFile(CONFIG_PATH);
		confFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
		confFile >> freq;*/
		freq=10;



        std::ifstream confFile("text.txt");
        new_freq = 1;
        confFile << new_freq;
        confFile.close();



system("reload_daemon.sh");

	std::cout << "Content-type: text/html" << std::endl << std::endl;
	std::cout << "<html><head><title>Estaci&oacumten meteorol&oacumtegica</title>";
	// std::cout << " <META HTTP-EQUIV="refresh" CONTENT="15">"; esto hace que la pag se actualice sola
	std::cout <<"</head><body><div align=\"center\"><br/><br/><h1> Estacion Meteorologica </h1>";
	std::cout <<"<br/><br/><br/><hr>";
	std::cout <<"<table><tr><td><a href=\"./show\">Recargar Sitio</a>";
	std::cout << "</td></tr></table><hr><p>Finalizó correctamente la escritura del archivo.</p></div></body></html>" << std::endl;

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
	std::cout << "</td></tr></table><hr><p>Falló la escritura del archivo.</p></div></body></html>" << std::endl;
}
