FROM alpine:latest

COPY src /usr/src/cogstreams
WORKDIR /usr/src/cogstreams

RUN apk add --no-cache g++ musl cmake make boost boost-dev
RUN cmake -S . -B build
RUN cmake --build build
RUN mkdir app && cd app && cp ../build/cog* ./
COPY config.cfg /usr/src/cogstreams/app/
WORKDIR /usr/src/cogstreams/app
CMD [ "/bin/sh" ]
