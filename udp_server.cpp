#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed with error: " << result << endl;
        return 1;
    }


    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on any interface
    serverAddr.sin_port = htons(12345); // Use the same port as the client

    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed with error: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    double f_lat;
    double f_lon;
    double f_alt;
    int n_pilot;
    bool mode_auto;

    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    int bytesReceived = recvfrom(sock, (char*)&f_lat, sizeof(f_lat), 0, (sockaddr*)&clientAddr, &clientAddrLen);
    bytesReceived += recvfrom(sock, (char*)&f_lon, sizeof(f_lon), 0, (sockaddr*)&clientAddr, &clientAddrLen);
    bytesReceived += recvfrom(sock, (char*)&f_alt, sizeof(f_alt), 0, (sockaddr*)&clientAddr, &clientAddrLen);
    bytesReceived += recvfrom(sock, (char*)&n_pilot, sizeof(n_pilot), 0, (sockaddr*)&clientAddr, &clientAddrLen);
    bytesReceived += recvfrom(sock, (char*)&mode_auto, sizeof(mode_auto), 0, (sockaddr*)&clientAddr, &clientAddrLen);

    if (bytesReceived < 0) {
        cerr << "Error receiving data: " << WSAGetLastError() << endl;
    } else {
        cout << "Data received:" << endl;
        cout << "f_lat: " << f_lat << endl;
        cout << "f_lon: " << f_lon << endl;
        cout << "f_alt: " << f_alt << endl;
        cout << "n_pilot: " << n_pilot << endl;
        cout << "mode_auto: " << (mode_auto ? "true" : "false") << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
