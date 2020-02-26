/////////////////////////////////////////////////////////////////////////////
//  
//  [Filename]      dc_arb.c
//  [Description]   Send SCPI commands to LAN-connected AWG; change 
//                  burst parameters, in order to start waveform
//                  with external trigger; change trigger delay.
//                  Manual loop, arbitrary waveform (to be set
//                  with generator interface).
//                  - Minimum step: 1ns
//
//  [Instruction]   make loop
//  
//  [Author]        Alberto Occelli - alberto.occelli@edu.unito.it
//
//  [Language]      C++
//
//  [Created]       18/2/2020                                                
//  [Revised]       19/2/2020                                          
//  [Version]       1.1
//  
//  [Changelog]     1.0 created     
//                  1.1 bugfix      
//
/////////////////////////////////////////////////////////////////////////////

#include "alb_tools.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <csignal>

using namespace std;

#define CHECK(func)  do {ViStatus _s = (func);if (_s < 0){fprintf(stderr, "Error: %s returned %d\n", #func, _s);exit(0);}} while (0)

/**************** Specify the default address *****************************/

#define DEFAULT_LOGICAL_ADDRESS "GPIB1::10::INSTR"

ViRsrc TxtAddress = "TCPIP0::10.16.6.120::inst0::INSTR";
unsigned char buffer[100];
char SCPIcmd[11000];


/*
void interrupt(int);
*/


int main()
{   
    // Open communication and retrieve waveform generator informations
    iStart();
    
    //Reset the function generator
    reset();

/****************************************************** Default parameters ************************************************/   
    
    float sweep = 18;
    int width = 225;
    int w = 20 ;
    float s = 1;
    float phase;
    int timeout = 1; 
    int steps = 20;
    int shift;
    float phaseCompi = 0; 
     
    sweep = (s*360)/250;
    width = 250-w;
    float iPhase = -360*((float)w/250);

/*************************************************************************************************************************/
/*************************************************  setup channels  ******************************************************/
/*************************************************************************************************************************/

    fprintf(stderr,"\nSetting up channels... \n");
  
    setArbFunction(1);
    setArbFunction(2);

    burstStatus(1, true);
    burstStatus(2, true);

    burstSetting(1, 't', 'e');
    burstSetting(2, 't', 'e');
    
    output(1, true);
    output(2, true);
    
    int sw  = 0;
    char ch;
    char ch1;
            
    for(int i = 0; i<=40; i++){ 
        sw = i*50;
        if(sw!=0){
            trigDelay(1, sw);
        }
        printf("\nCurrent distance: %dns.\n-->Press enter to continue\n",sw);
        cin.ignore();
    }
    printf("\nCycle completed!\n");

return 0;
}
