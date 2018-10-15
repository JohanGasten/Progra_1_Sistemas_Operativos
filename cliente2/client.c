
#include "headers.h"

int media_exponencial_tiempo;
int probabilidad_escritura_lectura;
char ip_servidor[25];
int *lista_paginas;

int iniciarConfig(const char *nombre){
    FILE *archivo;
    archivo = fopen(nombre,"r");
    
    if (archivo == NULL){
        printf("%s\n","El archivo ingresado no existe");
        return -1;
    }else{
        
        char line[25];
        char numero_paginas[1024];
        

        fgets(ip_servidor,25, archivo);
        fgets(numero_paginas, 1024, archivo);
        fgets(line,25, archivo);
        sscanf(line,"%d", &media_exponencial_tiempo);
        fgets(line,25, archivo);
        sscanf(line,"%d", &probabilidad_escritura_lectura);

        char *token = strtok(numero_paginas,",");
        //printf("%s",ip_servidor);
        //printf("%s",numero_paginas);
        int contador = 1;
        lista_paginas = (int *) malloc(sizeof(int));
        
        while(token != NULL){
            lista_paginas = (int *)realloc(lista_paginas,contador*sizeof(int));
            lista_paginas[contador-1] = atoi(token);
            //printf("%s\n", token); 
            token = strtok(NULL, ","); 
            contador++;
        }

        /*for (int i = 0; i < contador-1; ++i){
            printf("%d\n",lista_paginas[i]);
        }
        printf("%d\n",media_exponencial_tiempo);
        printf("%d\n",probabilidad_escritura_lectura);*/
    }
    fclose(archivo);
    return 0;
}

void iniciarSocketTCP(char *ip,int puerto){
    
    //Configuracion de la direccion de internet
    struct sockaddr_in configutacion_cliente_dir;
    char buffer[1024];
    int socket_cliente;
    int conexion;

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
        //buffer[strlen(buffer)] = EOL;
        send(socket_cliente, buffer, strlen(buffer)+1, 0);

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
            if(!strcmp(buffer, EXIT)){
                close(socket_cliente);
                printf("Desconectado del Servidor.\n");
                exit(1);
            }
        }
    }
}

void iniciarSocketUDP(int puerto){
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast Address */
    char recvString[25]; /* Buffer for received string */
    int recvStringLen;                /* Length of received string */

    

    /* Create a best-effort datagram socket using UDP */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        printf("socket() failed\n");
    }
    printf("mm\n");
    /* Construct bind structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
    broadcastAddr.sin_port = htons(puerto);      /* Broadcast port */

    /* Bind to the broadcast port */
    if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0){
        printf("bind() failed\n");
    }

    printf("m2\n");
    while(1){
        /* Receive a single datagram from the server */
        if ((recvStringLen = recvfrom(sock, recvString, recvStringLen, 0, NULL, 0)) < 0){
            printf("recvfrom() failed\n");
        }
        printf("HOla\n");
        recvString[recvStringLen] = '\0';
        printf("Received: %s\n", recvString);    /* Print the received string */
    }    
    close(sock);
    exit(0);    
}
int main(int argc, char const *argv[]){

    const char *nombre = argv[1];


    //netstat -tupln
    //sudo kill -9 PID
    char *ip ;//= "127.0.0.1";
    int puerto = 4444;
    
        
    iniciarConfig(nombre);
 
    ip = ip_servidor; 
    iniciarSocketUDP(puerto);
    //iniciarSocketTCP(ip,puerto);
   
    
    
    return 0;
}