my_kalc: my_kalc.c summ.c diff.c pro.c rat.c my_kalc.h
	gcc summ.c diff.c pro.c rat.c -c
	ar rc libmy_kalc.a summ.o diff.o pro.o rat.o
	gcc my_kalc.c -o my_kalc -L. -lmy_kalc
	./my_kalc
	rm -rf my_kalc summ.o diff.o pro.o rat.o libmy_kalc.a