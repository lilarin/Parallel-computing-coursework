FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y \
    build-essential \
    cmake \
    ca-certificates \
    g++-11

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110

RUN g++ --version

WORKDIR /app

COPY . .

RUN mkdir build

RUN cmake -DCMAKE_CXX_COMPILER=g++-11 -DCMAKE_CXX_STANDARD=17 -S . -B build

RUN cmake --build build

EXPOSE 8080

CMD ["./build/server"]
