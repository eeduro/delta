#pragma once

#include <eeros/core/Thread.hpp>
#include <eeros/logger/Logger.hpp>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

using namespace eeros::logger;
using namespace eeros::safety;

namespace eeduro {
namespace delta {
  
class UART : public eeros::Thread {
 public:
  UART(const char* dev) : Thread(), log(Logger::getLogger()) {
    fd = open("/dev/ttyS2", O_RDWR);
    int res = ioctl(fd,TIOCCONS,0);
    log.info() << "res = " << res;
    close(fd);
    fd = open(dev, O_RDWR | O_NONBLOCK);
    if (fd < 0) log.error() << "could not open device " << std::string(dev);
  }
  
  ~UART() {
    running = false; 
    join(); 
    close(fd); 
  }
  
  void run() {
    running = true;
    log.warn() << "UART thread started";
    while (running) {
      char ch;
      ssize_t size = read(fd, &ch, 1);
        log.warn() << size;
      if (size > 0) {
        if (ch == 'X') {
          buf[index] = 0;
          index = 0;
          str = buf;
        } else buf[index++] = ch;
      }
      usleep(100000);
    }
  }
  
  std::string getStr() {
    return str;
  }
  
 private:
  int fd;
  char buf[100];
  int index = 0;
  std::string str;
  std::atomic<bool> running;
  Logger log;
};

}
}

