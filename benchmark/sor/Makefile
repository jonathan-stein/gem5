all: clean o0 o1

o0: sor.c sor_hardened.c
	clang -lm -O0 sor.c -o o0out
	clang -lm -O0 sor_hardened.c -o o0hardenedout

o1: sor.c
	clang -lm -O1 sor.c -o o1out

clean:
	rm -f o0out
	rm -f o0hardenedout
	rm -f o1out