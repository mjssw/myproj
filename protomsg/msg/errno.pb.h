// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: errno.proto

#ifndef PROTOBUF_errno_2eproto__INCLUDED
#define PROTOBUF_errno_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

namespace sglib {
namespace errorcode {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_errno_2eproto();
void protobuf_AssignDesc_errno_2eproto();
void protobuf_ShutdownFile_errno_2eproto();


enum EErrorCode {
  E_ErrorCode_Success = 0,
  E_ErrorCode_Unknown = -1,
  E_ErrorCode_StateError = -2,
  E_ErrorCode_RegError = -3,
  E_ErrorCode_NoSuchGame = -101,
  E_ErrorCode_GameUserFull = -102,
  E_ErrorCode_NotLoginGame = -103,
  E_ErrorCode_AleardyLoginGame = -104,
  E_ErrorCode_NoThisRoom = -201,
  E_ErrorCode_RoomIsFull = -202,
  E_ErrorCode_RoomPwdError = -203,
  E_ErrorCode_AleardyInRoom = -204,
  E_ErrorCode_CreateGroupError = -301,
  E_ErrorCode_NotFoundGroup = -302,
  E_ErrorCode_NotFoundLoginGroup = -303,
  E_ErrorCode_NotInThisGroup = -304,
  E_ErrorCode_NoThisGroupGame = -305,
  E_ErrorCode_NotEnoughRoom = -306,
  E_ErrorCode_Tetris_NotInRoom = -10001
};
bool EErrorCode_IsValid(int value);
const EErrorCode EErrorCode_MIN = E_ErrorCode_Tetris_NotInRoom;
const EErrorCode EErrorCode_MAX = E_ErrorCode_Success;
const int EErrorCode_ARRAYSIZE = EErrorCode_MAX + 1;

const ::google::protobuf::EnumDescriptor* EErrorCode_descriptor();
inline const ::std::string& EErrorCode_Name(EErrorCode value) {
  return ::google::protobuf::internal::NameOfEnum(
    EErrorCode_descriptor(), value);
}
inline bool EErrorCode_Parse(
    const ::std::string& name, EErrorCode* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EErrorCode>(
    EErrorCode_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

}  // namespace errorcode
}  // namespace sglib

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::sglib::errorcode::EErrorCode>() {
  return ::sglib::errorcode::EErrorCode_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_errno_2eproto__INCLUDED
