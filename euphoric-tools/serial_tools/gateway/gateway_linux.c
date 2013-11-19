#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>

int server_sock=-1;
char *usage=
  "Serial-to-TCP/IP Gateway v1.0\n"
  "F.Frances 2003\n\n"
  "Usage:\n\n"
  "gateway <serial port> <port speed> <server TCP port>\n\n"
  "or\n\n"
  "gateway <serial port> <port speed> <remote host name or addr> <remote TCP port>\n\n"
  "Use first syntax when local computer waits for incoming connection, e.g.:\n"
  "gateway /dev/ttyS0 9600 6502\n\n"
  "Use second syntax when local computer connects to a remote host, e.g.:\n"
  "gateway com1: 19200 www.oric.org 6502\n\n"
;

void perror_and_quit(char *msg)
{
	perror(msg);
	exit(1);
}

void set_dtr(int fd)
{
	int status;
	ioctl(fd,TIOCMGET,&status);
	status |= TIOCM_DTR;
	ioctl(fd,TIOCMSET,&status);
}

void clear_dtr(int fd)
{
	int status;
	tcdrain(fd);
	ioctl(fd,TIOCMGET,&status);
	status &= ~TIOCM_DTR;
	ioctl(fd,TIOCMSET,&status);
}

int open_serial(char *port_name, int baudrate)
{
	int speed;
	struct termios tio;
	int serialfd = open(port_name, O_RDWR | O_NOCTTY ); 
	if (serialfd <0) perror_and_quit(port_name);
	clear_dtr(serialfd);
	switch (baudrate) {
		case 1200: speed=B1200; break;
		case 2400: speed=B2400; break;
		case 4800: speed=B4800; break;
		case 9600: speed=B9600; break;
		case 19200: speed=B19200; break;
		default: perror_and_quit("bad baudrate");
	}
				              
	bzero(&tio, sizeof(tio));
	tio.c_cflag = speed | CS8 | CREAD | HUPCL | CRTSCTS;
	tio.c_iflag = 0;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	tio.c_cc[VTIME] = 0;   /* inter-character timer unused */
	tio.c_cc[VMIN]  = 1;   /* blocking read until 1 char received */
            
	tcflush(serialfd, TCIFLUSH);
	tcsetattr(serialfd,TCSANOW,&tio);
	return serialfd;
}

void start_server(int local_port)
{
	struct sockaddr_in myaddr;

	server_sock=socket(PF_INET,SOCK_STREAM,0);
	if (server_sock<0) perror_and_quit("opening server socket");

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

int connection(char *host_name, int port)
{
	int sockfd;
	struct sockaddr_in his_addr;
	struct hostent *hp=gethostbyname(host_name);

	if (hp==NULL) perror_and_quit("bad remote address or name");
	his_addr.sin_family=AF_INET;
	bcopy((char *)hp->h_addr,(char *)&his_addr.sin_addr.s_addr,4);
	his_addr.sin_port=htons(port);

	sockfd=socket(PF_INET,SOCK_STREAM,0);
	if (sockfd<0) perror_and_quit("opening stream socket");

	if (connect(sockfd,(struct sockaddr *)&his_addr,sizeof his_addr)<0)
		perror_and_quit("connecting stream socket");
	printf("Connection successful\n");

	return sockfd;
}

void gateway(int sockfd, int serialfd)
{
	int c;
	int maxfd=(serialfd>sockfd)?serialfd:sockfd;
	fd_set fdset;
		
	for (;;) {
		FD_ZERO(&fdset);
		FD_SET(serialfd,&fdset);
		FD_SET(sockfd,&fdset);
		select(maxfd+1,&fdset,NULL,NULL,NULL);
		
		if (FD_ISSET(sockfd,&fdset)) {
			if (read(sockfd,&c,1)<=0) {
				printf("Remote host closed connection\n");
				return;
			}
			write(serialfd,&c,1);
		}
		if (FD_ISSET(serialfd,&fdset)) {
			if (read(serialfd,&c,1)<=0) {
				printf("Closing connection\n");
				return;
			}
			write(sockfd,&c,1);
		}
	}
}

int main(int argc, char *argv[])
{
	int server= (argc==4);
	int serialfd, sockfd;
	int tcp_port=0, baudrate=0;
	char *serial_port;

	if (argc<4 || argc>5) {
		printf(usage);
		exit(0);
	}
	serial_port=argv[1];
	sscanf(argv[2],"%d",&baudrate);
	sscanf(argv[argc-1],"%d",&tcp_port);

	serialfd=open_serial(serial_port,baudrate);

	if (server) {
		start_server(tcp_port);
		for (;;) {
			sockfd=wait_connection();
			set_dtr(serialfd);
			gateway(sockfd,serialfd);
			clear_dtr(serialfd);
			close(sockfd);
		}
	} else {
		char *remote_host=argv[3];
		sockfd=connection(remote_host,tcp_port);
		set_dtr(serialfd);
		gateway(sockfd,serialfd);
		clear_dtr(serialfd);
		close(sockfd);
	}
	
	return 0;
}
