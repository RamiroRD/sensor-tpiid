IDIR=include/
ODIR=obj/
SDIR=src/
BDIR=bin/
TDIR=tests/

INSTALL=install
DAEMONDIR=/usr/sbin/
CGIDIR=/var/www/html/cgi-bin/
LOGPATH=/var/lib/sensor-tpiid/
LOGFILENAME=log
CONFPATH=/etc/sensor-tpiid/
CONFFILE=sensor-tpiid.conf
DUSER=sensor-tpiid
CGIUSER=www-data

CC=g++
CFLAGS=-std=c++14 -O3 -Wall -I $(IDIR)

$(BDIR)daemon: $(ODIR)daemon.o $(ODIR)Log.o $(ODIR)WeatherStation.o $(ODIR)paths.o
	$(CC) $(CFLAGS) -o $(BDIR)daemon $(ODIR)daemon.o $(ODIR)Log.o $(ODIR)paths.o \
	$(ODIR)WeatherStation.o

$(BDIR)show: $(ODIR)cgi.o $(ODIR)Log.o $(ODIR)paths.o
	$(CC) $(CFLAGS) -o $(BDIR)show $(ODIR)cgi.o $(ODIR)Log.o $(ODIR)paths.o
	
$(TDIR)LogTest:  $(ODIR)LogTest.o $(ODIR)Log.o $(ODIR)paths.o
	$(CC) $(CFLAGS) -o $(TDIR)LogTest $(ODIR)LogTest.o $(ODIR)Log.o \
			$(ODIR)paths.o

	
$(ODIR)LogTest.o: $(TDIR)LogTest.cpp $(IDIR)common/Log.h
	$(CC) -c $(CFLAGS) -o $(ODIR)LogTest.o $(TDIR)LogTest.cpp
	
$(ODIR)WeatherStation.o: $(SDIR)daemon/WeatherStation.cpp \
									$(IDIR)daemon/WeatherStation.h
	$(CC) -c $(CFLAGS) -o $(ODIR)WeatherStation.o \
									$(SDIR)daemon/WeatherStation.cpp

$(ODIR)daemon.o: $(SDIR)daemon/daemon.cpp $(IDIR)common/Log.h\
					$(IDIR)common/paths.h
	$(CC) -c $(CFLAGS) -o $(ODIR)daemon.o $(SDIR)daemon/daemon.cpp

$(ODIR)Log.o: $(SDIR)common/Log.cpp  $(IDIR)common/Log.h $(IDIR)common/paths.h\
			$(IDIR)common/Record.h 
	$(CC) -c $(CFLAGS) -o $(ODIR)Log.o $(SDIR)common/Log.cpp

$(ODIR)paths.o: $(SDIR)common/paths.cpp  $(IDIR)common/paths.h
	$(CC) -c $(CFLAGS) -o $(ODIR)paths.o $(SDIR)common/paths.cpp
	
$(ODIR)cgi.o: $(SDIR)cgi/cgi.cpp $(IDIR)common/Log.h $(IDIR)common/paths.h\
			$(IDIR)common/Record.h 
	$(CC) -c $(CFLAGS) -o $(ODIR)cgi.o $(SDIR)cgi/cgi.cpp 


.PHONY: all
all: $(BDIR)daemon $(BDIR)show
	
.PHONY: install
install: all
	$(INSTALL) $(BDIR)daemon $(DAEMONDIR)sensor-tpiid
	$(INSTALL) $(BDIR)show $(CGIDIR)show
	$(INSTALL) conf/* $(CGIDIR)
	mkdir -p $(CONFPATH)
	touch $(CONFPATH)$(CONFFILE)
	chown -R $(DUSER):$(DUSER) $(CONFPATH)
	chmod 777 $(CONFPATH)
	chmod 666 $(CONFPATH)$(CONFFILE)
	mkdir -p $(LOGPATH)
	touch $(LOGPATH)$(LOGFILENAME)
	chown -R $(DUSER):$(DUSER) $(LOGPATH)
	chmod 777 $(LOGPATH)
	chmod 666 $(LOGPATH)$(LOGFILENAME)
	cp -rvf conf/sensor-tpiid.service /etc/systemd/system/


.PHONY: clean

clean:
	rm -rvf $(ODIR)*.o $(BDIR)* 
