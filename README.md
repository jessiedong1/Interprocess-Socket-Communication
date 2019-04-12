# Interprocess-Socket-Communication


## Getting Started 
These instructions will teach you how to run my code in the Linux Command Line. Surely, you need in a Linux enoviorment...

### Run the server side first
```
Gcc -c JessieServer.c -o JessieServer.o
Gcc JessieServer.o -o JessieServer
./JessieServer 8300
Wait for client(s)
```
### Client Side
```
gcc -c JessieClient.c -o JessieClient.o
gcc JessieClient.o -o JessieClient
./JessieClient localhost 8300 (For local clients)
./JessieClient 161.31.72.212 8300 (For remote clients)
Enter  your message
```
### Sample
Localhost Clients
![LocalhostClients](https://user-images.githubusercontent.com/38058527/56055214-9adbaa00-5d1e-11e9-931d-a530ad19293e.png)


Remote Client
![RemoteClient](https://user-images.githubusercontent.com/38058527/56055223-9dd69a80-5d1e-11e9-9530-ec98e240e034.png)
