all: clean o0 o1

o0: matvec.c matvec_hardened.c
	clang -O0 matvec.c -o o0out
	clang -O0 matvec_hardened.c -o o0hardenedout

o1: matvec.c matvec_hardened.c
	clang -O1 matvec.c -o o1out
	clang -O0 matvec_hardened.c -o o1hardenedout

clean:
	rm -f o0out
	rm -f o0hardenedout
	rm -f o1out
	rm -f o1hardenedout