all: clean o0 o1

o0: newton.c
	clang -O0 newton.c -o o0out

o1: newton.c
	clang -O1 newton.c -o o1out

clean:
	rm -f o0out
	rm -f o1out