#define _POSIX_
#include <windows.h>
#include <stdlib.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <winsock.h>

int server_sock=-1, sockfd;
HANDLE hComm,in_thread,out_thread;

char *usage=
  "Serial-to-TCP/IP Gateway v1.0\n"
  "F.Frances 2003\n\n"
  "Usage:\n\n"
  "gateway <serial port> <port speed> <server TCP port>\n\n"
  "or\n\n"
  "gateway <serial port> <port speed> <remote host name or addr> <remote TCP port>\n\n"
  "Use first syntax when local computer waits for incoming connection, e.g.:\n"
  "gateway com1 9600 6502\n\n"
  "Use second syntax when local computer connects to a remote host, e.g.:\n"
  "gateway com1 19200 www.oric.org 6502\n\n"
;

void perror_and_quit(char *msg)
{
	printf("%s: %d\n",msg,WSAGetLastError());
	exit(1);
}


void set_dtr()
{
	EscapeCommFunction(hComm,SETDTR);
}

void clear_dtr()
{
	EscapeCommFunction(hComm,CLRDTR);
}

void open_serial(char *port_name, int baudrate)
{
	DCB dcb;
	COMMTIMEOUTS timeouts;
	hComm=CreateFile(port_name,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
	if (hComm==INVALID_HANDLE_VALUE) perror_and_quit(port_name);

	FillMemory(&dcb,sizeof(dcb),0);
	if (!GetCommState(hComm,&dcb))
		perror_and_quit("cannot get serial status");

	clear_dtr();
	switch (baudrate) {
		case 1200: dcb.BaudRate=CBR_1200; break;
		case 2400: dcb.BaudRate=CBR_2400; break;
		case 4800: dcb.BaudRate=CBR_4800; break;
		case 9600: dcb.BaudRate=CBR_9600; break;
		case 19200: dcb.BaudRate=CBR_19200; break;
		case 38400: dcb.BaudRate=CBR_38400; break;
		case 57600: dcb.BaudRate=CBR_57600; break;
		case 115200: dcb.BaudRate=CBR_115200; break;
		default: perror_and_quit("bad baudrate");
	}
	dcb.fParity=FALSE;
	dcb.fOutxCtsFlow=TRUE;
	dcb.fOutxDsrFlow=FALSE;
	dcb.fDtrControl=DTR_CONTROL_ENABLE;
	dcb.fTXContinueOnXoff=TRUE;
	if (!SetCommState(hComm,&dcb))
		perror_and_quit("setting comm state");

	timeouts.ReadIntervalTimeout=0;
	timeouts.ReadTotalTimeoutConstant=300000; // 5 mins
	timeouts.ReadTotalTimeoutMultiplier=0;
	timeouts.WriteTotalTimeoutConstant=30000; // 30 seconds
	timeouts.WriteTotalTimeoutMultiplier=0;
	SetCommTimeouts(hComm,&timeouts);
}

void start_server(short local_port)
{
	struct sockaddr_in myaddr;

	server_sock=socket(PF_INET,SOCK_STREAM,0);
	if (server_sock==INVALID_SOCKET) perror_and_quit("opening server socket");

	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr=INADDR_ANY;
	myaddr.sin_port=htons(local_port);
	if (bind(server_sock,(struct sockaddr *)&myaddr,sizeof myaddr)) 
		perror_and_quit("binding server socket");
	listen(server_sock,1);
}

int wait_connection()
{
	int sockfd;
	struct hostent *hp;
	struct sockaddr_in his_addr;
	int addr_length=sizeof his_addr;
	unsigned char *addr= (unsigned char *)&his_addr.sin_addr;

	printf("Waiting for connection...\n");
	sockfd=accept(server_sock,(struct sockaddr *)&his_addr,&addr_length);
	if (sockfd==-1)
		perror_and_quit("accept");
	printf("Connection accepted\n");
	hp=gethostbyaddr(addr,4,AF_INET);
	printf("Remote host: ");
	if (hp && hp->h_name) printf("%s\n",hp->h_name);
	else printf("%d.%d.%d.%d\n",addr[0],addr[1],addr[2],addr[3]);
	return sockfd;
}

int connection(char *host_name, short port)
{
	int sockfd;
	struct sockaddr_in his_addr;
	struct hostent *hp=gethostbyname(host_name);

	if (hp==NULL) perror_and_quit("bad remote address or name");

	his_addr.sin_family=AF_INET;
	memcpy(&his_addr.sin_addr,hp->h_addr,4);
	his_addr.sin_port=htons(port);

	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if (sockfd==INVALID_SOCKET) perror_and_quit("opening stream socket");

	if (connect(sockfd,(struct sockaddr *)&his_addr,sizeof his_addr)==SOCKET_ERROR) {
		printf("Cannot connect: ");
		switch (WSAGetLastError()) {
		case WSANOTINITIALISED: printf("not initialised\n"); break;
		case WSAENETDOWN: printf("net down\n"); break;
		case WSAEADDRINUSE: printf("addr in use\n"); break;
		case WSAEINTR: printf("interrupted\n"); break;
		case WSAEINPROGRESS: printf("in progress\n"); break;
		case WSAEALREADY: printf("already\n"); break;
		case WSAEADDRNOTAVAIL: printf("addr not available\n"); break;
		case WSAEAFNOSUPPORT: printf("af not supported\n"); break;
		case WSAECONNREFUSED: printf("connection refused\n"); break;
		case WSAEFAULT: printf("fault\n"); break;
		case WSAEINVAL: printf("invalid\n"); break;
		case WSAEISCONN: printf("already connected\n"); break;
		case WSAENETUNREACH: printf("network unreachable\n"); break;
		case WSAENOBUFS: printf("no buffer space\n"); break;
		case WSAENOTSOCK: printf("not a socket\n"); break;
		case WSAETIMEDOUT: printf("timed out\n"); break;
		case WSAEWOULDBLOCK: printf("would block\n"); break;
		case WSAEACCES: printf("access\n"); break;
		default: printf("%d\n",WSAGetLastError()); break;
		}
		exit(1);
	}
	printf("Connection successful\n");

	return sockfd;
}

void serial_to_TCPIP()
{
	char c;
	int bytes_read=0;
	for (;;) {
		if (ReadFile(hComm,&c,1,&bytes_read,NULL)){
			if (bytes_read==1)
				if (send(sockfd,&c,1,0)<=0) {
					printf("Cannot send: remote host closed connection\n");
					closesocket(sockfd);
					clear_dtr();
					TerminateThread(in_thread,0);
					return;
				}
		} else {
			printf("Cannot read serial port\n");
			clear_dtr();
			closesocket(sockfd);
			TerminateThread(in_thread,0);
			return;
		}
	}
}

void TCPIP_to_serial()
{
	char c;
	int bytes_written;
	for (;;) {
		if (recv(sockfd,&c,1,0)<=0) {
			printf("Cannot read: remote host closed connection\n");
			clear_dtr();
			closesocket(sockfd);
			TerminateThread(out_thread,0);
			return;
		}
		bytes_written=0;
		while (bytes_written<1) {
			if (!WriteFile(hComm,&c,1,&bytes_written,NULL)) {
				printf("Can't write to serial port\n");
				clear_dtr();
				closesocket(sockfd);
				TerminateThread(out_thread,0);
				return;
			}
		}
	}
}

void gateway()
{
	DWORD id1,id2;
	in_thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TCPIP_to_serial,NULL,0,&id1);
	out_thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)serial_to_TCPIP,NULL,0,&id2);
	if (in_thread==NULL || out_thread==NULL) {
		printf("Cannot create thread\n");
		exit(1);
	}
	WaitForSingleObject(in_thread,INFINITE);
	WaitForSingleObject(out_thread,INFINITE);
}

void init_windows_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	if (WSAStartup( wVersionRequested, &wsaData )) {
		printf("Cannot initialize Winsock\n");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	int server= (argc==4);
	int baudrate=0;
	short tcp_port=0;
	char *serial_port;

	if (argc<4 || argc>5) {
		printf(usage);
		exit(0);
	}
	serial_port=argv[1];
	sscanf(argv[2],"%d",&baudrate);
	sscanf(argv[argc-1],"%d",&tcp_port);


	init_windows_socket();
	open_serial(serial_port,baudrate);
	if (server) {
		start_server(tcp_port);
		for (;;) {
			sockfd=wait_connection();
			set_dtr();
			gateway();
		}
	} else {
		char *remote_host=argv[3];
		sockfd=connection(remote_host,tcp_port);
		set_dtr();
		gateway();
	}
	return 0;
}
