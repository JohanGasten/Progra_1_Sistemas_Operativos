#ifndef HILOS_H
#define HILOS_H

struct Auto {
  int direccion; //0 o 1, viene del este o el oeste
  int velocidad;
};

struct Ambulancia {
  int direccion;
  int velocidad;
};

struct hilos_random_parms
{
    int randi;
    int index;
};

//lista de la autosEste y autosOeste?
struct Auto *autosEste;
struct Auto *autosOeste;
struct Ambulancia *ambulancias;

void* hilos_random(void* parameters);
int crearHilos();

#endif
