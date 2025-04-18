# Linux Makefile for VART Example "Animation"
# (C) Bruno de Oliveira Schneider - May 2014

# This Makefile requires a local directory "vart" with
# source code for the V-ART framework. Create a symbolic
# link to the real directory and you'll be OK.

APPLICATION = mdgp
CXXFLAGS = -O2 -Wall -I. -DIL_LIB -std=c++11
LDFLAGS = 
LDLIBS = 

OBJECTS = main.o grafo.o grupo.o solucaoInicial.o metodoLH.o metodoGC.o\
	metodoWJ.o buscaLocal.o perturbacao.o

# first, try to compile from this project
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

#~ # second, try to compile from the core
#~ %.o: vart/source/%.cpp vart/%.h
#~ 	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

#~ # last, try to compile from contribs
#~ %.o: vart/contrib/source/%.cpp vart/contrib/%.h
#~ 	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

$(APPLICATION): $(OBJECTS) main.o grafo.o grupo.o solucaoInicial.o\
	metodoLH.o metodoGC.o metodoWJ.o buscaLocal.o perturbacao.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

#gdb:
#	CXXFLAGS = $(CXXFLAGS) -ggdb
#	$(APPLICATION)

clean:
	rm -f *.o *~ $(APPLICATION)
