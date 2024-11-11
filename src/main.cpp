#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>

#include "HTTPServer.hpp"

#include "controllers/PersonController.hpp"
#include "db/SQLiteBackend.hpp"
#include "repositories/PersonRepository.hpp"
#include "services/PersonService.hpp"

std::function<void(int)> signal_handler;
void c_signal_handler(int signal) { signal_handler(signal); }

int main(void) {
  using namespace webapi;

  for (int i = 0; i < NSIG; i++) {
    signal(i, c_signal_handler);
  }

  db::SQLiteBackend sqlite("webapi.db");
  repositories::PersonRepository person_repo(&sqlite);
  services::PersonService person_service(&person_repo);

  HTTPServer server("www");
  server.register_controller(
      std::make_unique<controllers::PersonController>(&person_service));

  signal_handler = [&server](int signal) {
    std::println("main: Received signal '{}'", strsignal(signal));
    if (signal == SIGINT || signal == SIGTERM) {
      server.stop();
      std::println("main: Stopped HTTP server");
    }
  };

  server.listen("0.0.0.0", 3000);

  return 0;
}
