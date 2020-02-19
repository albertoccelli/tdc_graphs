#include <UDP_Readout.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netdb.h>

using namespace UDP_READOUT;

static int N_READ_PACKETS = 0;

Readout::Readout(const char* ip_address, int* ports) :
  f_isRunning(false)
{
  f_mutex = new pthread_mutex_t;
  int retCode = pthread_mutex_init(f_mutex,NULL);
  if(retCode != 0)
    {
      throw Exception("Cannot init mutex, error code %d", retCode);
    }
  for(int i = 0; ports[i] > 0; i++)
    {
      int port = ports[i];
      f_ports.push_back(port);
      char decimal_port[16];
      snprintf(decimal_port, sizeof(decimal_port), "%d", port);
      decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
      struct addrinfo hints;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family   = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      hints.ai_protocol = IPPROTO_UDP;
      struct addrinfo* _addrinfo;

      int r(getaddrinfo(ip_address, decimal_port, &hints, &_addrinfo));
      if(r != 0 || _addrinfo == NULL)
	{
	  throw Exception("Invalid address or port for UDP socket: %s:%s",
			  ip_address,decimal_port);
	}
      int sock = socket(_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
      if(sock == -1)
	{
	  freeaddrinfo(_addrinfo);
	  throw Exception("Could not create UDP socket for: %s:%d (%s)",
			  ip_address,port,strerror(errno));
	}
      r = bind(sock, _addrinfo->ai_addr, _addrinfo->ai_addrlen);
      if(r != 0)
	{
	  freeaddrinfo(_addrinfo);
	  close(sock);
	  throw Exception("Could not bind UDP socket with: %s:%d (%s)",
			  ip_address,port,strerror(errno));
	}
      f_sockets.push_back(sock);
      freeaddrinfo(_addrinfo);
    }
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
 
  f_isRunning = true;
  if(pthread_create(&f_readout_thread,&attr,ReadoutRoutine,this) != 0)
    {
      f_isRunning = false;
      throw Exception("Readout(): cannot create readout thread.");
    }
}

Readout::~Readout()
{
  f_isRunning = false;
  pthread_cancel(f_readout_thread);
  for(unsigned i = 0; i < f_sockets.size(); i++)
    close(f_sockets[i]);
  pthread_mutex_lock(f_mutex);
  while(!f_events.empty())
    {
      delete f_events.front();
      f_events.pop();
    }
  pthread_mutex_unlock(f_mutex);
  pthread_mutex_destroy(f_mutex);
  delete f_mutex;
}

void* Readout::ReadoutRoutine(void* p)
{
  Readout* server = (Readout*)p;
  fd_set s;
  try
    {
      while(server->f_isRunning)
	{
	  FD_ZERO(&s);
	  int fd = -1;
	  for(unsigned i = 0; i < server->f_sockets.size(); i++)
	    {
	      fd = server->f_sockets[i];
	      FD_SET(fd, &s);
	    }
	  int retval = select(fd + 1, &s, NULL, NULL, NULL);
	  if(retval > 0)
	    {
	      for(unsigned i = 0; i < server->f_sockets.size(); i++)
		{
		  int d = server->f_sockets[i];
		  if(FD_ISSET(d, &s))
		    {
		      Event* event = new Event;
		      int retCode = recv(d, event->buffer, UDP_PK_SIZE, MSG_DONTWAIT|MSG_OOB);
		      //printf("Packet received. retCode = %d\n", retCode);
		      if(retCode < 0)
			{
			  delete event;
			  throw Exception("Data reading error: %s",strerror(errno));
			}
		      if(retCode == 0)
			{
			  delete event;
			  throw Exception("Empty packet received.");
			}
		      if(retCode <= (int)UDP_PK_SIZE)
			{
			  N_READ_PACKETS++;
			  event->event_size = (uint32_t)retCode;
			  event->port = server->f_ports[i];			  
			  pthread_mutex_lock(server->f_mutex);
			  server->f_events.push(event);
			  pthread_mutex_unlock(server->f_mutex);
			}
		      else
			{
			  delete event;
			  throw Exception("Bad packet size (%d)",retCode);
			}
		    }
		}	  
	    }
	  else
	    {
	      throw Exception("Data polling error: %s",strerror(errno));
	    }
	}
    }
  catch(Exception& ex)
    {
      Message(ERROR,"Readout thread is canceled: %s",ex.what());
    }
  catch(...)
    {
      Message(ERROR,"Readout thread is canceled: unexpected error");
    }
  server->f_isRunning = false;
  return NULL;
}

Event* Readout::GetNextEvent()
{
  Event* event = NULL;
  if(!f_events.empty())
    {
      pthread_mutex_lock(f_mutex);
      event = f_events.front();
      f_events.pop();
      pthread_mutex_unlock(f_mutex);
    }
  return event;
}

void UDP_READOUT::Message(const int msg_type, const char* format,...)
{
  static int n_infos    = 0;
  static int n_warnings = 0;
  static int n_errors   = 0;
  char msg[1024];
  va_list argptr;
  va_start(argptr,format);
  vsprintf(msg,format,argptr);
  va_end(argptr);
  switch(msg_type)
    {
    case INFO:
      n_infos++;
      fprintf(stdout,"UDP INFO:    %s\n",msg);
      break;
    case WARNING:
      n_warnings++;
      fprintf(stderr,"UDP WARNING: %s\n",msg);
      break;
    case ERROR:
      n_errors++;
      fprintf(stderr,"UDP ERROR:   %s\n",msg);
      break;
    case SUMMARY:
      fprintf(stdout,"Summary:     n.warnings: %d\n"
    	      "             n.errors:   %d\n"
    	      "             %s\n"
    	      "             n.packets:  %d\n",
    	      n_warnings,n_errors,msg,N_READ_PACKETS);
      break;
    default:
      fprintf(stderr,"UDP UNKNOWN MESSAGE TYPE %d: %s\n",msg_type,msg);
      break;
    }
}

Exception::Exception(const char* format, ...)
{
  char msg[1024];
  va_list argptr;
  va_start(argptr,format);
  vsprintf(msg,format,argptr);
  va_end(argptr);
  *dynamic_cast<std::string*>(this) = msg;
}

