#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <UDP_Readout.h>

const int      DEFAULT_PORT         = 60001;         // default UDP server port
const char*    DEFAULT_IP           = "10.16.6.103";  // default UDP server IP

static const char* IP = DEFAULT_IP;
static int       PORT = DEFAULT_PORT;

int USAGE(char* argv[])
{
  printf("USAGE: %s [-ip ip.add.re.ss]\n", argv[0]);
  printf("       default ip: %s\n", DEFAULT_IP);
  return 1;
}

int main(int argc, char* argv[])
{
  if(argc != 1)
    {
      for(int i = 1; i < argc; i++)
	{
	  if(strncmp(argv[i],"-?",2) == 0)
	    return USAGE(argv);
	  if(strncmp(argv[i],"-ip",3) == 0 && argc > i+1)
	    {
	      i++;
	      IP = argv[i];
	      continue;
	    }
	  else if(strncmp(argv[i],"-p",2) == 0 && argc > i+1)
	    {
	      i++;
	      PORT = atoi(argv[i]);
	      continue;
	    }
	  else
	    return USAGE(argv);
	}
    }
  else
    USAGE(argv);

  try
    {
      static int ports[] = { 60001, 60002, 60003, 60004, -1};
      UDP_READOUT::Readout readout(IP, ports);
      while(readout.IsRunning())
  	{
	  UDP_READOUT::Event* event = readout.GetNextEvent();
	  if(event)
	    {
	      uint32_t* data     = (uint32_t*)(&event->buffer[0]);
	      uint32_t  hw_size  = data[0];
	      uint32_t  ev_num   = data[1]&0x7FFFFFFF;
	      uint32_t  coarse_t = ((data[2]&0x7FFFFFFF) << 4);
	      uint32_t  se_t = ((data[2]&0x7FFFFFFF) << 4);
	      uint32_t  channel  = (uint32_t)(-1);
	      uint32_t  tdc_time = 0;
	      if(hw_size >= 5)
		channel = (data[3]>>24)&0x7777777F;
	      if(channel == 64)
		tdc_time = data[3]&0xFFFFFF;
	      printf("Event: %4u, card # %d, size: %u", ev_num, event->port%10+70, event->event_size);
	      if(channel < 65)
		{
		  printf(", ch = %2u, coarse_t = %9u, t = %6u", channel, coarse_t, tdc_time);
		}
	      for(unsigned i = 0; i < event->event_size; i++)
		{
		  if(i % 8 == 0)
		    printf("\n\t");
		  printf("%02X ", (unsigned)event->buffer[i]);
		}
	      printf("\n");
	      delete event;
	    }
	  else
	    usleep(100);
  	}
      UDP_READOUT::Message(UDP_READOUT::INFO, "Stopping...");
      UDP_READOUT::Message(UDP_READOUT::SUMMARY, "Bye!\n");
      exit(0);
    }
  catch(UDP_READOUT::Exception& ex)
    {
      UDP_READOUT::Message(UDP_READOUT::ERROR,ex.what());
    }
  catch(...)
    {
      UDP_READOUT::Message(UDP_READOUT::ERROR,"Unexpected error.");
    }
  return 1;  
}
