#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Inicializar Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "¡Fallo al inicializar Winsock, hermano!" << endl;
        return 1;
    }

    string server_ip;
    cout << "===========================================" << endl;
    cout << "  TERMINAL CLIENTE DE PARKING - C++        " << endl;
    cout << "===========================================" << endl;
    cout << "Ingresa la direccion IP del servidor Python (Ej: 192.168.1.50): ";
    cin >> server_ip;

    int port = 65432;

    while (true) {
        string placa;
        cout << "\nIngresa la placa de 6 digitos (Ej: ABC-123) o escribe 'SALIR': ";
        cin >> placa;

        if (placa == "SALIR" || placa == "salir") {
            break;
        }

        // Crear socket
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            cout << "¡Error al crear el socket!" << endl;
            continue;
        }

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

        // Conectar al servidor
        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            cout << "¡No se pudo conectar con el servidor! Revisa la IP y la red Wi-Fi." << endl;
            closesocket(sock);
            continue;
        }

        // Enviar la placa
        send(sock, placa.c_str(), (int)placa.length(), 0);

        // Recibir respuesta del servidor
        char buffer[1024] = {0};
        int bytes_recibidos = recv(sock, buffer, 1024, 0);
        
        if (bytes_recibidos > 0) {
            cout << "\n-------------------------------------------" << endl;
            cout << "RESPUESTA DEL SERVIDOR:" << endl;
            cout << buffer << endl;
            cout << "-------------------------------------------" << endl;
        }

        closesocket(sock);
    }

    WSACleanup();
    cout << "¡Cerrando terminal cliente. ¡SUPERRRR!" << endl;
    return 0;
}