FROM ubuntu:17.04

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
COPY . /usr/src/app

RUN tools/setup-ubuntu.sh
RUN mkdir build
WORKDIR build
RUN cmake -DCMAKE_INSTALL_PREFIX=/usr ..
RUN make
RUN make install
WORKDIR /tmp
RUN rm -rf /usr/src/app

ENTRYPOINT ["/usr/bin/airmap", "daemon"]
