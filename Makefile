CFLAGS = /MD

build: so-cpp.exe

so-cpp.exe: hashmap.obj main.obj
	cl $(CFLAGS) /out:so-cpp.exe hashmap.obj main.obj

main.obj: main.c main.h
	cl $(CFLAGS) /c main.c

hashmap.obj: hashmap.c hashmap.h
	cl $(CFLAGS) /c hashmap.c

clean:
	del *.obj
	del so-cpp.exe