//
//  traffic.cpp
//  AirMap Platform SDK
//
//  Copyright © 2018 AirMap, Inc. All rights reserved.
//

#include <airmap/codec/grpc/traffic.h>

#include <airmap/codec.h>

void airmap::codec::grpc::decode(const ::grpc::airmap::Traffic_Update& from, Traffic::Update& to) {
  to.aircraft_id = from.aircraft_id();

  if (from.has_track()) {
    to.id = from.track().as_string();
  } else {
    to.id = "n/a";
  }

  if (from.has_position()) {
    to.latitude  = from.position().has_latitude() ? from.position().latitude().value() : 0;
    to.longitude = from.position().has_longitude() ? from.position().longitude().value() : 0;
  }

  if (from.has_ground_speed()) {
    to.ground_speed = from.ground_speed().value();
  } else {
    to.ground_speed = 0;
  }

  if (from.has_heading()) {
    to.heading = from.heading().value();
  } else {
    to.heading = 0;
  }

  if (from.has_direction()) {
    to.direction = from.direction().value();
  } else {
    to.direction = 0;
  }

  if (from.has_recorded()) {
    decode(from.recorded(), to.recorded);
  } else {
    to.recorded = Clock::universal_time();
  }

  if (from.has_generated()) {
    decode(from.generated(), to.timestamp);
  } else {
    to.timestamp = Clock::universal_time();
  }
}

void airmap::codec::grpc::encode(::grpc::airmap::Traffic_Update& to, const Traffic::Update& from) {
  to.set_aircraft_id(from.aircraft_id);
  to.mutable_track()->set_as_string(from.id);
  to.mutable_position()->mutable_latitude()->set_value(from.latitude);
  to.mutable_position()->mutable_longitude()->set_value(from.longitude);
  to.mutable_ground_speed()->set_value(from.ground_speed);
  to.mutable_heading()->set_value(from.heading);
  to.mutable_direction()->set_value(from.direction);

  encode(*to.mutable_recorded(), from.recorded);
  encode(*to.mutable_generated(), from.timestamp);
}