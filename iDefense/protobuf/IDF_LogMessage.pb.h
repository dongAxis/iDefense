// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: IDF_LogMessage.proto

#ifndef PROTOBUF_IDF_5fLogMessage_2eproto__INCLUDED
#define PROTOBUF_IDF_5fLogMessage_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace idf {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_IDF_5fLogMessage_2eproto();
void protobuf_AssignDesc_IDF_5fLogMessage_2eproto();
void protobuf_ShutdownFile_IDF_5fLogMessage_2eproto();

class LogMessage;

// ===================================================================

class LogMessage : public ::google::protobuf::Message {
 public:
  LogMessage();
  virtual ~LogMessage();

  LogMessage(const LogMessage& from);

  inline LogMessage& operator=(const LogMessage& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const LogMessage& default_instance();

  void Swap(LogMessage* other);

  // implements Message ----------------------------------------------

  LogMessage* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const LogMessage& from);
  void MergeFrom(const LogMessage& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 debug_level = 1 [default = 3];
  inline bool has_debug_level() const;
  inline void clear_debug_level();
  static const int kDebugLevelFieldNumber = 1;
  inline ::google::protobuf::int32 debug_level() const;
  inline void set_debug_level(::google::protobuf::int32 value);

  // required int32 max_size = 2 [default = 4194304];
  inline bool has_max_size() const;
  inline void clear_max_size();
  static const int kMaxSizeFieldNumber = 2;
  inline ::google::protobuf::int32 max_size() const;
  inline void set_max_size(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:idf.LogMessage)
 private:
  inline void set_has_debug_level();
  inline void clear_has_debug_level();
  inline void set_has_max_size();
  inline void clear_has_max_size();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 debug_level_;
  ::google::protobuf::int32 max_size_;
  friend void  protobuf_AddDesc_IDF_5fLogMessage_2eproto();
  friend void protobuf_AssignDesc_IDF_5fLogMessage_2eproto();
  friend void protobuf_ShutdownFile_IDF_5fLogMessage_2eproto();

  void InitAsDefaultInstance();
  static LogMessage* default_instance_;
};
// ===================================================================


// ===================================================================

// LogMessage

// required int32 debug_level = 1 [default = 3];
inline bool LogMessage::has_debug_level() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LogMessage::set_has_debug_level() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LogMessage::clear_has_debug_level() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LogMessage::clear_debug_level() {
  debug_level_ = 3;
  clear_has_debug_level();
}
inline ::google::protobuf::int32 LogMessage::debug_level() const {
  // @@protoc_insertion_point(field_get:idf.LogMessage.debug_level)
  return debug_level_;
}
inline void LogMessage::set_debug_level(::google::protobuf::int32 value) {
  set_has_debug_level();
  debug_level_ = value;
  // @@protoc_insertion_point(field_set:idf.LogMessage.debug_level)
}

// required int32 max_size = 2 [default = 4194304];
inline bool LogMessage::has_max_size() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LogMessage::set_has_max_size() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LogMessage::clear_has_max_size() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LogMessage::clear_max_size() {
  max_size_ = 4194304;
  clear_has_max_size();
}
inline ::google::protobuf::int32 LogMessage::max_size() const {
  // @@protoc_insertion_point(field_get:idf.LogMessage.max_size)
  return max_size_;
}
inline void LogMessage::set_max_size(::google::protobuf::int32 value) {
  set_has_max_size();
  max_size_ = value;
  // @@protoc_insertion_point(field_set:idf.LogMessage.max_size)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace idf

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_IDF_5fLogMessage_2eproto__INCLUDED