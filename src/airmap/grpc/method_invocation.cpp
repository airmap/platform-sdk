#include <airmap/grpc/method_invocation.h>

#include <iostream>

std::ostream& airmap::grpc::operator<<(std::ostream& out, MethodInvocation::State state) {
  switch (state) {
    case MethodInvocation::State::ready:
      return out << "ready";
    case MethodInvocation::State::streaming:
      return out << "streaming";
    case MethodInvocation::State::finished:
      return out << "ready";
  }

  return out;
}