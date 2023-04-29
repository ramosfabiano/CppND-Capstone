#include <file1.hpp>
#include <iostream>

int main() {
  std::cout << "Listening on port xxxx\n";
  std::cout << "Serving files from folder: xxxx\n";

  http_server_lib::dummy();
  return 0;
}