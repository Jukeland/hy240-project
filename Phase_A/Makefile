CC=g++
CFLAGS=-std=c++11 -g -Wall

cs240StreamingService: main.cpp streaming_service.h
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f cs240StreamingService
