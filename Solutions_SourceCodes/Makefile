lib = -lm

all: combiner.c mapper.c reducer.c 
	gcc -o combiner combiner.c 
	gcc -o mapper mapper.c 
	gcc -o reducer reducer.c 

combiner: combiner.c
		gcc -o combiner $<

mapper: mapper.c 
	gcc -o mapper $<

reducer: reducer.c 
	gcc -o reducer $<

clean:
	rm combiner mapper reducer

