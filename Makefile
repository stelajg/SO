CFLAGS = /MD


so-cpp.exe: hashmap.obj main.obj
	cl $(CFLAGS) /Feso-cpp.exe hashmap.obj main.obj

main.obj: main.c main.h
	cl $(CFLAGS) /Fomain.obj /c main.c

hashmap.obj: hashmap.c hashmap.h
	cl $(CFLAGS) /Fohashmap.obj /c hashmap.c

clean:
	del *.obj
	del so-cpp.exe
