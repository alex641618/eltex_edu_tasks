*.so: *.o
	gcc -shared -o libsumm.so summ.o
	gcc -shared -o libdiff.so diff.o
	gcc -shared -o librat.so rat.o
	gcc -shared -o libpro.so pro.o
	mv *.so ../plugin/
	rm -rf *.o
*.o: *.c
	gcc -fPIC *.c -c
