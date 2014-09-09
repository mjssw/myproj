TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

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
