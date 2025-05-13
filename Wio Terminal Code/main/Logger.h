#ifndef LOGGER_H
#define LOGGER_H

class Logger {
public:
  Logger(const char* filePath);
  bool begin();
  void log(const char* key, const char* action);
  void publish(PubSubClient& client);

private:
  String formatDate(time_t t);
  const char* _filePath;
  String lastLogJson;
};

#endif