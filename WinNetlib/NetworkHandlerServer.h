#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


#include <vector>
#include <string>
#include <iostream>
#include "NetworkBase.h"

class NetworkHandlerServer: public NetworkBase
{
public:
	bool Init(PCSTR Port);

	bool Listen();

	bool AcceptConn();

	bool DisConnect();

	bool DefaultInitConnect();
};

