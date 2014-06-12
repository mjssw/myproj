#!/bin/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/serverdeps
for proto in `ls *.proto`;
    do ../../third_part/protobuf/lib/protoc --cpp_out=../msg $proto;
done;

