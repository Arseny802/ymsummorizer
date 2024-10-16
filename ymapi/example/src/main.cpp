#include "pch.h"
#include "ymapi/client.h"

namespace ymsummorizer {

  void main() {
    ymapi::client client("Arseny802", "AQAAAAAcdVsAAAG8XqHsMmzQxkMsh2d2IBG4ysQ");

    if (client.playlist_create("test 0001")) {
      std::cout << "Playlist created" << std::endl;
    } else {
      std::cout << "Playlist NOT created" << std::endl;
    }
  }

} // namespace ymsummorizer

int main() {
  std::cout << "Hello, World!" << std::endl;

  ymsummorizer::main();

  return 0;
}
