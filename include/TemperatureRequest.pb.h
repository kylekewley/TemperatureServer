// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TemperatureRequest.proto

#ifndef PROTOBUF_TemperatureRequest_2eproto__INCLUDED
#define PROTOBUF_TemperatureRequest_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006002 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
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

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TemperatureRequest_2eproto();
void protobuf_AssignDesc_TemperatureRequest_2eproto();
void protobuf_ShutdownFile_TemperatureRequest_2eproto();

class TemperatureRequest;

// ===================================================================

class TemperatureRequest : public ::google::protobuf::Message {
 public:
  TemperatureRequest();
  virtual ~TemperatureRequest();

  TemperatureRequest(const TemperatureRequest& from);

  inline TemperatureRequest& operator=(const TemperatureRequest& from) {
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
  static const TemperatureRequest& default_instance();

  void Swap(TemperatureRequest* other);

  // implements Message ----------------------------------------------

  TemperatureRequest* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TemperatureRequest& from);
  void MergeFrom(const TemperatureRequest& from);
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

  // required uint32 sensorId = 1;
  inline bool has_sensorid() const;
  inline void clear_sensorid();
  static const int kSensorIdFieldNumber = 1;
  inline ::google::protobuf::uint32 sensorid() const;
  inline void set_sensorid(::google::protobuf::uint32 value);

  // optional uint32 startTime = 2;
  inline bool has_starttime() const;
  inline void clear_starttime();
  static const int kStartTimeFieldNumber = 2;
  inline ::google::protobuf::uint32 starttime() const;
  inline void set_starttime(::google::protobuf::uint32 value);

  // optional uint32 endTime = 3;
  inline bool has_endtime() const;
  inline void clear_endtime();
  static const int kEndTimeFieldNumber = 3;
  inline ::google::protobuf::uint32 endtime() const;
  inline void set_endtime(::google::protobuf::uint32 value);

  // optional uint32 interval = 4;
  inline bool has_interval() const;
  inline void clear_interval();
  static const int kIntervalFieldNumber = 4;
  inline ::google::protobuf::uint32 interval() const;
  inline void set_interval(::google::protobuf::uint32 value);

  // optional uint32 secondsBetweenReadings = 5;
  inline bool has_secondsbetweenreadings() const;
  inline void clear_secondsbetweenreadings();
  static const int kSecondsBetweenReadingsFieldNumber = 5;
  inline ::google::protobuf::uint32 secondsbetweenreadings() const;
  inline void set_secondsbetweenreadings(::google::protobuf::uint32 value);

  // optional bool summaryOnly = 6;
  inline bool has_summaryonly() const;
  inline void clear_summaryonly();
  static const int kSummaryOnlyFieldNumber = 6;
  inline bool summaryonly() const;
  inline void set_summaryonly(bool value);

  // @@protoc_insertion_point(class_scope:TemperatureRequest)
 private:
  inline void set_has_sensorid();
  inline void clear_has_sensorid();
  inline void set_has_starttime();
  inline void clear_has_starttime();
  inline void set_has_endtime();
  inline void clear_has_endtime();
  inline void set_has_interval();
  inline void clear_has_interval();
  inline void set_has_secondsbetweenreadings();
  inline void clear_has_secondsbetweenreadings();
  inline void set_has_summaryonly();
  inline void clear_has_summaryonly();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint32 sensorid_;
  ::google::protobuf::uint32 starttime_;
  ::google::protobuf::uint32 endtime_;
  ::google::protobuf::uint32 interval_;
  ::google::protobuf::uint32 secondsbetweenreadings_;
  bool summaryonly_;
  friend void  protobuf_AddDesc_TemperatureRequest_2eproto();
  friend void protobuf_AssignDesc_TemperatureRequest_2eproto();
  friend void protobuf_ShutdownFile_TemperatureRequest_2eproto();

  void InitAsDefaultInstance();
  static TemperatureRequest* default_instance_;
};
// ===================================================================


// ===================================================================

// TemperatureRequest

// required uint32 sensorId = 1;
inline bool TemperatureRequest::has_sensorid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void TemperatureRequest::set_has_sensorid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void TemperatureRequest::clear_has_sensorid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void TemperatureRequest::clear_sensorid() {
  sensorid_ = 0u;
  clear_has_sensorid();
}
inline ::google::protobuf::uint32 TemperatureRequest::sensorid() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.sensorId)
  return sensorid_;
}
inline void TemperatureRequest::set_sensorid(::google::protobuf::uint32 value) {
  set_has_sensorid();
  sensorid_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.sensorId)
}

// optional uint32 startTime = 2;
inline bool TemperatureRequest::has_starttime() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void TemperatureRequest::set_has_starttime() {
  _has_bits_[0] |= 0x00000002u;
}
inline void TemperatureRequest::clear_has_starttime() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void TemperatureRequest::clear_starttime() {
  starttime_ = 0u;
  clear_has_starttime();
}
inline ::google::protobuf::uint32 TemperatureRequest::starttime() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.startTime)
  return starttime_;
}
inline void TemperatureRequest::set_starttime(::google::protobuf::uint32 value) {
  set_has_starttime();
  starttime_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.startTime)
}

// optional uint32 endTime = 3;
inline bool TemperatureRequest::has_endtime() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void TemperatureRequest::set_has_endtime() {
  _has_bits_[0] |= 0x00000004u;
}
inline void TemperatureRequest::clear_has_endtime() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void TemperatureRequest::clear_endtime() {
  endtime_ = 0u;
  clear_has_endtime();
}
inline ::google::protobuf::uint32 TemperatureRequest::endtime() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.endTime)
  return endtime_;
}
inline void TemperatureRequest::set_endtime(::google::protobuf::uint32 value) {
  set_has_endtime();
  endtime_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.endTime)
}

// optional uint32 interval = 4;
inline bool TemperatureRequest::has_interval() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void TemperatureRequest::set_has_interval() {
  _has_bits_[0] |= 0x00000008u;
}
inline void TemperatureRequest::clear_has_interval() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void TemperatureRequest::clear_interval() {
  interval_ = 0u;
  clear_has_interval();
}
inline ::google::protobuf::uint32 TemperatureRequest::interval() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.interval)
  return interval_;
}
inline void TemperatureRequest::set_interval(::google::protobuf::uint32 value) {
  set_has_interval();
  interval_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.interval)
}

// optional uint32 secondsBetweenReadings = 5;
inline bool TemperatureRequest::has_secondsbetweenreadings() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void TemperatureRequest::set_has_secondsbetweenreadings() {
  _has_bits_[0] |= 0x00000010u;
}
inline void TemperatureRequest::clear_has_secondsbetweenreadings() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void TemperatureRequest::clear_secondsbetweenreadings() {
  secondsbetweenreadings_ = 0u;
  clear_has_secondsbetweenreadings();
}
inline ::google::protobuf::uint32 TemperatureRequest::secondsbetweenreadings() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.secondsBetweenReadings)
  return secondsbetweenreadings_;
}
inline void TemperatureRequest::set_secondsbetweenreadings(::google::protobuf::uint32 value) {
  set_has_secondsbetweenreadings();
  secondsbetweenreadings_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.secondsBetweenReadings)
}

// optional bool summaryOnly = 6;
inline bool TemperatureRequest::has_summaryonly() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void TemperatureRequest::set_has_summaryonly() {
  _has_bits_[0] |= 0x00000020u;
}
inline void TemperatureRequest::clear_has_summaryonly() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void TemperatureRequest::clear_summaryonly() {
  summaryonly_ = false;
  clear_has_summaryonly();
}
inline bool TemperatureRequest::summaryonly() const {
  // @@protoc_insertion_point(field_get:TemperatureRequest.summaryOnly)
  return summaryonly_;
}
inline void TemperatureRequest::set_summaryonly(bool value) {
  set_has_summaryonly();
  summaryonly_ = value;
  // @@protoc_insertion_point(field_set:TemperatureRequest.summaryOnly)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TemperatureRequest_2eproto__INCLUDED
