// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TemperatureRequest.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "TemperatureRequest.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* TemperatureRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TemperatureRequest_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_TemperatureRequest_2eproto() {
  protobuf_AddDesc_TemperatureRequest_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "TemperatureRequest.proto");
  GOOGLE_CHECK(file != NULL);
  TemperatureRequest_descriptor_ = file->message_type(0);
  static const int TemperatureRequest_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, sensorid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, starttime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, endtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, interval_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, secondsbetweenreadings_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, summaryonly_),
  };
  TemperatureRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      TemperatureRequest_descriptor_,
      TemperatureRequest::default_instance_,
      TemperatureRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TemperatureRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(TemperatureRequest));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_TemperatureRequest_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    TemperatureRequest_descriptor_, &TemperatureRequest::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_TemperatureRequest_2eproto() {
  delete TemperatureRequest::default_instance_;
  delete TemperatureRequest_reflection_;
}

void protobuf_AddDesc_TemperatureRequest_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\030TemperatureRequest.proto\"\221\001\n\022Temperatu"
    "reRequest\022\020\n\010sensorId\030\001 \002(\r\022\021\n\tstartTime"
    "\030\002 \001(\r\022\017\n\007endTime\030\003 \001(\r\022\020\n\010interval\030\004 \001("
    "\r\022\036\n\026secondsBetweenReadings\030\005 \001(\r\022\023\n\013sum"
    "maryOnly\030\006 \001(\010", 174);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "TemperatureRequest.proto", &protobuf_RegisterTypes);
  TemperatureRequest::default_instance_ = new TemperatureRequest();
  TemperatureRequest::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_TemperatureRequest_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_TemperatureRequest_2eproto {
  StaticDescriptorInitializer_TemperatureRequest_2eproto() {
    protobuf_AddDesc_TemperatureRequest_2eproto();
  }
} static_descriptor_initializer_TemperatureRequest_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int TemperatureRequest::kSensorIdFieldNumber;
const int TemperatureRequest::kStartTimeFieldNumber;
const int TemperatureRequest::kEndTimeFieldNumber;
const int TemperatureRequest::kIntervalFieldNumber;
const int TemperatureRequest::kSecondsBetweenReadingsFieldNumber;
const int TemperatureRequest::kSummaryOnlyFieldNumber;
#endif  // !_MSC_VER

TemperatureRequest::TemperatureRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:TemperatureRequest)
}

void TemperatureRequest::InitAsDefaultInstance() {
}

TemperatureRequest::TemperatureRequest(const TemperatureRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:TemperatureRequest)
}

void TemperatureRequest::SharedCtor() {
  _cached_size_ = 0;
  sensorid_ = 0u;
  starttime_ = 0u;
  endtime_ = 0u;
  interval_ = 0u;
  secondsbetweenreadings_ = 0u;
  summaryonly_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

TemperatureRequest::~TemperatureRequest() {
  // @@protoc_insertion_point(destructor:TemperatureRequest)
  SharedDtor();
}

void TemperatureRequest::SharedDtor() {
  if (this != default_instance_) {
  }
}

void TemperatureRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TemperatureRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TemperatureRequest_descriptor_;
}

const TemperatureRequest& TemperatureRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_TemperatureRequest_2eproto();
  return *default_instance_;
}

TemperatureRequest* TemperatureRequest::default_instance_ = NULL;

TemperatureRequest* TemperatureRequest::New() const {
  return new TemperatureRequest;
}

void TemperatureRequest::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<TemperatureRequest*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  if (_has_bits_[0 / 32] & 63) {
    ZR_(sensorid_, summaryonly_);
  }

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool TemperatureRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:TemperatureRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint32 sensorId = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &sensorid_)));
          set_has_sensorid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_startTime;
        break;
      }

      // optional uint32 startTime = 2;
      case 2: {
        if (tag == 16) {
         parse_startTime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &starttime_)));
          set_has_starttime();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(24)) goto parse_endTime;
        break;
      }

      // optional uint32 endTime = 3;
      case 3: {
        if (tag == 24) {
         parse_endTime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &endtime_)));
          set_has_endtime();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(32)) goto parse_interval;
        break;
      }

      // optional uint32 interval = 4;
      case 4: {
        if (tag == 32) {
         parse_interval:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &interval_)));
          set_has_interval();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(40)) goto parse_secondsBetweenReadings;
        break;
      }

      // optional uint32 secondsBetweenReadings = 5;
      case 5: {
        if (tag == 40) {
         parse_secondsBetweenReadings:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &secondsbetweenreadings_)));
          set_has_secondsbetweenreadings();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(48)) goto parse_summaryOnly;
        break;
      }

      // optional bool summaryOnly = 6;
      case 6: {
        if (tag == 48) {
         parse_summaryOnly:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &summaryonly_)));
          set_has_summaryonly();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:TemperatureRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:TemperatureRequest)
  return false;
#undef DO_
}

void TemperatureRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:TemperatureRequest)
  // required uint32 sensorId = 1;
  if (has_sensorid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(1, this->sensorid(), output);
  }

  // optional uint32 startTime = 2;
  if (has_starttime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->starttime(), output);
  }

  // optional uint32 endTime = 3;
  if (has_endtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(3, this->endtime(), output);
  }

  // optional uint32 interval = 4;
  if (has_interval()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(4, this->interval(), output);
  }

  // optional uint32 secondsBetweenReadings = 5;
  if (has_secondsbetweenreadings()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(5, this->secondsbetweenreadings(), output);
  }

  // optional bool summaryOnly = 6;
  if (has_summaryonly()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(6, this->summaryonly(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:TemperatureRequest)
}

::google::protobuf::uint8* TemperatureRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:TemperatureRequest)
  // required uint32 sensorId = 1;
  if (has_sensorid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(1, this->sensorid(), target);
  }

  // optional uint32 startTime = 2;
  if (has_starttime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->starttime(), target);
  }

  // optional uint32 endTime = 3;
  if (has_endtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(3, this->endtime(), target);
  }

  // optional uint32 interval = 4;
  if (has_interval()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(4, this->interval(), target);
  }

  // optional uint32 secondsBetweenReadings = 5;
  if (has_secondsbetweenreadings()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(5, this->secondsbetweenreadings(), target);
  }

  // optional bool summaryOnly = 6;
  if (has_summaryonly()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(6, this->summaryonly(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:TemperatureRequest)
  return target;
}

int TemperatureRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint32 sensorId = 1;
    if (has_sensorid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->sensorid());
    }

    // optional uint32 startTime = 2;
    if (has_starttime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->starttime());
    }

    // optional uint32 endTime = 3;
    if (has_endtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->endtime());
    }

    // optional uint32 interval = 4;
    if (has_interval()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->interval());
    }

    // optional uint32 secondsBetweenReadings = 5;
    if (has_secondsbetweenreadings()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->secondsbetweenreadings());
    }

    // optional bool summaryOnly = 6;
    if (has_summaryonly()) {
      total_size += 1 + 1;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void TemperatureRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const TemperatureRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const TemperatureRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void TemperatureRequest::MergeFrom(const TemperatureRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_sensorid()) {
      set_sensorid(from.sensorid());
    }
    if (from.has_starttime()) {
      set_starttime(from.starttime());
    }
    if (from.has_endtime()) {
      set_endtime(from.endtime());
    }
    if (from.has_interval()) {
      set_interval(from.interval());
    }
    if (from.has_secondsbetweenreadings()) {
      set_secondsbetweenreadings(from.secondsbetweenreadings());
    }
    if (from.has_summaryonly()) {
      set_summaryonly(from.summaryonly());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void TemperatureRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TemperatureRequest::CopyFrom(const TemperatureRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TemperatureRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void TemperatureRequest::Swap(TemperatureRequest* other) {
  if (other != this) {
    std::swap(sensorid_, other->sensorid_);
    std::swap(starttime_, other->starttime_);
    std::swap(endtime_, other->endtime_);
    std::swap(interval_, other->interval_);
    std::swap(secondsbetweenreadings_, other->secondsbetweenreadings_);
    std::swap(summaryonly_, other->summaryonly_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata TemperatureRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TemperatureRequest_descriptor_;
  metadata.reflection = TemperatureRequest_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
