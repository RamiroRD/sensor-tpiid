#include <iostream>
#include <fstream>
#include "common/paths.h"


int main()
{
	int freq = 10;
	char * input = getenv("QUERY_STRING");
	if(input==NULL)
	{
		std::cout << "Content-type: text/plain" << std::endl  << std::endl;
		std::cout << "No se pudo leer parametro GET." << std::endl;
	}
	sscanf(input,"%*[^=]=%d",&freq);
	std::ofstream confFile(CONFIG_PATH,std::ofstream::out);
	confFile << freq << std::endl;
	system("sudo systemctl reload sensor-tpiid > /dev/null 2>&1");
	
	std::cout << "Content-type: text/html" << std::endl  << std::endl;
	//std::cout << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">";
	std::cout << "<meta http-equiv=\"refresh\" content=\"1;url=./show\" />";
	std::cout << "<html>";
	std::cout << "<head>";
	std::cout << "<title>Nueva Frecuencia:"<< freq << "</title>";
	std::cout << "</head>";
	std::cout << "<body>";
	std::cout << "</body>";
	std::cout << "<p>La frecuencia fue cambiada correctamente a: "<< freq << "</p>";
	std::cout << "</html>";
	return 0;
}

