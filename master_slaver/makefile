main:test.o data_translate.o utils.o common.o 
	g++ -o $@ $^ -std=c++11 -g
	
test.o:test.cpp data_translate.h test.h
	g++ -c test.cpp -std=c++11 -g
	
data_translate.o:data_translate.cpp
	g++ -c data_translate.cpp -std=c++11 -g

utils.o:utils.cpp
	g++ -c utils.cpp -std=c++11 -g

common.o:../common/common.c
	g++ -c ../common/common.c -std=c++11 -g
	
clean:
	rm *.o main