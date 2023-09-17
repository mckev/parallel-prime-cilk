#!/bin/bash

# Ref: https://www.opencilk.org/doc/users-guide/install/
# wget https://github.com/OpenCilk/opencilk-project/releases/download/opencilk/v2.0/docker-opencilk-v2.0.tar.gz
# docker load -i docker-opencilk-v2.0.tar.gz

docker run --volume .:/workspace -it opencilk:v2.0 cp -p /usr/local/lib/clang/14.0.6/lib/x86_64-unknown-linux-gnu/libopencilk.so.1 /workspace/
docker run --volume .:/workspace -it opencilk:v2.0 cp -p /usr/local/lib/clang/14.0.6/lib/x86_64-unknown-linux-gnu/libopencilk-personality-cpp.so.1 /workspace/

rm -f main
cat main.cpp
docker run --volume .:/workspace -it opencilk:v2.0 clang++ -Ofast -fopencilk -o /workspace/main /workspace/main.cpp

export LD_LIBRARY_PATH=.
time ./main
