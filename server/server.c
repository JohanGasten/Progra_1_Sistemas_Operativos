
#include "headers.h"

struct InfoAdd{
    char *ip;
    int puerto;
} *TablaClientes;


int len_tabla_clientes;

void iniciarTablaClientes(){
    TablaClientes = (struct InfoAdd *) malloc(sizeof(struct InfoAdd));
    len_tabla_clientes = 0;
}
void agregarCliente(char *ip,int puerto){
    len_tabla_clientes++;
    TablaClientes = (struct InfoAdd *)realloc(TablaClientes,len_tabla_clientes*sizeof(struct InfoAdd));
    TablaClientes[len_tabla_clientes-1].ip = ip;
    TablaClientes[len_tabla_clientes-1].puerto = puerto;
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
    configutacion_servidor_dir.sin_addr.s_addr = INADDR_ANY;//inet_addr(ip);

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
        if(!(nodo_pid = fork())){
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

void iniciarSocketUDP(int puerto){

    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    
    int sendStringLen;       /* Length of string to broadcast */

    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    


    char *sendString = "Johan esta el broadcast";             /* string to broadcast */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        printf("socket() failed\n");
    }
    printf("Socket\n");
    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) < 0){
        printf("Error setsockopt()\n");
    }
    printf("setsockopt\n");
    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr("192.168.1.255");/* Broadcast IP address */
    broadcastAddr.sin_port = htons(puerto);         /* Broadcast port */

    sendStringLen = strlen(sendString);  /* Find length of sendString */

    while(1){
         /* Broadcast sendString in datagram to clients every 3 seconds*/
         if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) != sendStringLen){
             printf("sendto error\n");
         }
         printf("Tiempo\n");
        sleep(3);   /* Avoids flooding the network */
    }
    /* NOT REACHED */

}
int main(){

    char *ip = "127.0.0.1";
    int puerto = 4444;
    int disponibilidad = 10;
    iniciarTablaClientes();
    iniciarSocketUDP(puerto);
    //iniciarSocketTCP(ip,puerto,disponibilidad);
    

    return 0;
}