#include "common/Log.h"
#include "common/Record.h"
#include <iostream>
#include <cassert>
#include <ctime>
#include <unistd.h>

int main()
{
	if(unlink("/var/lib/sensor-tpii/log") == -1)
	{
		perror("No se pudo borrar el archivo");
		/* Si fue alguna otra razón distinta a que no exista el archivo, salir*/
		if(errno != ENOENT)
			return -1;
	}else{
		std::cout << "Log borrado." << std::endl;
	}
	
	time_t tim;
	time(&tim);
	Record r1=Record(21,21,3423,0.23,12.2,tim);
	time(&tim);
	Record r2=Record(30,50,1423,-1.42,56.25,tim);
	time(&tim);
	Record r3=Record(-15,99,1025,30,-40,tim);
	
	std::cout << "Comienzo del test de Log!" << std::endl;
	{
		Log log(false);
		log.insert(r1);
		assert(log.getCount() == 1);
		assert(log.getLastIndex() == 0);
		std::cout << "OK" << std::endl;
		log.insert(r2);
		assert(log.getCount() == 2);
		assert(log.getLastIndex() == 1);
		std::cout << "OK" << std::endl;
		assert(log.getLast() == r2);
		std::cout << "OK" << std::endl;
		log.insert(r3);
		assert(log.getCount() == 3);
		std::cout << "OK" << std::endl;
		assert(log.getLast() == r3);
		std::cout << "OK" << std::endl;
	}
	
	std::cout << std::endl;
	{
		Log log;
	}
	
	std::cout << "Fin del test de Log!" << std::endl;
	return 0;
}

