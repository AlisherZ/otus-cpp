.PHONY: all clean

all: bin/mapper bin/reducer

bin/mapper:
	if not exist bin mkdir bin
	g++ -o $@ mapper.cpp

bin/reducer:
	if not exist bin mkdir bin
	g++ -o $@ reducer.cpp

clean:
	rm -rf bin output