all: clean o0 o1

o0: matvec.c
	clang -O0 matvec.c -o o0out

o1: matvec.c
	clang -O1 matvec.c -o o1out

clean:
	rm -f o0out
	rm -f o1out