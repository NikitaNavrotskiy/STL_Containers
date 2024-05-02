FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    cmake \
    make \
    libgtest-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txt && make
COPY *.a /usr/lib


WORKDIR /containers

COPY . .

CMD ["make", "test_exec"]
CMD ["make", "test"]
