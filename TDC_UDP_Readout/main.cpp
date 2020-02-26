#include <stdio.h> 
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <UDP_Readout.h>
#include "../utils/alb_tools.h"

using namespace std;

const int   DEFAULT_PORT    = 60001;                                // default UDP server port
const char* DEFAULT_IP      = "10.16.6.103";                        // default UDP server IP
ViRsrc      TxtAddress      = "TCPIP0::10.16.6.120::inst0::INSTR";  // waveform generator IP address

// Parameters (default)
int entries         = 50000;         // Entries
int takes           = 1;            // Number of reading cycles
int delay           = 13;           // Internal delay of the TDC
int step            = 50;                      
int dist            = 0;            // Distance between signals
int ncycles         = 8;            // Number of pulses in the burst
char dataPath[50]   = "root/test";  // Path of the readout data


static const char* IP = DEFAULT_IP; 
static int       PORT = DEFAULT_PORT;
char filename01[100], filename00[100], filenametd[100]; // Filenames

int USAGE(char* argv[])
{
  printf("USAGE: %s [-ip ip.add.re.ss]\n", argv[0]);
  printf("       default ip: %s\n", DEFAULT_IP);
  return 1;
}

int main(int argc, char* argv[]){

    signal(SIGINT, interrupt);

//----------------------------------------------------------------------------------------------------------
//------------------------------------- Initialize the generator--------------------------------------------
//----------------------------------------------------------------------------------------------------------
    iStart();   //Startup message
    reset();
    
    setArbFunction(1);
    setArbFunction(2);

    burstStatus(1, true);
    burstStatus(2, true);

    burstSetting(1, 't', 'e');
    burstSetting(2, 't', 'e');
    
    burstNCycles(1, ncycles);
    burstNCycles(2, ncycles);

    output(1, true);
    output(2, true);

    printf("\nPress enter to start the readout: \n");
    cin.ignore();

//----------------------------------------- The readout stuff----------------------------------------------

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

        int tk=1;   // current take
        while(readout.IsRunning() && tk<=takes){     // Start of the readout loop
            delay = 13;
            
            // Change generator settings 
            dist = (tk-1)*step;
            printf("\nSetting generator...\n\n");
            trigDelay(1, dist);                     
            sleep(1);
            
            // Open new output files (named after the experimental conditions) 
            snprintf(filename01, 100, "%s/data01_%02d_%04d.csv", dataPath, delay, dist);
            snprintf(filename00, 100, "%s/data00_%02d_%04d.csv", dataPath, delay, dist);
            snprintf(filenametd, 100, "%s/datatd_%02d_%04d.csv", dataPath, delay, dist);
            ofstream data01;
            ofstream data00;
            ofstream datatd;
            data01.open(filename01);
            data00.open(filename00);
            datatd.open(filenametd);
           
            int e01 = 0, e00 = 0, etd = 0;          // current entrie
            
            while(e01!=entries && e00!=entries){    // Readout 
                UDP_READOUT::Event* event = readout.GetNextEvent();
                if(event){
                    uint32_t*   data    = (uint32_t*)(&event->buffer[0]);
                    uint32_t    hw_size = data[0];
                    int         lines   = (int)((event->event_size)/4)-4;
                    
                    uint32_t  ev_num    = 0; 
                    uint32_t  coarse_t  = 0;
                    uint32_t  full_t    = 0;
                    uint32_t  channel   = 0;
                    uint32_t  fine_time = 0;
                    
                    //int firstc = 0;
                    //int secondc = 0;
                    int firstf[10] = {0}, secondf[10] = {0}, first[10] = {0}, second[10] = {0}, t00 = 0, t01 = 0;
                    
                    for(int i=0; i<lines; i++){ 
                        if(hw_size >= 5){
                            ev_num      = data[1]&0x7FFFFFFF;                        
                            channel     = (data[3+i]>>24)&0x7777777F;   
                            fine_time   = (data[3+i])&0x1F;             // size of the fine time: 5bits -> 0x1F, 4bits -> 0xF, 3bits -> 0x8, 2bits -> 0x4,...  
                            coarse_t    = ((data[3+i]&0xFFFFFF)>>5);     // coarse time mask
                            full_t      = (data[3+i])&0xFFFFFF; 
                        }
                        printf("Ev: %4u, card # %d, size: %u", ev_num, event->port%10+70, event->event_size);
                        if(channel < 65){
                            printf(", ch = %2u, full_t = %7u, coarse_t = %7u,  t = %3u\n", channel, full_t, coarse_t, fine_time);   // print time data
                            
                            if(channel==0 && e00 < entries){
                                e00++;
                                firstf[t00]=fine_time;
                                //firstc=coarse_t;
                                first[t00]=full_t;
                                t00++;
                            }
                            else if(channel == 1 && e01 < entries){
                                e01++;
                                secondf[t01]=fine_time;
                                //secondc=coarse_t;
                                second[t01]=full_t;
                                t01++;
                            }    
                        }   
                    }
                    cout<<endl; 
                    //printf("N. of lines = %d\n", t01);
                    
                    // Sort data
                    sortDescending(first, lines/2); 
                    sortDescending(second, lines/2);
                    // Write data to file
                    for(int k = 0; k<lines/2; k++){ 
                        cout    << "\tTime difference : "   << (first[k] - second[k]) <<" ";// print time difference
                        datatd  << -(second[k] - first[k])  << endl;                        // write time difference to file
                        data00  << firstf[k]    << endl;                                    // write fine time to file
                        data01  << secondf[k]   << endl;                                    // "   "   "   "   "   "
                        cout<<endl;
                        etd++;
                    }
                    for(unsigned i = 0; i < event->event_size; i++){    
                        if(i%8 == 0)
                            printf("\n\t");
                        printf("%02X ", (unsigned)event->buffer[i]);    // print raw data 
                    }
                    printf("\n\n---------------------------------------------------------------------------------------------\n");
                    //printf("\n\n Time left: %02d mins, %02d secs\n\n", ((entries-e01)/(60000), ((entries-e01)/1000)%60);
                    delete event;
                    }
                else
                    usleep(100);
            }
            data01.close(); // close files
            data00.close();
            datatd.close(); 
            printf("\nCycle n.%d of %d completed. TDC delay: %d; distance between signals: %dns", tk, takes, delay, dist);
            printf("\n\tData written to --> %s", filename00);
            printf("\n\tData written to --> %s", filename01);
            printf("\n\tData written to --> %s\n\n", filenametd);
            if(tk!=takes){
                printf("Proceeding to the next one... \n");
            }
        tk++;           // next take
        }
    
    // End of readout loop!
    cout<<endl;
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
