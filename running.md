# Commands to run 
## First install the some packages if they aren't install in your system
```
sudo apt update
```

```
sudo apt install libsctp-dev
```

```
gcc server.c -o server -lsctp
```

```
gcc client.c -o client -lsctp
```

```
./server
```
```
./client
```


