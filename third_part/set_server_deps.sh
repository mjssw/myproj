#!/bin/sh

DEPS_PATH="/usr/local/serverdeps"
if [ ! -d "$DEPS_PATH" ]; then
sudo mkdir "$DEPS_PATH"
fi
sudo cp ./log4cxx/lib/liblog4cxx.* "$DEPS_PATH"
sudo cp ./protobuf/lib/libprotobuf* "$DEPS_PATH"
sudo cp ./tinyxml/lib/libtinyxml* "$DEPS_PATH"
#sudo echo "$DEPS_PATH" >> /etc/ld.so.conf
