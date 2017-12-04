#include <airmap/net/http/response.h>

// classify returns the Classification of the status of this response.
airmap::net::http::Response::Classification airmap::net::http::Response::classify() const {
  if (100 <= status && status < 200) {
    return Classification::informational;
  } else if (200 <= status && status < 300) {
    return Classification::success;
  } else if (300 <= status && status < 400) {
    return Classification::redirection;
  } else if (400 <= status && status < 500) {
    return Classification::client_error;
  } else if (500 <= status && status < 600) {
    return Classification::server_error;
  }

  return Classification::unclassified;
}