# demiurge
A server made in c from scratch.

## Specs
The current state of the server is child per connection architecture.
The next progression is a child worker + epoll architecture like nginx.

The end goal is a multithreaded + epoll server.

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
