LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)
#LOCAL_MODULE := static 
#LOCAL_SRC_FILES := libprotobuf.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := game_shared
LOCAL_MODULE_FILENAME := libgame
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti
LOCAL_CFLAGS += -std=c++11
LOCAL_CXXFLAGS := -DHAVE_PTHREADS
LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Block.cpp \
                   ../../Classes/GameCell.cpp \
                   ../../Classes/GameList.cpp \
                   ../../Classes/GameManager.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/CursorTextControl.cpp \
                   ../../Classes/LoginScene.cpp \
                   ../../Classes/NetClient.cpp \
                   ../../Classes/NetManager.cpp \
                   ../../Classes/Room.cpp \
                   ../../Classes/utils.cpp \
                   ../../Classes/TetrisScene.cpp \
                   ../../Classes/TetrisRoomScene.cpp \
                   ../../Classes/TetrisBattleScene.cpp \
                   ../../Classes/netlib/mtwist.cpp \
                   ../../Classes/netlib/Event.cpp \
                   ../../Classes/netlib/Lock.cpp \
                   ../../Classes/netlib/Thread.cpp \
                   ../../Classes/protobuf/center.pb.cc \
                   ../../Classes/protobuf/login.pb.cc \
                   ../../Classes/protobuf/gate.pb.cc \
                   ../../Classes/protobuf/common.pb.cc \
                   ../../Classes/protobuf/tetris.pb.cc \
                   ../../Classes/protobuf/errno.pb.cc \
                   ../../Classes/protobuf/msgid.pb.cc \
                   ../../Classes/protobuf/public.pb.cc \
                   ../../Classes/protobuf/client.pb.cc \
                   ../../Classes/google/protobuf/descriptor.cc \
                   ../../Classes/google/protobuf/descriptor_database.cc \
                   ../../Classes/google/protobuf/descriptor.pb.cc \
                   ../../Classes/google/protobuf/dynamic_message.cc \
                   ../../Classes/google/protobuf/extension_set.cc \
                   ../../Classes/google/protobuf/extension_set_heavy.cc \
                   ../../Classes/google/protobuf/generated_message_reflection.cc \
                   ../../Classes/google/protobuf/generated_message_util.cc \
                   ../../Classes/google/protobuf/message.cc \
                   ../../Classes/google/protobuf/message_lite.cc \
                   ../../Classes/google/protobuf/reflection_ops.cc \
                   ../../Classes/google/protobuf/repeated_field.cc \
                   ../../Classes/google/protobuf/service.cc \
                   ../../Classes/google/protobuf/text_format.cc \
                   ../../Classes/google/protobuf/unknown_field_set.cc \
                   ../../Classes/google/protobuf/wire_format.cc \
                   ../../Classes/google/protobuf/wire_format_lite.cc \
                   ../../Classes/google/protobuf/io/coded_stream.cc \
                   ../../Classes/google/protobuf/io/gzip_stream.cc \
                   ../../Classes/google/protobuf/io/printer.cc \
                   ../../Classes/google/protobuf/io/tokenizer.cc \
                   ../../Classes/google/protobuf/io/zero_copy_stream.cc \
                   ../../Classes/google/protobuf/io/zero_copy_stream_impl.cc \
                   ../../Classes/google/protobuf/io/zero_copy_stream_impl_lite.cc \
                   ../../Classes/google/protobuf/stubs/atomicops_internals_x86_gcc.cc \
                   ../../Classes/google/protobuf/stubs/common.cc \
                   ../../Classes/google/protobuf/stubs/once.cc \
                   ../../Classes/google/protobuf/stubs/stringprintf.cc \
                   ../../Classes/google/protobuf/stubs/structurally_valid.cc \
                   ../../Classes/google/protobuf/stubs/strutil.cc \
                   ../../Classes/google/protobuf/stubs/substitute.cc
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../Classes/netlib \
$(LOCAL_PATH)/../../Classes/protobuf \
$(LOCAL_PATH)/../../Classes/google \
$(LOCAL_PATH)/../../Classes/google/protobuf \
$(LOCAL_PATH)/../../Classes/google/protobuf/io \
$(LOCAL_PATH)/../../Classes/google/protobuf/stubs

LOCAL_LDLIBS += -llog -lgcc -pthread

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
