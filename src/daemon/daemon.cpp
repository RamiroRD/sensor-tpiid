#include <iostream>
#include <fstream>
#include <unistd.h>
#include <csignal>
#include <cstring>
#include "common/Log.h"
#include "daemon/WeatherStation.h"
#include "common/paths.h"

const int DEFAULT_REFRESH_RATE = 10;
int refreshRate;
bool running = true;


static int readConfigFile()
{
	std::fstream configFile(CONFIG_PATH, std::ios::out | std::ios::in);
	if(configFile.is_open())
	{
		if(configFile.peek() == std::ifstream::traits_type::eof())
		{
			/* TODO: ver por qué no escribe */
			std::cerr << "Archivo de configuración vacío. Escribiendo genérico.."
						<< std::endl;
			configFile << DEFAULT_REFRESH_RATE << std::endl;
			refreshRate = DEFAULT_REFRESH_RATE;
		}
		configFile >> refreshRate;
		std::cerr << "Archivo de configuración cargado!" << std::endl;
		std::cerr << "Período de muestreo: " << refreshRate << std::endl;
		
		
		configFile.close();
		return 0;
	}
	else
		perror("<1> Error");
		return -1;
		
}

 
static void handle_signal(int signal)
{
    switch (signal) {
        case SIGHUP:
			std::cerr << "SIGHUP recibido. Recargando configuración..." 
					<< std::endl;
			readConfigFile();
            break;
        case SIGTERM:
			running = false;
            std::cerr << "SIGTERM recibido. Saliendo ..." << std::endl;
            break;
        default:
            fprintf(stderr, "Señal sin handler: %d\n", signal);
            return;
    }
}
 
 int main()
 {
	struct sigaction sa;
	sa.sa_handler = &handle_signal;
	sa.sa_flags = SA_RESTART;
	sigfillset(&sa.sa_mask);
	
    if (sigaction(SIGHUP, &sa, NULL) == -1)
	{
		perror("");
        return -1;
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
        perror("");
		return -1;
    }
	 
	
	if(readConfigFile() == -1)
	{
		std::cerr << "<1>No se pudo leer archivo de configuración!" << std::endl;
		std::cerr << CONFIG_PATH << std::endl;
		return -1;
	}else{
		Log log(false);
		WeatherStation weatherStation;
		
		
		while(running)
		{
			log.insert(weatherStation.getStats());
			std::cerr << "Record insertado!" << std::endl;
			
			sleep(refreshRate);
		}
		
		/* Terminó normalmente con SIGTERM */
		return 0;
	}
		
	 

 }
