#include "SocketServer.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <stdbool.h>
char message[1024];
bool authenticated = true;

void server() {
    int serverConection, clienConection, port;
    socklen_t longc;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[100];

    port = 25558;
    serverConection = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Se comprueba si hay error al asociar el puerto a la conexion
    if (bind(serverConection, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        close(serverConection);
        return;
    }
    while (authenticated) {
        sprintf(message, "");
        listen(serverConection, 3);
        printf("Port waiting %d\n", ntohs(serverAddr.sin_port));
        longc = sizeof(clientAddr);
        clienConection = accept(serverConection, (struct sockaddr *) &clientAddr, &longc);

        if (clienConection < 0) {
            while (clienConection < 0) {
                clienConection = accept(serverConection, (struct sockaddr *) &clientAddr, &longc);
            }
        }

        printf("Conectando con el client\n");

        //Se comprueba si se ha cerrado la conexion recv =0
        //Se comprueba si ha ocurrido un error recv < 0
        if (recv(clienConection, buffer, 100, 0) < 0) {
            close(serverConection);
            return;
        } else {
            printf("%s\n", buffer);
            bzero((char *) &buffer, sizeof(buffer));
            joinToGame();
            if(authenticated) {
                send(clienConection, message, 1024, 0);
            }
        }
        close(clienConection);
    }
    close(serverConection);
    return;

}


void joinToGame() {
    printf("\n");
    printf("------------------------------------------------------\n");
    printf("--------------------OBJECTS MENU----------------------\n");
    printf("------------------------------------------------------\n");
    printf("------------------------------------------------------\n");
    printf("1.\tPara ingresar una fruta:\n");
    printf("2.\tPara ingresar un fantasma:\n");
    printf("3.\tPara ingresar una pastilla:\n");
    printf("4.\tModificar la velocidad de los fantasmas:\n");
    printf("5.\tRegresar: ");
    printf("6.\tSalir: ");
    printf("  \tIngrese la accion a realizar: ");

    int option=0;
    scanf("%u", &option);

    if(option==1) {

        printf("1.\tCereza:\n");
        printf("2.\tFresa:\n");
        printf("3.\tNaranja:\n");
        printf("4.\tManzana:\n");
        printf("5.\tUvas:\n");
        int fruitType;
        printf("  \tIngrese la fruta a escoger: ");
        scanf("%u", &fruitType);
        int fruitScore;
        printf("  \tIngrese el puntaje de la fruta: ");
        scanf("%u", &fruitScore);
        sprintf(message, "fruta,%u,%u", fruitType, fruitScore);

    }
    if(option==2) {
        printf("1.\tBlinky:\n");
        printf("2.\tPinky:\n");
        printf("3.\tInky:\n");
        printf("4.\tClynde:\n");
        printf("  \tIngrese el fantasma a crear: ");

        int ghostType;
        scanf("%u", &ghostType);

        printf("\n\tIngrese velocidad:");
        int speed;
        scanf("%u", &speed);
        printf("\n\tIngrese la fila");
        int row;
        scanf("%u", &row);
        printf("\n\tIngrese la columna");
        int colum;
        scanf("%u", &colum);
        sprintf(message, "fantasma,%u,%u,%u,%u", ghostType, speed, row, colum);


    }
    if(option==3) {
        printf("\n\tIngrese la fila");
        int rowp;
        scanf("%u", &rowp);
        printf("\n\tIngrese la columna");
        int colump;
        scanf("%u", &colump);
        sprintf(message, "pastilla,%u,%u", rowp, colump);
    }
    if(option==4) {
        printf("\n\t Cambiar velocidad del fantasma\n");
        printf("1.\tBlinky:\n");
        printf("2.\tPinky:\n");
        printf("3.\tInky:\n");
        printf("4.\tClynde:\n");
        printf("  \tIngrese el fantasma a cambiar: ");
        int typeG;
        scanf("%u", &typeG);
        printf("\n\tNueva velocidad:");
        int speedG;
        scanf("%u", &speedG);
        sprintf(message, "velocidad,%u,%u", typeG, speedG);

    }
    if(option==5) {
        printf("No agrega nada\n");
        return;
    }
    if(option==6) {
        authenticated= false;
    }
    printf("Mensaje enviado: \"%s\". \n", message);
    return;

    }





void client() {
    int socketValue = 0;
    struct sockaddr_in socketServerAddr;
    char buffer[1024];
    int PORT = 25578;

    //Comprobación de un error en la creación del socket
    if ((socketValue = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        return;
    }
    socketServerAddr.sin_family = AF_INET;
    socketServerAddr.sin_port = htons(PORT);

    //Comprobación de un error en el puerto o ip incorrecta
    if(inet_pton(AF_INET, "127.0.0.1", &socketServerAddr.sin_addr)<=0)
    {
        return;
    }

    //Comprobación de un error en la conexión
    if (connect(socketValue, (struct sockaddr *)&socketServerAddr, sizeof(socketServerAddr)) < 0)
    {
        return;
    }
    joinToGame();
    send(socketValue , message , 1024, 0 );
    printf("\n");
    printf("1.\tPara ingresar objetos:\n");
    printf("2.\tPara desconectarse:\n");
    printf("  \tIngrese la accion a realizar: ");
    close(socketValue);
}

void startClient() {
    printf("------------------------------------------------------\n");
    printf("--------------------PACMAN MENU-----------------------\n");
    printf("------------------------------------------------------\n");
    printf("------------------------------------------------------\n");
    printf("1.\tPara ingresar objetos:\n");
    printf("2.\tPara desconectarse:\n");
    printf("  \tIngrese la accion a realizar: ");


    while (authenticated) {
        int option=0;
        scanf("%u", &option);
        if(option == 2) {
            authenticated= false;
            break;
        }
        if(option == 1) {
            client();
        }
    }

}