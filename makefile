#Este es el archivo de makefile para appKubo.c

appKubo.exe: appKubo.o funCkubo.o funGTKkubo.o
	gcc appKubo.o funCkubo.o funGTKkubo.o -o appKubo.exe `pkg-config --libs gtk+-2.0`

funGTKkubo.o: funGTKkubo.c tiposGTK.h
	gcc -c funGTKkubo.c `pkg-config --cflags gtk+-2.0`

funCkubo.o: funCkubo.c tiposGTK.h
	gcc -c funCkubo.c `pkg-config --cflags gtk+-2.0`

appKubo.o: appKubo.c tiposGTK.h
	gcc -c appKubo.c `pkg-config --cflags gtk+-2.0`
