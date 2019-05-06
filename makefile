main: lex_analyzer.o synth_analyzer.o main.o
	g++ -Wall -Werror  lex_analyzer.o synth_analyzer.o main.o -o main

lex_analyzer.o: lex_analyzer.cpp lex_analyzer.h
	g++ -Wall -c lex_analyzer.cpp 

synth_analyzer.o: synth_analyzer.cpp synth_analyzer.h
	g++ -Wall -c synth_analyzer.cpp 

main.o: main.cpp 
	g++ -Wall -c main.cpp 
