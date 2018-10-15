
#include "headers.h"

struct InfoAdd{
    char *ip;
    int puerto;
} *TablaClientes;

int nuevo_socket;
int socket_server;
int len_tabla_clientes;
char *direccion;
int puerto;
//25 hilos
pthread_t t[25];
int indiceHilos;

void iniciarTablaClientes(){
    TablaClientes = (struct InfoAdd *) malloc(sizeof(struct InfoAdd));
    len_tabla_clientes = 0;
}

void agregarCliente(char *ip,int puerto){
    printf("%d\n", len_tabla_clientes);
    len_tabla_clientes++;
    TablaClientes = (struct InfoAdd *) realloc(TablaClientes,len_tabla_clientes*sizeof(struct InfoAdd));
    TablaClientes[len_tabla_clientes-1].ip = ip;
    TablaClientes[len_tabla_clientes-1].puerto = puerto;
    printf("%d\n", len_tabla_clientes);
}

void removerCliente(int puerto){
    int indice = -1;
    for (int i = 0; i < len_tabla_clientes; i++) {
        if(TablaClientes[i].puerto == puerto){
          len_tabla_clientes --;
          indice = i;
          printf("%d\n",len_tabla_clientes );
          printf("este cliente sera removido\n");
          break;
        }
    }
    if(len_tabla_clientes > 1 || indice != (len_tabla_clientes-1)){
        for (int j = indice; j < (len_tabla_clientes-1); j++) {
            TablaClientes[j].ip = TablaClientes[j+1].ip;
            TablaClientes[j].puerto = TablaClientes[j+1].puerto;
        }
        TablaClientes = (struct InfoAdd *) realloc(TablaClientes,len_tabla_clientes*sizeof(struct InfoAdd));
    }else{
        TablaClientes = (struct InfoAdd *) realloc(TablaClientes,(len_tabla_clientes)*sizeof(struct InfoAdd));
    }
    printf("%d\n", len_tabla_clientes);
}

void imprimirClientes(){
    printf("-------------------------\n" );
    printf("|\tCliente\t|\tPuerto|\n");
    for (int i = 0; i < len_tabla_clientes; ++i)
    {
        printf("|\t%s\t|\t%d|\n",TablaClientes[i].ip,TablaClientes[i].puerto);
    }
    printf("-------------------------\n" );
}

void *funcionCliente(){
  char bufffer[1024];
  int socket_hilo = nuevo_socket;
  char * addr = direccion;
  int puertoHilo = puerto;
  int ind = indiceHilos;

  close(socket_server);

  while(1){
      recv(socket_hilo, bufffer, 1024, 0);
      if(strcmp(bufffer, ":q") == 0){
          printf("Desconexion en la direcion %s:%d\n", addr, puertoHilo);
          break;
      }else{
          printf("Cliente: %s\n", bufffer);
          send(socket_hilo, bufffer, strlen(bufffer)+1, 0);
          bzero(bufffer, sizeof(bufffer));
      }
  }
  //removerCliente(puertoHilo);
  imprimirClientes();
  pthread_cancel(t[ind]);
}

void *recibirEntrada(){
    char buffer[1024];
    while(1){
        printf("Server: ");
        scanf("%s", &buffer[0]);
        //broadcast
        //send(nuevo_socket, buffer, strlen(buffer), 0);
    }
}

void iniciarSocketTCP(char *ip,int puerto,int disponibilidad){

    int enlace;

    struct sockaddr_in configutacion_servidor_dir;
    struct sockaddr_in configuracion_nuevo_dir;

    socklen_t len_dir;
    pid_t nodo_pid;
    char buffer[1024];

    printf("\nProgra 1 Sistemas Operativos\n\n");
    printf("Iniciando Servidor...\n");

    socket_server = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_server < 0){
        printf("Error en la conexion\n");
        exit(1);
    }

    memset(&configutacion_servidor_dir, EOL, sizeof(configutacion_servidor_dir));
    configutacion_servidor_dir.sin_family = AF_INET;
    configutacion_servidor_dir.sin_port = htons(puerto);
    configutacion_servidor_dir.sin_addr.s_addr = inet_addr(ip);

    enlace = bind(socket_server, (struct sockaddr*)&configutacion_servidor_dir, sizeof(configutacion_servidor_dir));

    if(enlace < 0){
        printf("Error al establecer la comunicacion.\n");
        exit(1);
    }

    printf("Puerto: %d\n", puerto);

    if(!listen(socket_server, disponibilidad)){
        printf("Escuchando...\n");
    }else{
        printf("Error al intentar hacer la conexion.\n");
    }

    //pthread_t hiloScan;
    //pthread_create(&hiloScan, NULL, &recibirEntrada, NULL);

    while(1){
        //scanf("%s", &buffer[0]);

        nuevo_socket = accept(socket_server, (struct sockaddr*)&configuracion_nuevo_dir, &len_dir);
        if(nuevo_socket < 0){
            exit(1);
        }

        printf("Conexion aceptada de la direccion %s:%d\n", inet_ntoa(configuracion_nuevo_dir.sin_addr), ntohs(configuracion_nuevo_dir.sin_port));
        agregarCliente(inet_ntoa(configuracion_nuevo_dir.sin_addr), ntohs(configuracion_nuevo_dir.sin_port));
        imprimirClientes();
        direccion = inet_ntoa(configuracion_nuevo_dir.sin_addr);
        puerto = ntohs(configuracion_nuevo_dir.sin_port);
        //if(!(nodo_pid = fork())){

        pthread_create( &t[indiceHilos], NULL, &funcionCliente, NULL); //Default Attributes
        //pthread_create(&hiloCliente, NULL, &funcionCliente, NULL);
        //}
        indiceHilos++;
    }

    close(nuevo_socket);
}

void iniciarSocketUDP(){
    printf("HOla\n");
}

int main(){

    char *ip = "127.0.0.1";
    int puerto = 4444;
    int disponibilidad = 10;

    iniciarTablaClientes();
    iniciarSocketTCP(ip,puerto,disponibilidad);


    return 0;
}
