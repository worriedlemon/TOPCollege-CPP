#ifndef SOCKETS_H

#define SOCKETS_H
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER(x) (char*)&x, sizeof(x)

#include <winsock2.h>
#include <windows.h>

#endif