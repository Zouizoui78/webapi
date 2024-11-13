#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>

#include "HTTPServer.hpp"

#include "auth/BasicAuthenticator.hpp"
#include "controllers/UserController.hpp"
#include "db/SQLiteBackend.hpp"
#include "repositories/UserRepository.hpp"
#include "services/UserService.hpp"

std::function<void(int)> signal_handler;
void c_signal_handler(int signal) { signal_handler(signal); }

int main(void) {
  using namespace webapi;

  for (int i = 0; i < NSIG; i++) {
    signal(i, c_signal_handler);
  }

  db::SQLiteBackend sqlite("webapi.db");
  repositories::UserRepository user_repo(&sqlite);
  services::UserService user_service(&user_repo);
  auth::BasicAuthenticator basic_authenticator(&user_service);

  HTTPServer server("www", &basic_authenticator);
  server.register_controller(
      std::make_unique<controllers::UserController>(&user_service));

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
