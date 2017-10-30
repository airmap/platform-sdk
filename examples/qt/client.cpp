#include <airmap/qt/client.h>
#include <airmap/qt/logger.h>

#include <airmap/authenticator.h>

#include <QCoreApplication>

namespace {
constexpr const char* api_key =
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
    "eyJjcmVkZW50aWFsX2lkIjoiY3JlZGVudGlhbHx6MzN6T0dEY21wbjV3NmZHTVdwUFpJblBEd1puIiwiYXBwbGljYXRpb25faWQiOiJhcHBsaWNh"
    "dGlvbnxwNE4ySnF2aGc3cFpxWVM2dzZ2eFh0Z2czeDV4Iiwib3JnYW5pemF0aW9uX2lkIjoiZGV2ZWxvcGVyfEt5cURrM0p0N2VuUGJLVVk0QU5i"
    "NUllMEFXbkQiLCJpYXQiOjE1MDg4ODU1NTN9.K3ejcgnoyip3u59ba-VBCivs6tn5gahOsI9FYkCI464";
}

int main(int argc, char** argv) {
  QCoreApplication app{argc, argv};

  auto qlogger = std::make_shared<airmap::qt::Logger>();

  qlogger->logging_category().setEnabled(QtDebugMsg, true);
  qlogger->logging_category().setEnabled(QtInfoMsg, true);
  qlogger->logging_category().setEnabled(QtWarningMsg, true);

  auto credentials    = airmap::Credentials{};
  credentials.api_key = api_key;
  auto dlogger         = std::make_shared<airmap::qt::DispatchingLogger>(qlogger);
  auto configuration  = airmap::Client::default_staging_configuration(credentials);

  airmap::qt::Client::create(configuration, dlogger, &app, [](const auto& result) {
    if (result) {
      qInfo("Successfully created airmap::qt::Client instance");
      airmap::Authenticator::AuthenticateAnonymously::Params params;
      params.id = "qt client";
      result.value()->authenticator().authenticate_anonymously(params, [](const auto& result) {
        if (result) {
          qInfo("Successfully authenticated with AirMap: %s", result.value().id.c_str());
          QCoreApplication::exit(0);
        } else {
          qInfo("Failed to authenticate with AirMap");
          QCoreApplication::exit(1);
        }
      });
    } else {
      qInfo("Failed to create airmap::qt::Client instance");
      QCoreApplication::exit(1);
    }
  });

  return app.exec();
}
