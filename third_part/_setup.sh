#!/bin/bash
DEPS_PATH="/usr/local/serverdeps"
if [ ! -d "$DEPS_PATH" ]; then
sudo mkdir "$DEPS_PATH"
fi
sudo cp ./log4cxx/lib/liblog4cxx.* "$DEPS_PATH"
sudo cp ./protobuf/lib/libprotobuf* "$DEPS_PATH"
sudo cp ./tinyxml/lib/libtinyxml* "$DEPS_PATH"
#sudo echo "$DEPS_PATH" >> /etc/ld.so.conf

MYSQL_LIB_PATH="/usr/lib"
sudo cp ./mysql/lib/libmysqlclient.so.16.0.0 "$MYSQL_LIB_PATH"
sudo cp ./mysql/lib/libmysqlclient_r.so.16.0.0 "$MYSQL_LIB_PATH"
sudo ln -s $MYSQL_LIB_PATH/libmysqlclient_r.so.16.0.0 $MYSQL_LIB_PATH/libmysqlclient_r.so.16
sudo ln -s $MYSQL_LIB_PATH/libmysqlclient.so.16.0.0 $MYSQL_LIB_PATH/libmysqlclient.so.16
sudo ln -s $MYSQL_LIB_PATH/libmysqlclient.so.16 $MYSQL_LIB_PATH/libmysqlclient.so
sudo ln -s $MYSQL_LIB_PATH/libmysqlclient.so.16 $MYSQL_LIB_PATH/libmysqlclient
