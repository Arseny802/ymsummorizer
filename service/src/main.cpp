#include "pch.h"

namespace ymsummorizer::service {

  void main() {
    log()->debug("Starting...");
  }

} // namespace ymsummorizer::service

int main() {
  ymsummorizer::service::initialize_logging();
  ymsummorizer::service::main();

  return EXIT_SUCCESS;
}
