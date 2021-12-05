my: assembler.o struct.o first_check.o second_check.o function.o
	gcc -g assembler.o struct.o first_check.o second_check.o function.o

assembler.o: assembler.c info.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

struct.o: struct.c info.h struct.h
	gcc -c -Wall -ansi -pedantic struct.c -o struct.o

firstcheck.o: first_check.c info.h struct.h
	gcc -c -Wall -ansi -pedantic first_check.c -o first_check.o

second_check.o: second_check.c info.h struct.h
	gcc -c -Wall -ansi -pedantic second_check.c -o second_check.o

function.o: function.c info.h
	gcc -c -Wall -ansi -pedantic function.c -o function.o

