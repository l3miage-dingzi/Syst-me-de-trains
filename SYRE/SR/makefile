all :./bin/main ./bin/client

./bin/main:./bin/main.o ./bin/fonc_ser.o
	gcc -o  ./bin/main ./bin/main.o ./bin/fonc_ser.o

./bin/main.o:./source/main.c ./header/fonc_ser.h
	gcc -c -Wall ./source/main.c -o ./bin/main.o

./bin/fonc_ser.o:./source/fonc_ser.c ./header/fonc_ser.h
	gcc -c -Wall ./source/fonc_ser.c -o ./bin/fonc_ser.o

./bin/client:./bin/client.o ./bin/fonc_cli.o
	gcc -o  ./bin/client ./bin/client.o ./bin/fonc_cli.o

./bin/client.o:./source/client.c ./header/fonc_cli.h
	gcc -c -Wall ./source/client.c -o ./bin/client.o

./bin/fonc_cli.o:./source/fonc_cli.c ./header/fonc_cli.h
	gcc -c -Wall ./source/fonc_cli.c -o ./bin/fonc_cli.o

clean:
	rm -f  ./bin/*.o ./bin/main ./bin/client
