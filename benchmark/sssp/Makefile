all: clean o0 o1

o0: sssp.c sssp_hardened.c
	clang -lm -O0 sssp.c -o o0out
	clang -lm -O0 sssp_hardened.c -o o0hardenedout

o1: sssp.c
	clang -lm -O1 sssp.c -o o1out

clean:
	rm -f o0out
	rm -f o0hardenedout
	rm -f o1out