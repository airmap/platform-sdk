/* Copyright 2018 AirMap, Inc.

Licensed under the Apache License, Version 2.0 (the License);
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an AS IS BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: telemetry.proto

#ifndef PROTOBUF_INCLUDED_telemetry_2eproto
#define PROTOBUF_INCLUDED_telemetry_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_telemetry_2eproto 

namespace protobuf_telemetry_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[4];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_telemetry_2eproto
namespace airmap {
namespace telemetry {
class Attitude;
class AttitudeDefaultTypeInternal;
extern AttitudeDefaultTypeInternal _Attitude_default_instance_;
class Barometer;
class BarometerDefaultTypeInternal;
extern BarometerDefaultTypeInternal _Barometer_default_instance_;
class Position;
class PositionDefaultTypeInternal;
extern PositionDefaultTypeInternal _Position_default_instance_;
class Speed;
class SpeedDefaultTypeInternal;
extern SpeedDefaultTypeInternal _Speed_default_instance_;
}  // namespace telemetry
}  // namespace airmap
namespace google {
namespace protobuf {
template<> ::airmap::telemetry::Attitude* Arena::CreateMaybeMessage<::airmap::telemetry::Attitude>(Arena*);
template<> ::airmap::telemetry::Barometer* Arena::CreateMaybeMessage<::airmap::telemetry::Barometer>(Arena*);
template<> ::airmap::telemetry::Position* Arena::CreateMaybeMessage<::airmap::telemetry::Position>(Arena*);
template<> ::airmap::telemetry::Speed* Arena::CreateMaybeMessage<::airmap::telemetry::Speed>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace airmap {
namespace telemetry {

// ===================================================================

class Position : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:airmap.telemetry.Position) */ {
 public:
  Position();
  virtual ~Position();

  Position(const Position& from);

  inline Position& operator=(const Position& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Position(Position&& from) noexcept
    : Position() {
    *this = ::std::move(from);
  }

  inline Position& operator=(Position&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Position& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Position* internal_default_instance() {
    return reinterpret_cast<const Position*>(
               &_Position_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Position* other);
  friend void swap(Position& a, Position& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Position* New() const final {
    return CreateMaybeMessage<Position>(NULL);
  }

  Position* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Position>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Position& from);
  void MergeFrom(const Position& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Position* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint64 timestamp = 1;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // double latitude = 2;
  void clear_latitude();
  static const int kLatitudeFieldNumber = 2;
  double latitude() const;
  void set_latitude(double value);

  // double longitude = 3;
  void clear_longitude();
  static const int kLongitudeFieldNumber = 3;
  double longitude() const;
  void set_longitude(double value);

  // float altitude_agl = 4;
  void clear_altitude_agl();
  static const int kAltitudeAglFieldNumber = 4;
  float altitude_agl() const;
  void set_altitude_agl(float value);

  // float altitude_msl = 5;
  void clear_altitude_msl();
  static const int kAltitudeMslFieldNumber = 5;
  float altitude_msl() const;
  void set_altitude_msl(float value);

  // float horizontal_accuracy = 6;
  void clear_horizontal_accuracy();
  static const int kHorizontalAccuracyFieldNumber = 6;
  float horizontal_accuracy() const;
  void set_horizontal_accuracy(float value);

  // @@protoc_insertion_point(class_scope:airmap.telemetry.Position)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint64 timestamp_;
  double latitude_;
  double longitude_;
  float altitude_agl_;
  float altitude_msl_;
  float horizontal_accuracy_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_telemetry_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Attitude : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:airmap.telemetry.Attitude) */ {
 public:
  Attitude();
  virtual ~Attitude();

  Attitude(const Attitude& from);

  inline Attitude& operator=(const Attitude& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Attitude(Attitude&& from) noexcept
    : Attitude() {
    *this = ::std::move(from);
  }

  inline Attitude& operator=(Attitude&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Attitude& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Attitude* internal_default_instance() {
    return reinterpret_cast<const Attitude*>(
               &_Attitude_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Attitude* other);
  friend void swap(Attitude& a, Attitude& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Attitude* New() const final {
    return CreateMaybeMessage<Attitude>(NULL);
  }

  Attitude* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Attitude>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Attitude& from);
  void MergeFrom(const Attitude& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Attitude* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint64 timestamp = 1;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // float yaw = 2;
  void clear_yaw();
  static const int kYawFieldNumber = 2;
  float yaw() const;
  void set_yaw(float value);

  // float pitch = 3;
  void clear_pitch();
  static const int kPitchFieldNumber = 3;
  float pitch() const;
  void set_pitch(float value);

  // float roll = 4;
  void clear_roll();
  static const int kRollFieldNumber = 4;
  float roll() const;
  void set_roll(float value);

  // @@protoc_insertion_point(class_scope:airmap.telemetry.Attitude)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint64 timestamp_;
  float yaw_;
  float pitch_;
  float roll_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_telemetry_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Speed : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:airmap.telemetry.Speed) */ {
 public:
  Speed();
  virtual ~Speed();

  Speed(const Speed& from);

  inline Speed& operator=(const Speed& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Speed(Speed&& from) noexcept
    : Speed() {
    *this = ::std::move(from);
  }

  inline Speed& operator=(Speed&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Speed& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Speed* internal_default_instance() {
    return reinterpret_cast<const Speed*>(
               &_Speed_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(Speed* other);
  friend void swap(Speed& a, Speed& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Speed* New() const final {
    return CreateMaybeMessage<Speed>(NULL);
  }

  Speed* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Speed>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Speed& from);
  void MergeFrom(const Speed& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Speed* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint64 timestamp = 1;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // float velocity_x = 2;
  void clear_velocity_x();
  static const int kVelocityXFieldNumber = 2;
  float velocity_x() const;
  void set_velocity_x(float value);

  // float velocity_y = 3;
  void clear_velocity_y();
  static const int kVelocityYFieldNumber = 3;
  float velocity_y() const;
  void set_velocity_y(float value);

  // float velocity_z = 4;
  void clear_velocity_z();
  static const int kVelocityZFieldNumber = 4;
  float velocity_z() const;
  void set_velocity_z(float value);

  // @@protoc_insertion_point(class_scope:airmap.telemetry.Speed)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint64 timestamp_;
  float velocity_x_;
  float velocity_y_;
  float velocity_z_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_telemetry_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Barometer : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:airmap.telemetry.Barometer) */ {
 public:
  Barometer();
  virtual ~Barometer();

  Barometer(const Barometer& from);

  inline Barometer& operator=(const Barometer& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Barometer(Barometer&& from) noexcept
    : Barometer() {
    *this = ::std::move(from);
  }

  inline Barometer& operator=(Barometer&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Barometer& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Barometer* internal_default_instance() {
    return reinterpret_cast<const Barometer*>(
               &_Barometer_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  void Swap(Barometer* other);
  friend void swap(Barometer& a, Barometer& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Barometer* New() const final {
    return CreateMaybeMessage<Barometer>(NULL);
  }

  Barometer* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Barometer>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Barometer& from);
  void MergeFrom(const Barometer& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Barometer* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // uint64 timestamp = 1;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // float pressure = 2;
  void clear_pressure();
  static const int kPressureFieldNumber = 2;
  float pressure() const;
  void set_pressure(float value);

  // @@protoc_insertion_point(class_scope:airmap.telemetry.Barometer)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint64 timestamp_;
  float pressure_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_telemetry_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Position

// uint64 timestamp = 1;
inline void Position::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Position::timestamp() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.timestamp)
  return timestamp_;
}
inline void Position::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.timestamp)
}

// double latitude = 2;
inline void Position::clear_latitude() {
  latitude_ = 0;
}
inline double Position::latitude() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.latitude)
  return latitude_;
}
inline void Position::set_latitude(double value) {
  
  latitude_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.latitude)
}

// double longitude = 3;
inline void Position::clear_longitude() {
  longitude_ = 0;
}
inline double Position::longitude() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.longitude)
  return longitude_;
}
inline void Position::set_longitude(double value) {
  
  longitude_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.longitude)
}

// float altitude_agl = 4;
inline void Position::clear_altitude_agl() {
  altitude_agl_ = 0;
}
inline float Position::altitude_agl() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.altitude_agl)
  return altitude_agl_;
}
inline void Position::set_altitude_agl(float value) {
  
  altitude_agl_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.altitude_agl)
}

// float altitude_msl = 5;
inline void Position::clear_altitude_msl() {
  altitude_msl_ = 0;
}
inline float Position::altitude_msl() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.altitude_msl)
  return altitude_msl_;
}
inline void Position::set_altitude_msl(float value) {
  
  altitude_msl_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.altitude_msl)
}

// float horizontal_accuracy = 6;
inline void Position::clear_horizontal_accuracy() {
  horizontal_accuracy_ = 0;
}
inline float Position::horizontal_accuracy() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Position.horizontal_accuracy)
  return horizontal_accuracy_;
}
inline void Position::set_horizontal_accuracy(float value) {
  
  horizontal_accuracy_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Position.horizontal_accuracy)
}

// -------------------------------------------------------------------

// Attitude

// uint64 timestamp = 1;
inline void Attitude::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Attitude::timestamp() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Attitude.timestamp)
  return timestamp_;
}
inline void Attitude::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Attitude.timestamp)
}

// float yaw = 2;
inline void Attitude::clear_yaw() {
  yaw_ = 0;
}
inline float Attitude::yaw() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Attitude.yaw)
  return yaw_;
}
inline void Attitude::set_yaw(float value) {
  
  yaw_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Attitude.yaw)
}

// float pitch = 3;
inline void Attitude::clear_pitch() {
  pitch_ = 0;
}
inline float Attitude::pitch() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Attitude.pitch)
  return pitch_;
}
inline void Attitude::set_pitch(float value) {
  
  pitch_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Attitude.pitch)
}

// float roll = 4;
inline void Attitude::clear_roll() {
  roll_ = 0;
}
inline float Attitude::roll() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Attitude.roll)
  return roll_;
}
inline void Attitude::set_roll(float value) {
  
  roll_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Attitude.roll)
}

// -------------------------------------------------------------------

// Speed

// uint64 timestamp = 1;
inline void Speed::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Speed::timestamp() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Speed.timestamp)
  return timestamp_;
}
inline void Speed::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Speed.timestamp)
}

// float velocity_x = 2;
inline void Speed::clear_velocity_x() {
  velocity_x_ = 0;
}
inline float Speed::velocity_x() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Speed.velocity_x)
  return velocity_x_;
}
inline void Speed::set_velocity_x(float value) {
  
  velocity_x_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Speed.velocity_x)
}

// float velocity_y = 3;
inline void Speed::clear_velocity_y() {
  velocity_y_ = 0;
}
inline float Speed::velocity_y() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Speed.velocity_y)
  return velocity_y_;
}
inline void Speed::set_velocity_y(float value) {
  
  velocity_y_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Speed.velocity_y)
}

// float velocity_z = 4;
inline void Speed::clear_velocity_z() {
  velocity_z_ = 0;
}
inline float Speed::velocity_z() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Speed.velocity_z)
  return velocity_z_;
}
inline void Speed::set_velocity_z(float value) {
  
  velocity_z_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Speed.velocity_z)
}

// -------------------------------------------------------------------

// Barometer

// uint64 timestamp = 1;
inline void Barometer::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 Barometer::timestamp() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Barometer.timestamp)
  return timestamp_;
}
inline void Barometer::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Barometer.timestamp)
}

// float pressure = 2;
inline void Barometer::clear_pressure() {
  pressure_ = 0;
}
inline float Barometer::pressure() const {
  // @@protoc_insertion_point(field_get:airmap.telemetry.Barometer.pressure)
  return pressure_;
}
inline void Barometer::set_pressure(float value) {
  
  pressure_ = value;
  // @@protoc_insertion_point(field_set:airmap.telemetry.Barometer.pressure)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace telemetry
}  // namespace airmap

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_telemetry_2eproto
