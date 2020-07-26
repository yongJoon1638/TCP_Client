#include <iostream>
#include <thread>
#include <stdio.h>
#include <cstring>
#include <Winsock2.h>
#include <Windows.h>
#include "MessageHandler.h"

#pragma comment(lib,"ws2_32")

#define PORT 4578
#define PACKET_SIZE 1024
#define SERVER_IP "211.218.13.115"

using namespace std;

MessageHandler mh;

void waitForRecv(SOCKET Socket) {   
    while (mh.getHandlerRunning()) {
        char Buffer[PACKET_SIZE];
        int recvedBytes = recv(Socket, Buffer, PACKET_SIZE, 0);
        string parsed(Buffer);
        //parsed = parsed.substr(0, recvedBytes);
        mh.recved(parsed);
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("winsock.dll error\n");
        return 1;
    }

    SOCKET hSocket;
    hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == INVALID_SOCKET) {
        printf("socket error\n");
        return 1;
    }

    SOCKADDR_IN tAddr = {};
    memset(&tAddr, 0, sizeof(tAddr));
    tAddr.sin_family = AF_INET;
    tAddr.sin_port = htons(PORT);
    tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if (connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr)) == SOCKET_ERROR) {
        printf("connect error\n");
        closesocket(hSocket);
        WSACleanup();
        return 1;
    }
    else {
        printf("Connect Success!\n===>");
    }

    mh.start();
    thread recvedthread(waitForRecv, hSocket);
    while (1) {
        char cMsg[PACKET_SIZE];
        int i = 0;
        while (1) {
            cMsg[i] = getchar();
            if (cMsg[i] == '\n') {
                cMsg[i++] = '\0';
                break;
            }   
            i++;
       }
        if (strcmp(cMsg, "exit") == 0) {
            break;
        }
       int sendBytes = send(hSocket, cMsg, i, 0);
       if (sendBytes > 0) {
           printf("%s sended(%d bytes)\n", cMsg, i);
       }
    }
    mh.stop();

    recvedthread.join();
    closesocket(hSocket);
    WSACleanup();
    return 0;
}

