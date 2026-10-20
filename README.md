# demiurge
A multi-threaded event driven server in C.

## Vision
To build a multi-threaded event driven server in C using the resilient **Reactor Pattern**. 
To be more precise it will be a *Multi-Reactor(Worker per Epoll)*. One listener thread accepts connectionsand round-robins them to worker threads, each running its own event loop.

## Wanna try it?
Here's your magic spell to get it working wizard!!
```bash
gcc -o build/demiurge src/main.c
./build/demiurge
```

Wanna try server?? Cast the above spell and here you go...
```bash
gcc -o build/client src/client.c
./build/client
```
