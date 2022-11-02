#include "Utility.h"

void Utility::makeSockAddr(sockaddr_in& addr, const ulong hostlong, const ushort port)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
#ifdef WIN32
	addr.sin_addr.S_un.S_addr = htonl(hostlong);
#else
	addr.sin_addr.s_addr = htonl(hostlong);
#endif
}