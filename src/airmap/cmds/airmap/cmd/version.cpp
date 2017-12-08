#include <airmap/cmds/airmap/cmd/version.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::Version::Version()
    : cli::CommandWithFlagsAndAction{"version", "prints version information", "prints version information"} {
  action([this](const cli::Command::Context& ctxt) {
    ctxt.cout << ::airmap::Version::current().major << "." << ::airmap::Version::current().minor << "."
              << ::airmap::Version::current().patch << " " << ::airmap::Version::current().git_revision << " "
              << ::airmap::Version::current().build_timestamp << std::endl;
    return 0;
  });
}
