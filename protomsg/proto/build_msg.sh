#!/bin/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/serverdeps
for proto in `ls *.proto`;
    do ../../protobuf-2.5.0/lib/linux/x64/protoc --cpp_out=../msg $proto;
done;

