# demiurge
A multi-threaded event driven server in C.

## Vision
To build a multi-threaded event driven server in C using the resilient **Reactor Pattern**. 
To be more precise it will be a *Multi-Reactor(Worker per Epoll)*. One listener thread accepts connectionsand round-robins them to worker threads, each running its own event loop.

## Doubting performance
4 threads, 100 concurrent connections, and 30 seconds...
```bash
$ wrk -t4 -c100 -d30s http://127.0.0.1:8080/
Running 30s test @ http://127.0.0.1:3490
  4 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    15.08ms   95.66ms   1.67s    96.95%
    Req/Sec     4.19k     2.17k   13.28k    68.52%
  496002 requests in 30.01s, 81.83MB read
  Socket errors: connect 0, read 0, write 0, timeout 54
Requests/sec:  16525.73
Transfer/sec:      2.73MB
```

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
