ifeq ($(OS), Windows_NT)
	EXEC=run.exe
else
	EXEC=run
endif

CXX=g++
CFLAGS= -g -std=c++11 -I./include/jdbc
LDFLAGS=-g -std=c++11 -L./lib64 -lmysqlcppconn 
CFILES= $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(CFILES))
INCLUDE_FMT=-I/include/spdlog/fmt -lspdlog -lfmt

SHARED_LIB=-L./lib64

all: $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(EXEC) $(SHARED_LIB) $(INCLUDE_FMT)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $^


clean: 
	rm *.o run
