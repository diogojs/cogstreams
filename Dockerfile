FROM gcc:latest

COPY src /usr/src/cogstreams
WORKDIR /usr/src/cogstreams

RUN g++ -o cogclient main_client.cpp
CMD [ "./cogclient" ]