#Este es el archivo de makefile para (NOMBRE DEL ARCHIVO PRINCIPAL)

plantilla.exe: appKubo.o funCkubo.o funGTKkubo.o
	gcc appKubo.o funCkubo.o funGTKkubo.o -o appKubo.exe `pkg-config --libs gtk+-2.0`

funGTK.o: funGTKkubo.c tiposGTK.h
	gcc -c funGTKkubo.c `pkg-config --cflags gtk+-2.0`

funC.o: funCkubo.c tiposGTK.h
	gcc -c funCkubo.c `pkg-config --cflags gtk+-2.0`

app.o: appKubo.c tiposGTK.h
	gcc -c appKubo.c `pkg-config --cflags gtk+-2.0`
