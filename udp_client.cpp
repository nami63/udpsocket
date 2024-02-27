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

    double f_lat = 12.96093;
    double f_lon = 77.65333;
    double f_alt = 860.0;
    int n_pilot = 2;
    bool mode_auto = true;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    BOOL broadcastEnable = TRUE;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) == SOCKET_ERROR) {
        cerr << "Error setting broadcast option: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    destAddr.sin_port = htons(12345); // Choose any port number

    int bytesSent = sendto(sock, (char*)&f_lat, sizeof(f_lat), 0, (sockaddr*)&destAddr, sizeof(destAddr));
    bytesSent += sendto(sock, (char*)&f_lon, sizeof(f_lon), 0, (sockaddr*)&destAddr, sizeof(destAddr));
    bytesSent += sendto(sock, (char*)&f_alt, sizeof(f_alt), 0, (sockaddr*)&destAddr, sizeof(destAddr));
    bytesSent += sendto(sock, (char*)&n_pilot, sizeof(n_pilot), 0, (sockaddr*)&destAddr, sizeof(destAddr));
    bytesSent += sendto(sock, (char*)&mode_auto, sizeof(mode_auto), 0, (sockaddr*)&destAddr, sizeof(destAddr));

    if (bytesSent < 0) {
        cerr << "Error sending data: " << WSAGetLastError() << endl;
    } else {
        cout << "Data sent successfully" << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
