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
#include <csignal>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>

ViSession viDefaultRM, Instrument;
ViUInt32 actual, retCount;


void interrupt(int signum){
    printf("\n\n********");
    printf("\n* STOP *");
    printf("\n********\n");
    CHECK(viClose(Instrument));
    CHECK(viClose(viDefaultRM));
    exit(signum); 
}


void readErr(void){
    char Scmd[10000];
    unsigned char buf[100];
    printf("**********************************************************************************");
    strcpy(Scmd, "SYST:ERR?\n");
    CHECK(viWrite(Instrument, (unsigned char*)Scmd, (ViUInt32)strlen(Scmd),&actual));
    CHECK(viRead(Instrument, buf, 100, &retCount));
    printf("\n%*s", retCount, buf);       
    printf("**********************************************************************************\n\n");
}

void iStart(void){
    char Scmd[10000];
    unsigned char buf[100];
    // Open communication
    CHECK(viOpenDefaultRM(&viDefaultRM)); 
    CHECK(viOpen(viDefaultRM, TxtAddress, VI_NULL, VI_NULL, &Instrument));
    // Specify long seconds timeout for waveform download
    CHECK(viSetAttribute(Instrument, VI_ATTR_TMO_VALUE, 400000));
    printf("**********************************************************************************");
    strcpy(Scmd, "*IDN?\n");
    CHECK(viWrite(Instrument, (unsigned char*)Scmd, (ViUInt32)strlen(Scmd),&actual));
    CHECK(viRead(Instrument, buf, 100, &retCount));
    printf("\nAWG informations: %*s", retCount, buf);       
    printf("**********************************************************************************\n\n");
}


void setArbFunction(int source) {
    char Scmd[10000];
    snprintf(Scmd, 10000, "MMEM:LOAD:DATA%d \"INT:\\User Files\\NIM.arb\" \n", source);
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
    snprintf(Scmd, 10000, "SOUR%d:FUNC:ARB \"INT:\\User Files\\NIM.arb\" \n", source); 
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));  
    snprintf(Scmd, 10000, "SOUR%d:FUNC ARB \n", source); 
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));  
}

void reset(void){
    char Scmd[10000];
    fprintf(stderr, "Resetting generator");
    strcpy(Scmd,"*RST\n"); 
    CHECK(viWrite(Instrument, (unsigned char*) Scmd, (ViUInt32)strlen(Scmd), &actual));
    strcpy(Scmd,"*CLS\n"); /* Clear errors and status registers */
    CHECK(viWrite(Instrument, (unsigned char*)Scmd, (ViUInt32)strlen(Scmd),&actual));
    strcpy(Scmd, "DATA:VOL:CLEAR\n");
    CHECK(viWrite(Instrument, (unsigned char*)Scmd, (ViUInt32)strlen(Scmd),&actual));
    for(int j=0; j<3; j++){
        sleep(1);
        fprintf(stderr, ".");
        }
    sleep(1);
    fprintf(stderr, " done\n");
}

void burstStatus(int source, bool status){
    char Scmd[10000];
    if(status)
        snprintf(Scmd, 10000, "SOUR%d:BURS:STAT ON \n", source);
    else
        snprintf(Scmd, 10000, "SOUR%d:BURS:STAT OFF \n", source);
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
}


void burstSetting(int source, char mode = 't', char sourceb = 'i'){
    //default: triggered with external source
    char Scmd[10000];
    switch(mode){
        case 't':   // Triggered mode
            snprintf(Scmd, 10000, "SOUR%d:BURS:MODE TRIG \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        case 'g':   // Gated mode
            snprintf(Scmd, 10000, "SOUR%d:BURS:MODE GATed \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        default:    // No match->Triggered mode
            snprintf(Scmd, 10000, "SOUR%d:BURS:MODE TRIG \n", source);            
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
    }
    switch(sourceb){
        case 'i':   // Immediate
            snprintf(Scmd, 10000, "TRIG%d:SOUR IMM \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        case 'e':   // External
            snprintf(Scmd, 10000, "TRIG%d:SOUR EXT \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        case 't':   // Timer
            snprintf(Scmd, 10000, "TRIG%d:SOUR TIM \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        case 'b':   // Bus
            snprintf(Scmd, 10000, "TRIG%d:SOUR BUS \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
        default:   // No match->Immediate source
            snprintf(Scmd, 10000, "TRIG%d:SOUR IMM \n", source); 
            CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
            break;
    }
}

void burstNCycles(int source, int ncycles){
    char Scmd[10000];
    snprintf(Scmd, 10000, "SOUR%d:BURS:NCYC %d \n", source, ncycles); 
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
}

void trigDelay(int source, int value){
    char Scmd[10000];
    snprintf(Scmd, 10000, "TRIGger%d:DELay %iE-09", source, value);
    CHECK(viWrite(Instrument,(unsigned char*) Scmd, (ViUInt32)strlen(Scmd),&actual));
}

void output(int source, bool status){
    char Scmd[10000];
    if(status)
        snprintf(Scmd, 10000, "OUTPut%d ON\n", source);
    else
        snprintf(Scmd, 10000, "OUTPut%d OFF\n", source);
    CHECK(viWrite(Instrument, (unsigned char*)Scmd, (ViUInt32)strlen(Scmd),&actual));
}

// Other functions
void sortAscending(int a[], int n) {
    int i, j, min, temp;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (a[j] < a[min])
                min = j;
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

void sortDescending(int a[], int n) {
    int i, j, min, temp;
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (a[j] > a[min])
                min = j;
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}
