#Bloque de variables

BANDERAS = -g -lm --std=c11
SALIDA = ejecutable

#SALIDA es el nombre final del producto compilado

#regla que se ejecuta por defecto

all:
	gcc $(BANDERAS) main.c -o $(SALIDA)

	 ./$(SALIDA)

#regla especial para limpiar

.PHONY clean:
	rm *.o $(SALIDA)
