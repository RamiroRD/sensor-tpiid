#ifndef LOG_H
#define LOG_H

#include "common/Record.h"
#include <list>
#include <string>

/* 
 * Tipo de dato del índice con el que se maneja el archivo.
 */
typedef uint16_t Index;

/* 
 * Tipo de dato del contador de entradas en el archivo.
 */
typedef uint32_t Count;


/* 
 * Esta clase se encarga de manejar el archivo con las mediciones de la estación
 * Debería ser la única clase que toque el archivo. Esta clase sirve tanto para 
 * el daemon que escribe sobre el log como para el programa cgi que va a acceder
 * el archivo. 
 */
class Log
{
public:
	/* 
	* Constructor. Abre el log si ya existiera o lo crea en caso contrario. Si 
	* se abre en sólo lectura, se pone un lock compartido sobre el archivo, en 
	* caso contrario, se pone un lock exclusivo sobre el archivo al insertar un
	* registro. 
	*
	* Por defecto, se abre en sólo lectura.
	*/
	Log(bool readOnly = true);
	/* 
	* Destructor. Cierra el log y libera los locks.
	*/
	~Log();
	/* 
	 * Inserta un record en el archivo. Si estuviera lleno, no se hace la
	 * escritura, se imprime un error y se sale.
	 */
	void insert(const Record&);
	/*
	 * Devuelve un booleano que es verdadero si el archivo está lleno.
	 */
	bool isFull();
	/*
	 * Devuelve un booleano que es verdadero si el archivo está vacío.
	 */
	bool isEmpty();
	/* 
	 * Devuelve la cantidad de registros almacenados (incluso los que ya no se
	 * pueden acceder).
	 */
	Count getCount();
	/* 
	 * Devuelve el último record insertado. Si no existe, cierra el programa.
	 */
	Record getLast();
	/* 
	 * Devuelve el índice del último record insertado. 
	 * Si no existe, cierra el programa. Preguntar primero si no está vacío.
	 */
	Index getLastIndex();
	/* 
	 * Devuelve el registro en el índice dado.
	 * Si no existe, cierra el programa. Preguntar primero si no está vacío.
	 */
	 Record get(const Index);
private:
	void create();
	void setLastIndex(Index);
	int lock(); //RW
	int unlock(); //RW
	int waitForLock(); //RO
	void setCount(Count);
	const std::list<Record> lastN(size_t);
	
	const bool readOnlyMode;
	int storageFD;
	int lockFD;
};


#endif