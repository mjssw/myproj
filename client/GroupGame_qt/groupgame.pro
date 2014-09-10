TEMPLATE = app

CONFIG(release, debug|release) {  
DEFINES += MYRELEASE  
}  
else {  
DEFINES += MYDEBUG  
} 

QT += qml quick widgets

SOURCES += main.cpp \
	../../protomsg/msg/center.pb.cc \
    ../../protomsg/msg/login.pb.cc \
    ../../protomsg/msg/gate.pb.cc \
    ../../protomsg/msg/common.pb.cc \
    ../../protomsg/msg/tetris.pb.cc \
    ../../protomsg/msg/errno.pb.cc \
    ../../protomsg/msg/msgid.pb.cc \
    ../../protomsg/msg/public.pb.cc \
    ../../protomsg/msg/client.pb.cc \
    ../../protomsg/msg/gamemanager.pb.cc \
    ../../protomsg/msg/group.pb.cc \
    ../../third_part/protobuf-android/include/google/protobuf/descriptor.cc \
    ../../third_part/protobuf-android/include/google/protobuf/descriptor_database.cc \
    ../../third_part/protobuf-android/include/google/protobuf/descriptor.pb.cc \
    ../../third_part/protobuf-android/include/google/protobuf/dynamic_message.cc \
    ../../third_part/protobuf-android/include/google/protobuf/extension_set.cc \
    ../../third_part/protobuf-android/include/google/protobuf/extension_set_heavy.cc \
    ../../third_part/protobuf-android/include/google/protobuf/generated_message_reflection.cc \
    ../../third_part/protobuf-android/include/google/protobuf/generated_message_util.cc \
    ../../third_part/protobuf-android/include/google/protobuf/message.cc \
    ../../third_part/protobuf-android/include/google/protobuf/message_lite.cc \
    ../../third_part/protobuf-android/include/google/protobuf/reflection_ops.cc \
    ../../third_part/protobuf-android/include/google/protobuf/repeated_field.cc \
    ../../third_part/protobuf-android/include/google/protobuf/service.cc \
    ../../third_part/protobuf-android/include/google/protobuf/text_format.cc \
    ../../third_part/protobuf-android/include/google/protobuf/unknown_field_set.cc \
    ../../third_part/protobuf-android/include/google/protobuf/wire_format.cc \
    ../../third_part/protobuf-android/include/google/protobuf/wire_format_lite.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/coded_stream.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/gzip_stream.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/printer.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/tokenizer.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream_impl.cc \
    ../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream_impl_lite.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/atomicops_internals_x86_gcc.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/common.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/once.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/stringprintf.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/structurally_valid.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/strutil.cc \
    ../../third_part/protobuf-android/include/google/protobuf/stubs/substitute.cc \
    ../../third_part/protobuf-android/include/google/protobuf/compiler/importer.cc \
    ../../third_part/protobuf-android/include/google/protobuf/compiler/parser.cc \
    UserManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    qml/LoginScene.qml \
    qml/ChatView.qml \
    qml/FriendView.qml \
    qml/StartScene.qml \
    qml/MainScene.qml \
    qml/GameView.qml \
    qml/FindView.qml \
    qml/MoreView.qml \
    qml/ImageRadioBtn.qml \
    qml/GroupView.qml \
    qml/ChatMenu.qml \
    qml/GameMenu.qml \
    qml/MembersMenu.qml \
    qml/CreateGroup.qml
	
INCLUDEPATH += ./ \
	../../protomsg/msg \
	../../third_part/protobuf-android \
	../../third_part/protobuf-android/include \
	../../third_part/protobuf-android/include/google \
	../../third_part/protobuf-android/include/google/protobuf \
	../../third_part/protobuf-android/include/google/protobuf/io \
	../../third_part/protobuf-android/include/google/protobuf/stubs

HEADERS += \
    UserManager.h \
    Utils.h
