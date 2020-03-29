CXX=g++-7
CXXFLAGS= -Wall -Wextra -pedantic  -std=c++17 -O0 -g -Iinclude/
LDFLAGS=$(CXXFLAGS)
LIBS=-pthread -lboost_program_options
OBJ=$(SRC:.cc=.o)

all:  cache_server

cache_server: cache_server.o cache_lib.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

test_evictors: lru_evictor.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o cache_server
