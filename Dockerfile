FROM alpine:latest

COPY src /usr/src/cogstreams
WORKDIR /usr/src/cogstreams

RUN apk add --no-cache g++ musl cmake make boost boost-dev
RUN cmake -S . -B build
RUN cmake --build build
CMD [ "./build/cogserver -h" ]