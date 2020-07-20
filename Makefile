sDir = src/
bDir = build/

test: $(sDir)test.cpp $(bDir)genome.o $(sDir)lib/population_config.h $(bDir)population.o $(bDir)species.o $(bDir)innovations.o
	g++ $(sDir)test.cpp -o test $(bDir)genome.o $(bDir)population.o $(bDir)species.o $(bDir)innovations.o -g

fwtest: $(sDir)fwtest.cpp $(bDir)genome.o $(sDir)lib/population_config.h $(bDir)population.o $(bDir)species.o $(bDir)innovations.o
	g++ $(sDir)fwtest.cpp -o fwtest $(bDir)genome.o $(bDir)population.o $(bDir)species.o $(bDir)innovations.o -g

$(bDir)genome.o : $(sDir)lib/genome.cpp $(sDir)lib/genome.h
	g++ -c $(sDir)lib/genome.cpp -o $(bDir)genome.o -g

$(bDir)population.o : $(sDir)lib/population.cpp $(sDir)lib/population.h
	g++ -c $(sDir)lib/population.cpp -o $(bDir)population.o -g

$(bDir)species.o : $(sDir)lib/species.cpp $(sDir)lib/species.h
	g++ -c $(sDir)lib/species.cpp -o $(bDir)species.o -g

$(bDir)innovations.o : $(sDir)lib/innovations.cpp $(sDir)lib/innovations.h
	g++ -c $(sDir)lib/innovations.cpp -o $(bDir)innovations.o -g