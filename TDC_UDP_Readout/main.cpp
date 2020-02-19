#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <UDP_Readout.h>

using namespace std;

const int      DEFAULT_PORT         = 60001;         // default UDP server port
const char*    DEFAULT_IP           = "10.16.6.103";  // default UDP server IP

char filename00[100];
char filename32[100];
char filenametd[100];


int entries = 50000;
int e00 = 0;
int e32 = 0;
int etd = 0;

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

int delay = 13;
int dist = 0;

printf("\nInsert the delay: \n");
cin>>delay;
printf("\n\nInsert the distance (ns): \n");
cin>>dist;

snprintf(filename00, 100, "root/data200219/data00_%d_%04d.csv", delay, dist);
snprintf(filename32, 100, "root/data200219/data32_%d_%04d.csv", delay, dist);
snprintf(filenametd, 100, "root/data200219/datatd_%d_%04d.csv", delay, dist);

ofstream data00;
ofstream data32;
ofstream datatd;

data00.open(filename00);
data32.open(filename32);
datatd.open(filenametd);

if(argc != 1){
    for(int i = 1; i < argc; i++){
        if(strncmp(argv[i],"-?",2) == 0)
	    return USAGE(argv);
        if(strncmp(argv[i],"-ip",3) == 0 && argc > i+1){
	        i++;
	        IP = argv[i];
            continue;
        }
	    else if(strncmp(argv[i],"-p",2) == 0 && argc > i+1){
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

try{
    static int ports[] = { 60001, 60002, 60003, 60004, -1};
    UDP_READOUT::Readout readout(IP, ports);
    while(readout.IsRunning()){
	    UDP_READOUT::Event* event = readout.GetNextEvent();
	    if(event){
            uint32_t* data     = (uint32_t*)(&event->buffer[0]);
	        uint32_t  hw_size  = data[0];
            int lines = (int)((event->event_size)/4)-4;
	        
            uint32_t  ev_num   = data[1]&0x7FFFFFFF;
	        uint32_t  coarse_t = {((data[2]&0x7FFFFFFF) << 4)};
	        uint32_t  channel  = (uint32_t)(-1);
	        uint32_t  fine_time = 0;
            
            int first = 0;
            int second = 0;
	        
            for(int i=0; i<lines; i++){ 
                if(hw_size >= 5){
		            channel = (data[3+i]>>24)&0x7777777F;
                    fine_time=(data[3+i])&0x1F; 
                    coarse_t=(data[3+i]>>4)&0xFFFFFF; 
                }
          /*   if(channel == 64)}
		    tdc_time = data[3]&0xFFFFFF;*/
	            printf("Event: %4u, card # %d, size: %u", ev_num, event->port%10+70, event->event_size);
	            if(channel < 65){
		            printf(", ch = %2u, coarse_t = %9u, t = %6u\n", channel, coarse_t, fine_time);
                    
                    if(channel==0 && e00 < entries){
                        data00 << fine_time << endl;
                        e00++;
                        first=coarse_t;
                    }
                    else if(channel == 32 && e32 < entries){
                        data32 << fine_time << endl;
                        e32++;
                        second=coarse_t;
                    }    
                }   
            }
            cout<< first - second <<" ";
            datatd << second-first<<endl;
            
            cout<<endl;

            for(unsigned i = 0; i < event->event_size; i++){
                if(i%8 == 0)
                    printf("\n\t");
                printf("%02X ", (unsigned)event->buffer[i]);
                etd++;
            }
	        printf("\n\n Time left: %02d mins, %02d secs\n\n", ((entries-e00)/6000), ((entries-e00)/100)%60);
	        delete event;
	            
            if(e00==entries && e32==entries)
                break;
            }
        else
	        usleep(100);
  	}

    data00.close();
    data32.close();
    datatd.close();
    
    UDP_READOUT::Message(UDP_READOUT::INFO, "Stopping...");
    UDP_READOUT::Message(UDP_READOUT::SUMMARY, "Bye!\n");
    
    exit(0);
    }
catch(UDP_READOUT::Exception& ex){
    UDP_READOUT::Message(UDP_READOUT::ERROR,ex.what());
}
catch(...){
    UDP_READOUT::Message(UDP_READOUT::ERROR,"Unexpected error.");
}
    return 1;
}

                   
