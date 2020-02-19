#ifndef __UDP_READOUT_H__
#define __UDP_READOUT_H__

#include <stdint.h>
#include <pthread.h>

#include <string>
#include <vector>
#include <queue>

namespace UDP_READOUT
{
  const uint32_t PAGE_SIZE            = 4096;
  const uint32_t UDP_PK_SIZE          = 1450;

  class Exception : public std::string
  {
  public:
    Exception(const char* format, ...);
    const char* what() const { return c_str(); }
  };
  
  struct Event
  {
    uint32_t event_size;
    int      port;
    uint8_t  buffer[UDP_PK_SIZE];
  };

  class Readout
  {
    static void* ReadoutRoutine(void*);
  public:
    
    Readout(const char* IP, int* ports);
    virtual ~Readout();
    
    Event* GetNextEvent();

    bool IsRunning() const { return f_isRunning; }

  private:
    pthread_mutex_t*        f_mutex;    
    pthread_t               f_readout_thread;
    std::vector<int>        f_sockets;
    std::vector<int>        f_ports;
    std::queue<Event*>      f_events;
    bool                    f_isRunning;
  };

  const int INFO    = 0;
  const int WARNING = 1;
  const int ERROR   = 2;
  const int SUMMARY = 3;

  void Message(const int msg_type, const char* format,...);

}

#endif
