#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#include "DataFIFO.h"
#include "CommunicationProtocol.h"

class StatusDataServer {
public:
    static const int bufsize = 256;
    StatusFIFO *_fifo;
    int client, server;
    bool isExit = false;
    bool closing = false;
    char buffer[bufsize];
    unsigned int _portNum;
    struct sockaddr_in server_addr;
    socklen_t size;

    StatusDataServer(StatusFIFO *fifo, unsigned int portNum) {
        _fifo = fifo;
        _portNum = portNum;

        // Init socket
        client = socket(AF_INET, SOCK_STREAM, 0);
        if (client < 0) {
            cout << "(Status) Error establishing connection." << endl;
            exit(1);
        }
        cout << "(Status) Server Socket connection created..." << endl;

        // Configure server address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htons(INADDR_ANY);
        server_addr.sin_port = htons(portNum);

        // Binding socket
        if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            cout << "(Status) Error binding socket..." << endl;
            exit(1);
        }
        size = sizeof(server_addr);

        cout << "(Status) Looking for clients..." << endl;

        // Listening socket
        listen(client, 2);
    }

    void run_server() {
        // Accept client
        server = accept(client, (struct sockaddr*)&server_addr, &size);
        if (server < 0) {
            cout << "(Status) Error on accepting client..." << endl;
            exit(1);
        }

        HealthData newPacket;

        if (server > 0) {
            strcpy(buffer, "Server connected...\n");
            send(server, buffer, bufsize, 0);
            cout << "(Status) Connected with client..." << endl;

            do {
                unsigned int recvN = recv(server, buffer, bufsize, 0);

                if (strcmp(buffer, "SendPacket\n") == 0) {
                    // cout << "(Status) Receiving packet" << endl;
                    strcpy(buffer, "Ok\n");
                    send(server, buffer, bufsize, 0);

                    int recvN = recv(server, (uint8_t*)&newPacket, sizeof(HealthData), 0);
                    // cout << "(Status) Packet received" << endl;
                    if (recvN > 0) {
                        _fifo->write(newPacket);
                    }

                } else if (strcmp(buffer, "RequestUpdate\n") == 0) {
                    if (!closing) {
                        strcpy(buffer, "Nominal\n");
                        send(server, buffer, bufsize, 0);
                    } else {
                        strcpy(buffer, "Close\n");
                        send(server, buffer, bufsize, 0);
                        isExit = true;
                    }
                }

            } while (!isExit);

            cout << "(Status) Connection terminated..." << endl;
            cout << "(Status) Goodbye..." << endl;
        }

        close(client);
    }

    void close_server() {
        closing = true;
    }
};
