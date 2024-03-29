#!/usr/bin/env -S sh -c 'docker build --rm -t stdml/stdtracer:latest .'

FROM ubuntu:focal

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y build-essential cmake

WORKDIR /src
ADD . .
RUN ./configure --deb && make package
