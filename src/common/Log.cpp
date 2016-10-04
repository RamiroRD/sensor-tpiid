#include "common/Log.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <cstring>
#include <cassert>
#include "common/paths.h"


static constexpr size_t HEADER_SIZE = sizeof(Count)+sizeof(Index);
static constexpr size_t RECORD_SIZE = sizeof(Temperature)
									+ sizeof(Humidity)   + sizeof(Pressure)
									+ 2*sizeof(Velocity) + sizeof(time_t);
static const char * LOCK_FILENAME="lock";

Log::Log(bool readOnly)
: readOnlyMode(readOnly),storageFD(-1),lockFD(-1)
{
	/*
	* Tamaño que debería tener el archivo si esta todo bien. Se cuentan los dos
	* primeros campos más la cantidad de records direccionables según
	* sizeof(Index).
	*/
	const size_t MIN_SIZE = 2*sizeof(Index);
	const size_t MAX_SIZE = MIN_SIZE + RECORD_SIZE*(1<<(sizeof(Index)*8));
	
	/* Nos movemos al directorio donde está el log */
	if(chdir(LOG_PATH) == -1)
	{
		std::cerr << "<1>No se puede acceder al directorio " << LOG_PATH 
					<< ". Razón: " << strerror(errno) << std::endl;
		exit(-1);
	}
	
	/* Abrimos el archivo en el modo que corresponda */
	if(readOnlyMode)
		storageFD = open(LOG_FILENAME,  O_RDONLY);
	else
		storageFD = open(LOG_FILENAME,  O_CREAT | O_RDWR);
	
	/*
	 * Reportar el caso en que no se puede abrir ni crear el archivo.
	 */
	if(storageFD == -1)
	{
		std::cerr << "<1>No se pudo abrir log. Razón: " << strerror(errno) 
					<< std::endl;
		exit(-1);
	}
	
	/*
	 * Verificamos que el tamaño del archivo esté dentro de los límites como 
	 * primera medida de integridad. Caso contrario creamos un log nuevo.
	 */
	const size_t SIZE_AT_OPEN = lseek(storageFD,0,SEEK_END);
	if(SIZE_AT_OPEN < MIN_SIZE || SIZE_AT_OPEN>MAX_SIZE-1)
	{	 
		std::cerr << "<2>Log tiene tamaño inválido. " << std::endl;
		create();
	}else{
		/*
		 * Hay que verificar que el espacio donde están los registros sea 
		 * múltiplos del tamaño del registro
		 */
		if((SIZE_AT_OPEN-HEADER_SIZE)%RECORD_SIZE != 0)
			std::cerr << "<2>Espacio de records tiene tamaño incorrecto."
						<< std::endl;
	}
	
	/*
	 * A esta altura debería estar garantizado que están los dos primeros campos
	 * del archivo están presentes y está todo usable.
	 */
	std::cerr << "Log abierto con éxito ";
	if(readOnlyMode)
		std::cerr << "en modo de sólo lectura." << std::endl;
	else
		std::cerr << "en modo de lectura y escritura." << std::endl;
	
	std::cerr << "Cantidad de registros: " << getCount() << std::endl;
	std::cerr << "Índice del último escrito (base cero): " << getLastIndex() 
			<< std::endl;
}



Count Log::getCount()
{
	Count aux;
	lseek(storageFD,0,SEEK_SET);
	if(read(storageFD,&aux,sizeof(Count)) == -1)
	{
		std::cerr << "<1>No se pudo conseguir cantidad de entradas. Saliendo..."
			<< std::endl;
		exit(-1);
	}
	
	return aux;
}

Index Log::getLastIndex()
{
	Index aux;
	lseek(storageFD,sizeof(Count),SEEK_SET);
	if(read(storageFD,&aux,sizeof(Index)) == -1)
	{
		std::cerr << "<1>No se pudo conseguir índice de último record. Saliendo..."
			<< std::endl;
		exit(-1);
	}
	return aux;
}

void Log::setCount(Count count)
{
	lseek(storageFD,0,SEEK_SET);
	if(write(storageFD,&count,sizeof(Count)) == -1)
	{
		std::cerr << "<1>No se pudo escribir campo de cantidad. Saliendo..."
			<< std::endl;
		exit(-1);
	}
}

void Log::setLastIndex(Index index)
{
	lseek(storageFD,sizeof(Count),SEEK_SET);
	if(write(storageFD,&index,sizeof(Index)) == -1)
	{
		std::cerr << "<1>No se pudo escribir índice de último record. Saliendo..."
			<< std::endl;
		exit(-1);
	}	
}

/*
 * Crea un log vacío bien formado. Se asume que ya esta seteado el working 
 * directory. Este método deja el archivo abierto!
 */
void Log::create()
{
	if(!readOnlyMode)
		{
		std::cerr << "Creando log..." << std::endl;
		close(storageFD);
		storageFD = open(LOG_FILENAME, O_CREAT | O_RDWR);
		if(storageFD == -1)
		{
			std::cerr << "<1>No se pudo crear log. Razón: " << strerror(errno)
				<< std::endl;
			exit(-1);
		}
		lseek(storageFD,0,SEEK_SET);
		setCount(0);
		setLastIndex(-1);
			
	}else{
		std::cerr << "<1>Estamos en modo lectura, saliendo..." << std::endl;
		exit(-1);
	}
}

bool Log::isEmpty()
{
	return (getCount()==0);
}

bool Log::isFull()
{
	return (getCount()==((1<<sizeof(Count))-1));
}

void Log::insert(const Record &record)
{
	if(!readOnlyMode)
	{
		const Count count		= getCount()+1;
		// Se hace el incremento aca para que se de el overflow naturalmente
		const Index last		= getLastIndex()+1;
		const size_t writePos = HEADER_SIZE + last*RECORD_SIZE;
		
		errno = 0;
		lseek(storageFD,writePos,SEEK_SET);
		write(storageFD, &record.temp,		sizeof(record.temp));
		write(storageFD, &record.hum,		sizeof(record.hum));
		write(storageFD, &record.pres,		sizeof(record.pres));
		write(storageFD, &record.windX,		sizeof(record.windX));
		write(storageFD, &record.windY,		sizeof(record.windY));
		write(storageFD, &record.timestamp,	sizeof(record.timestamp));
		assert(errno==0);
		auto currPos = lseek(storageFD,0,SEEK_CUR);
		assert(currPos-writePos == RECORD_SIZE);
		
		/*
		 * Actualizamos la cabezera del archivo. Esto forma una sección critica
		 */
		lock();
		setCount(count);
		setLastIndex(last % (1<<(8*sizeof(Index))));

		fsync(storageFD);
		unlock();
		/* Fin de sección crítica */
		
		if(count==0)
			std::cerr << "Overflow en campo de cantidad de registros!";
	}else{
		std::cerr << "<2>Se está tratando de insertar en modo de sólo lectura!"
			<< std::endl;
	}
}

Record Log::getLast()
{
	return get(getLastIndex());
}

int Log::lock()
{
	if(!readOnlyMode)
	{
		chdir(LOG_PATH);
		lockFD = open(LOCK_FILENAME, O_CREAT | O_RDWR);
		if(lockFD == -1)
		{
			std::cerr << "<1>No se pudo poner lock. Saliendo..." << std::endl;
			perror("<1>");
			return -1;
		}
		if(flock(lockFD,LOCK_EX) == -1)
		{
			perror("<1>No se pudo flockear el lock. Razón");
			return -1;
		}
		return 0;
	}else{	
		std::cerr << "No se puede poner lock. Estamos en modo sólo lectura."
			<< std::endl;
		return -1;
	}
}

int Log::unlock()
{
	if(!readOnlyMode)
	{
		flock(lockFD,LOCK_UN);
		close(lockFD);
		if(unlink(LOCK_FILENAME) == -1)
		{
			perror("<1> No se pudo sacar lock. Razón");
			return -1;
		}else		
			return 0;
	}else{	
		std::cerr << "No se puede sacar lock. Estamos en modo sólo lectura."
			<< std::endl;
		return -1;
	}
}

int Log::waitForLock()
{	
	int count = 0;
	const int MAX_COUNT = 5;
	while(access(LOCK_FILENAME, F_OK) == 0 && count < MAX_COUNT)
	{
		perror("");
		sleep(1);
		count++;
	}
	if (count == 5)
	{
		std::cerr << "No se pudo acceder al archivo." << std::endl;
		std::cerr << "Se intentaron " << count << " veces." << std::endl;
		return -1;
	}else
		return 0;
}

Record Log::get(const Index index)
{
	if(waitForLock() == -1)
		exit(-1);
	Temperature temp;
	Humidity hum;
	Pressure pres;
	Velocity windX;
	Velocity windY;
	time_t timestamp;
	if(index < getCount())
	{
		const size_t readPos = HEADER_SIZE + index*RECORD_SIZE;
		
		errno=0;
		lseek(storageFD,readPos,SEEK_SET);
		read(storageFD, &temp	,sizeof(temp));
		read(storageFD, &hum,	sizeof(hum)); 
		read(storageFD, &pres,	sizeof(pres));
		read(storageFD, &windX,	sizeof(windX));
		read(storageFD, &windY,	sizeof(windY));
		read(storageFD, &timestamp,sizeof(timestamp));
		assert(errno==0);
	}else{
		std::cerr << "<1>Se está tratando de leer un registro no existente!"
			<< std::endl;
		exit(-1);
	}
	return Record(temp,hum,pres,windX,windY,timestamp);
}

Log::~Log()
{
	close(storageFD);
	if(!readOnlyMode)
		unlock();
	std::cerr << "Log cerrado!" << std::endl;
}