all : main

main : main.o
	g++ -o $@ $^

%.o : %.cpp
	g++ -c -O3 -o $@ $<