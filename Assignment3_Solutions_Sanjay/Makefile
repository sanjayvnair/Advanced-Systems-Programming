lib = -lm

all: host.c philosopher.c 
	gcc -pthread -o host host.c
	gcc -pthread -o phil philosopher.c

host: host.c
	gcc -pthread -o host $<

phil: philosopher.c 
	gcc -pthread -o phil $<

clean:
	rm phil host 
	rm -rf sema* state.txt

