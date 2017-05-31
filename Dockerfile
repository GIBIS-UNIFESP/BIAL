FROM buildpack-deps:xenial

RUN apt update && apt install qt-sdk zlib1g-dev freeglut3-dev -y

RUN mkdir -p /usr/src/bial
WORKDIR /usr/src/bial

RUN git clone https://github.com/GIBIS-UNIFESP/BIAL
RUN mkdir bial-debug && cd bial-debug && qmake -r CONFIG+=debug .. && make -j4
RUN mkdir bial-release && cd bial-release && qmake -r CONFIG+=release .. && make -j4
