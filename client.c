
#include "headers.h"

int main(){
    //netstat -tupln
    //sudo kill -9 PID
    char *ip = "127.0.0.1";
    int puerto = 4444;
    int socket_cliente;
    int conexion;

    //Configuracion de la direccion de internet
    struct sockaddr_in configutacion_cliente_dir;
    char buffer[1024];

    printf("Estableciendo conexion.\n");

    //Iniciar socket domain: Internet domain = constante AF_INET,
    // type: Stream socket , protocol: 0 por decfecto es TCP
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);


    if(socket_cliente < 0){
        printf("Error de conexion\n");
        exit(1);
    }


    /*
    * Asicnamos memoria al struct configutacion_cliente_dir
    * sin_family: AF_INET
    * sin_port: puerto
    * sin_addr: ip
    */
    memset(&configutacion_cliente_dir, EOL, sizeof(configutacion_cliente_dir));
    configutacion_cliente_dir.sin_family = AF_INET;
    configutacion_cliente_dir.sin_port = htons(puerto);
    configutacion_cliente_dir.sin_addr.s_addr = inet_addr(ip);

    //Establecemos conexion
    conexion = connect(socket_cliente, (struct sockaddr*)&configutacion_cliente_dir, sizeof(configutacion_cliente_dir));
    if(conexion < 0){
        printf("Error en la conexion.\n");
        exit(1);
    }
    printf("Conexion establecida.\n");

    while(1){
        // escribir
        printf("Cliente: ");
        scanf("%s", &buffer[0]);

        send(socket_cliente, buffer, strlen(buffer), 0);

        if(!strcmp(buffer, EXIT)){
            close(socket_cliente);
            printf("Desconectado del Servidor.\n");
            exit(1);
        }

        // Leer
        if(recv(socket_cliente, buffer, 1024, 0) < 0){
            printf("Error al recivir paquetes.\n");
        }else{
            printf("Server: %s\n", buffer);
        }
    }

    return 0;
}
