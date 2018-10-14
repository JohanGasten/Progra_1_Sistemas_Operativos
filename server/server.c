
#include "headers.h"


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
    int socket_server;
    int enlace;
    int nuevo_socket;


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

        if((nodo_pid = fork()) == 0){
            close(socket_server);

            while(1){
                recv(nuevo_socket, buffer, 1024, 0);
                if(strcmp(buffer, ":q") == 0){
                    printf("Desconexion en la direcion %s:%d\n", inet_ntoa(configuracion_nuevo_dir.sin_addr), ntohs(configuracion_nuevo_dir.sin_port));
                    break;
                }else{
                    printf("Cliente: %s\n", buffer);
                    send(nuevo_socket, buffer, strlen(buffer)+1, 0);
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
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

    iniciarSocketTCP(ip,puerto,disponibilidad);
    

    return 0;
}