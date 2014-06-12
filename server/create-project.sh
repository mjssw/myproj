#!/bin/bash

PWD=`pwd`
if [ ! -f $PWD/create-project.sh ]; then echo Error!!! You should run it under base root & exit ; fi
echo "Start"

# modify the project name !!!
PROJECT_NAME=MyTestServer

#create project
echo Create config file:$PROJECT_NAME.xml
cp "$PWD/serverconfig/Template.xml" "$PWD/serverconfig/$PROJECT_NAME.xml"

echo Create project:$PROJECT_NAME
ROOT_DIR=$PWD
#echo RootDir=$ROOT_DIR
sudo mkdir $PROJECT_NAME
cp $ROOT_DIR/Template/Main.cpp $ROOT_DIR/$PROJECT_NAME/Main.cpp.tp
cp $ROOT_DIR/Template/Template.vcxproj $ROOT_DIR/$PROJECT_NAME/$PROJECT_NAME.vcxproj.tp
cp $ROOT_DIR/Template/Template.vcxproj.filters $ROOT_DIR/$PROJECT_NAME/$PROJECT_NAME.vcxproj.filters.tp
cp $ROOT_DIR/Template/TemplateClient.cpp $ROOT_DIR/$PROJECT_NAME/${PROJECT_NAME}Client.cpp.tp
cp $ROOT_DIR/Template/TemplateClient.h $ROOT_DIR/$PROJECT_NAME/${PROJECT_NAME}Client.h.tp
cp $ROOT_DIR/Template/TemplateRpcClient.cpp $ROOT_DIR/$PROJECT_NAME/${PROJECT_NAME}RpcClient.cpp.tp
cp $ROOT_DIR/Template/TemplateRpcClient.h $ROOT_DIR/$PROJECT_NAME/${PROJECT_NAME}RpcClient.h.tp
cp $ROOT_DIR/Template/makefile $ROOT_DIR/$PROJECT_NAME/makefile.tp

echo Configure all files
cd $PROJECT_NAME
for f in $(ls *.tp)
do
    len=${#f}
    filename=${f:0:len-3}
    echo $f
    #echo $filename
    #echo $ROOT_DIR/$PROJECT_NAME/$filename

	sed -r "s/Template/$PROJECT_NAME/g" $ROOT_DIR/$PROJECT_NAME/$f > $ROOT_DIR/$PROJECT_NAME/$filename
    rm -f $ROOT_DIR/$PROJECT_NAME/$f
done

echo Done!

