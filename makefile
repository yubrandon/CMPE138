ifeq ($(OS), Windows_NT)
	EXEC=run.exe
else
	EXEC=run
endif

CXX=g++
CFLAGS= -g -std=c++11 -I./include/jdbc
LDFLAGS=-g -std=c++11 -L./lib64 -lmysqlcppconn 
CFILES= main.cpp misc_func.cpp sql_func.cpp
OBJECTS = main.o misc_func.o sql_func.o

SHARED_LIB=-L./lib64

all: $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(EXEC) $(SHARED_LIB)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $^


clean: 
	rm *.o run
