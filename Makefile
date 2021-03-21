CFLAGS = /MD


build: main.obj hashmap.obj
	cl $(CFLAGS) /Feso-cpp.exe main.obj hashmap.obj

main.obj: main.c main.h
	cl $(CFLAGS) /c /Fomain.obj  main.c

hashmap.obj: hashmap.c hashmap.h
	cl $(CFLAGS) /c /Fohashmap.obj hashmap.c

clean:
	del *.obj
	del so-cpp.exe
