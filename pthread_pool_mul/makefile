pool:pthread_pool.o test.o
	g++ -o $@ $^ -std=c++11 -lpthread
pthread_pool.o:pthread_pool.cpp
	g++ -c  $^ -std=c++11 -lpthread
test.o:test.cpp pthread_pool.h
	g++ -c  $^ -std=c++11 -lpthread
