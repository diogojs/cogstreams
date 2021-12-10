# cogstreams

Simple server-client application made in C++, using CMake and BOOST.

## Prerequisites

- Docker

or if you want to build it from source:

- Boost 1.77
- CMake > 3.15
- GCC > 8.5

---
## Install from Docker Hub

If you have docker installed it should be pretty straightforward.

First, you can download the prebuilt image from Docker Hub:

`$ docker pull diogojs/cogstream`

Then you should run two containers, one for the Server and another for the Client, using the command:

`$ [sudo] docker run -it --expose 8042 diogojs/cogstream`


## How to Use with Docker

Once you are in a sh session into the containers, you should be inside the `app` folder that contains the executables for the client and server, and the configuration file.

Run `./cogserver` to start the server.

After that, get the IP that docker assigned to this container in its bridge network (by default). You can find the IP running one of the following commands outside the containers (in your host machine).
```
$ docker network inspect bridge
// or
$ docker container inspect <container_name> | grep IPAddress
```
Then you should be able to run the client in the other container, using:
`./cogclient --host=<IPAddress>`

After connected, you can type the messages you want to send to the server, and the server will save all your messages into unique files.

The file names follow the pattern:

`prefix_idYYYYMMddHHmmss_n`

Where `prefix` is defined by the config file; `id` is a unique identifier of each connection/client, and then the timestamp when the file was created. If the data received is bigger than the maximum file size specified in the config, we may need to create two files at the same time/second, so we add the self-incremented counter `n` to differentiate their names.


## Configuration File

You can configure the client and server parameters through the file `config.cfg`, e.g.:
```
host=localhost
port=8042
max_file_size=16 (in bytes)
file_prefix=prefixo_
connection_timeout=10 (in seconds)
```
 Instead of editing the file you can also pass the settings as command line arguments.
e.g.:

`./cogserver --port=8099 --connection_timeout=10 --max_file_size=16 --file_prefix=MY_PREFIX_`

`./cogclient --host=172.17.0.3 --port=8099`

---

## Build from source

Check if you have all the prerequisites installed on your system.
After cloning this repository and going to its directory, use the following commands to build the project:
```
$ cmake -S src -B build
$ cmake --build build
```

Two binary files should have been generated in the `build` folder.

You can run them in a similar way to what is described on the "How to Use with Docker" topic, but without worrying about the host and ports.

---

# Extra Challenge Info

It took me from 6 to 7 hours to complete the challenge. Around 4h of those actually coding, and the rest of the time setting everything up, studying the subject and documenting.

The most difficult part for me was to correctly design the message exchange between Client and Server, partly because the client behavior was not detailed on the Challenge Specification document. I believe it was on purpose because the focus should be on the server-side, but I got a little lost on time thinking about how the client should communicate and send data to the server.

The setup of the docker images and containers also took me some time studying because it's not a tool I use frequently on my current job, but it was a fun learning experience.
