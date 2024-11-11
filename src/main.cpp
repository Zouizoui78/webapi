#include <csignal>
#include <cstring>
#include <functional>
#include <iostream>

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

  entities::Person p;
  p.first_name = "Myfirstname";
  p.last_name = "Mylastname";
  p.age = 50;
  auto id = person_service.create(p);

  if (id.has_value()) {
    std::println("new id = {}", *id);
  }

  signal_handler = [](int signal) {
    std::cout << "Received signal " << strsignal(signal) << std::endl;
    if (signal == SIGINT || signal == SIGTERM) {
    }
  };

  return 0;
}
