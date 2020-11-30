all: clean o0 o1

o0: sor.c
	clang -lm -O0 sor.c -o o0out

o1: sor.c
	clang -lm -O1 sor.c -o o1out

clean:
	rm -f o0out
	rm -f o1out