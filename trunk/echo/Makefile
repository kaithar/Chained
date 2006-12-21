all: echo

echo: echo.c
	gcc -L../corelib -Wl,--rpath -Wl,../corelib -lchained -g -fPIC -I../includes/ -o echo echo.c 
	
clean:
	rm echo

