LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := groupgame_shared

LOCAL_MODULE_FILENAME := libgroupgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/utils.cpp \
				   ../../Classes/gamemsg/MsgGroupModule.cpp \
				   ../../Classes/gamemsg/MsgLoginModule.cpp \
				   ../../Classes/gamemsg/MsgGameModule.cpp \
				   ../../Classes/lua/DynamicProto.cpp \
				   ../../Classes/lua/ExportProtobuf.cpp \
				   ../../Classes/net/GameClient.cpp \
				   ../../Classes/net/GroupClient.cpp \
				   ../../Classes/net/LoginClient.cpp \
				   ../../Classes/net/NetManager.cpp \
				   ../../Classes/net/ProtoClientBase.cpp \
				   ../../Classes/net/RegisterClient.cpp \
				   ../../Classes/scene/LoginScene.cpp \
				   ../../Classes/scene/MainScene.cpp \
				   ../../Classes/scene/SceneManager.cpp \
				   ../../Classes/user/Basic.cpp \
				   ../../Classes/user/GameInfo.cpp \
				   ../../Classes/user/Group.cpp \
				   ../../Classes/user/GroupManager.cpp \
				   ../../Classes/user/GroupMember.cpp \
				   ../../Classes/user/UserManager.cpp \
				   ../../Classes/user/ViewData.cpp \
				   ../../Classes/user/GameRoomInfo.cpp \
				   ../../Classes/res/ResManager.cpp \
				   ../../Classes/view/AskJoinGroupPopLayer.cpp \
				   ../../Classes/view/ChatTableView.cpp \
				   ../../Classes/view/CreateGroupPopLayer.cpp \
				   ../../Classes/view/InviteMemberPopLayer.cpp \
				   ../../Classes/view/MemberListPopLayer.cpp \
				   ../../Classes/view/MyEditBox.cpp \
				   ../../Classes/view/MyLoadingItem.cpp \
				   ../../Classes/view/MyMenuItemImage.cpp \
				   ../../Classes/view/MyRadioButton.cpp \
				   ../../Classes/view/MyTableView.cpp \
				   ../../Classes/view/PopLayerBase.cpp \
				   ../../Classes/view/RegisterPopLayer.cpp \
				   ../../Classes/view/CreateGamePopLayer.cpp \
				   ../../../../../sglib/Event.cpp \
                   ../../../../../sglib/Lock.cpp \
                   ../../../../../sglib/Thread.cpp \
                   ../../../../../sglib/Log.cpp \
				   ../../../../../protomsg/msg/center.pb.cc \
                   ../../../../../protomsg/msg/login.pb.cc \
                   ../../../../../protomsg/msg/gate.pb.cc \
                   ../../../../../protomsg/msg/common.pb.cc \
                   ../../../../../protomsg/msg/tetris.pb.cc \
                   ../../../../../protomsg/msg/errno.pb.cc \
                   ../../../../../protomsg/msg/msgid.pb.cc \
                   ../../../../../protomsg/msg/public.pb.cc \
                   ../../../../../protomsg/msg/client.pb.cc \
				   ../../../../../protomsg/msg/gamemanager.pb.cc \
				   ../../../../../protomsg/msg/group.pb.cc \
				   ../../../../../third_part/protobuf-android/include/google/protobuf/descriptor.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/descriptor_database.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/descriptor.pb.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/dynamic_message.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/extension_set.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/extension_set_heavy.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/generated_message_reflection.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/generated_message_util.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/message.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/message_lite.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/reflection_ops.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/repeated_field.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/service.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/text_format.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/unknown_field_set.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/wire_format.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/wire_format_lite.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/coded_stream.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/gzip_stream.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/printer.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/tokenizer.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream_impl.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/io/zero_copy_stream_impl_lite.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/atomicops_internals_x86_gcc.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/common.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/once.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/stringprintf.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/structurally_valid.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/strutil.cc \
                   ../../../../../third_part/protobuf-android/include/google/protobuf/stubs/substitute.cc \
				   ../../../../../third_part/protobuf-android/include/google/protobuf/compiler/importer.cc \
				   ../../../../../third_part/protobuf-android/include/google/protobuf/compiler/parser.cc

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
$(LOCAL_PATH)/../../../../../sglib \
$(LOCAL_PATH)/../../../../../protomsg/msg \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android/include \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android/include/google \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android/include/google/protobuf \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android/include/google/protobuf/io \
$(LOCAL_PATH)/../../../../../third_part/protobuf-android/include/google/protobuf/stubs \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv/include \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv/libcharset \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv/lib \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv/licharset/include \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv/srclib \
$(LOCAL_PATH)/../../../../cocos2d-x/iconv

LOCAL_STATIC_LIBRARIES := curl_static_prebuilt

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += iconv_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static


include $(BUILD_SHARED_LIBRARY)

#$(call import-add-path,$(LOCAL_PATH)/../../../../cocos2d-x/iconv)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
$(call import-module,extensions)
$(call import-module,iconv)
$(call import-module,scripting/lua-bindings)
