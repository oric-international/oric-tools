Serial-to-TCPIP gateway program v1.0
F.Frances 2003


Introduction
------------
This is a program that relays all traffic on a serial communication port to a TCP/IP connection. The computer on which you will run this program will be herein defined as the gateway computer. It needs to have at least one serial communication port, where you will connect a serial device, be it a terminal, modem, computer, fridge, VCR, heat control, etc.
The gateway computer also needs a network interface, usually it will be an ethernet network card or a modem but you can also test the program with a pseudo interface like a loopback connection.
Depending on command line options, the gateway program either initiates a TCP/IP connection, or waits for incoming connections. Once a TCP/IP connection is established, a virtual serial link exists over this connection, so all the bytes coming from the serial port are forwarded to the remote host (encapsulated in TCP segments), and all the bytes coming from the TCP/IP connection are forwarded to the serial device.
Thus, the TCP/IP connection is completly transparent to the serial device, which thinks the remote host is directly connected through its serial link.
However, the status of the TCP/IP connection is made available to the serial device by the way of the modem control signals: DTR (Data Terminal Ready) goes active when the connection is established, and return to inactive state when the connection is closed.
Currently, the gateway program only supports one format of serial transmission : 8 data bits, no parity, one stop bit, but of course, the serial line speed can be set. I don't think 7 bits transmission or parity is needed, please correct me if I am wrong (for example, you can use 7 databits plus one parity bit, and tunnel this in a TCP stream of bytes)

Serial setup - hardware
-----------------------
Needless to say, if you want to connect something to a serial port of your gateway computer, you need the right cable. Thus you need to know the category of your serial device, is it a DTE (Data Terminal Equipment) or a DCE (Data Communication Equipment) ?
Computers and terminals are DTE, modems are DCE; for your VCR with serial interface, please check the documentation...
Since your gateway computer has DTE ports, you connect DCEs (mainly modems) with straight (direct) serial links, and DTEs (e.g computers) with so-called 'null-modem' serial links.
Usually, null-modem links are wired this way:

Tx  ----------> Rx
Rx  <---------- Tx
RTS ----------> CTS
CTS <---------- RTS
DTR ----------> DCD, DSR
DCD, DSR <----- DTR


Serial setup - permissions (Unix only)
--------------------------------------
You need to know how your serial communication ports are named on your computer. On a Linux box, they are usually named /dev/ttyS0, /dev/ttyS1, or more recently /dev/tts/0 and /dev/tts/1 but this can be different if you have a card with 4 or 8 ports...
Please note that if you want to run this program from a non-priviledged account, you might need to set the permissions of the serial port you intend to use, so that your account can open and control the serial port. This is not always as it depends on your operating system, so please check it first...

On a RedHat 7.3, ls -l /dev/ttyS0 gave me:

crw-rw---- 1 root tty    4,  64 jan 1 1970 /dev/ttyS0

On a RedHat 8.0, ls -l /dev/tts/0 replied:

crw-rw---- 1 fabrice tty  4,  64 jan 1 1970 /dev/tts/0

So, if you are not the owner of the device, you will have to either change ownership of the device (e.g. chown yourname /dev/ttyS0) or permissions (e.g. chmod o+rw /dev/ttyS0) if you are not too much concerned by security issues.

Initiating a connection to a remote server
------------------------------------------
If your serial device is a computer, you can start the gateway program on the gateway computer so that a connection is initiated to a remote host. The syntax is:

gateway <serial port> <port speed> <remote host> <remote port>

This will immediately initiate a connection to the remote host/port, and then setup the serial line at the specified baudrate and DTR will be activated, so that usual serial devices will detect the activation of either DSR or DCD, meaning that the serial link is up.

E.g.: 
(Windows:) gateway com1 9600 www.google.com 80
(Unix:) gateway/dev/ttyS0 9600 www.google.com 80

Note: The remote host can be specified by a dotted IP address (e.g. 192.168.0.1).

Waiting for a connection
------------------------
In this mode, the gateway program will accept incoming connections on a specified port, so a remote host may contact your gateway computer in order to connect to your serial device.
Beware of security issues, currently no authentication is made, so it really means everyone is granted to access your serial device. As the serial line is not shared, only one connection will be accepted at a time.

Syntax:
gateway <serial port> <port speed> <TCP port>

E.g.: 
(Windows:) gateway com2 19200 6502
(Unix:) gateway /dev/tts/0 19200 6502

In this example, once a remote host connects to your gateway computer at port 6502, the program will notify your serial device by activating the DTR line of the serial port (gateway computer's side, which usually means DSR or DCD viewed from your serial device).
When the connection is ended (either by the serial device or the remote host), the gateway program is ready again to accept another incoming connection.

Ending a connection
-------------------
The connection is terminated either by the serial device or by the remote host.
If the TCP/IP is closed by the remote host, the serial device is notified of the termination by releasing DTR state to inactive state.
If the serial device terminates its connection to the gateway computer, the TCP/IP connection is closed.

Security issues for server usage
--------------------------------
Once again, you must know that in this usage, your serial device will be available to everyone on the network, no authentication is done.
This can be acceptable on a local network, but if you also want to make your serial device available to the whole Internet, your firewall is likely to not allow it. If you still want to take the risk, you will need to open the firewall for incoming connections on the TCP port you selected for the gateway program, and then communicate this port number and the IP address or name of your gateway computer to your friends, so that they can access your serial device.

BUGS (Unix)
-----------
- Opening the serial port in blocking mode only works if the device connected is ready (it seems the Linux' open call waits for DSR being active, ie. the DTR line of the device). This means that you need to start the device before launching this program. If you start the program before the device is ready, the program will block until you start the device. This is not a problem for serial devices acting as servers, but it might be a problem for devices acting as clients.

BUGS (Windows)
--------------
- the program currently doesn't detect when the serial device terminates the serial connection (by releasing its DTR line). This shouldn't be a big problem when the device acts as a client, because usually the server will close the TCP/IP connection first. But it might a problem for devices willing to act as servers, because they won't be able to make the program close the TCP/IP connection.

TO-DO
-----
- Better handling of the modem lines, so that the serial device can issue connection-on-demand, once the gateway program is started in background.
- maybe adding a mean for the serial device to specify the remote host it wants to connect to, for example using a dialing directory and Hayes-modem compatible dialing commands...
- authentication options

DISCLAIMER
----------
I cannot be made responsible of damages resulting from the use of this program, or from any information given by this document. If you burn your computer, your fridge, your VCR, etc. when connecting them together (or even when not connecting them), it's your fault.
If you get your computer (or your fridge, your VCR, etc.) infested by pirates, virus, or anything else, because of the use of this program (or even if you don't use it), it's your fault.