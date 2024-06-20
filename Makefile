CCFLAGS=-Wall -Werror

.PHONY: clean

run: build
	./tema1

build: main.c 
	gcc main.c functii.c $(CCFLAGS) -o tema1 -g

# regula folosita pentru testare
# valgrind:
# 	valgrind --tool=memcheck --leak-check=full --error-exitcode=1 -s ./main.out 

clean:
	rm ./tema1