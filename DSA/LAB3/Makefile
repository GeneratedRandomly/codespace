main: hashtable
	g++ main.cpp hashtable.o -o main
hashtable:
	g++ hashtable.cpp -o hashtable.o -c
gen:
	g++ gen.cpp -o gen
clean:
	rm -f *.o gen main *.in
	find . -type f -name '*.txt' ! -name 'poj.txt' ! -name 'hdu.txt' -exec rm -f {} +
	find . -type f -executable ! -name '*.sh' -exec rm -f {} +
cltxt:
	find . -type f -name '*.txt' ! -name 'poj.txt' ! -name 'hdu.txt' -exec rm -f {} +