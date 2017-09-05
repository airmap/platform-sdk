#include <airmap/context.h>

#include <airmap/boost/context.h>

airmap::Context::CreateResult airmap::Context::create(const std::shared_ptr<Logger>& logger) {
  return CreateResult{boost::Context::create(logger)};
}
