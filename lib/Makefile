Runtime: runtime.c
	gcc -c -m32 -Wall -Werror -o runtime.o runtime.c
Test: test.c
	gcc -c -m32 -Wall -Werror -o test.o test.c
	gcc -m32 -Wall -Werror runtime.o test.o
	gcc -c -m32 -S -o test.s test.c
	gcc -c -m32 -S -o runtime.s runtime.c
