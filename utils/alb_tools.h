/////////////////////////////////////////////////////////////////////////////
//  
//  [Filename]      albscpi.h
//  [Description]   Header file for SCPI commands  
//
//  [Instruction]   make loop
//  
//  [Author]        Alberto Occelli - alberto.occelli@edu.unito.it
//
//  [Language]      C++
//
//  [Created]       21/2/2020                                                
//  [Revised]       21/2/2020                                          
//  [Version]       1.0
//  
//  [Changelog]     1.0 created     
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <visa.h>
#include <csignal>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>


#define CHECK(func)  do {ViStatus _s = (func);if (_s < 0){fprintf(stderr, "Error: %s returned %d\n", #func, _s);exit(0);}} while (0)

extern ViSession viDefaultRM, Instrument;
extern ViRsrc TxtAddress;
extern ViUInt32 actual, retCount;

void interrupt(int);

void readErr(void);
void iStart(void);
void reset(void);
void setArbFunction(int);
void burstStatus(int, bool);
void burstSetting(int, char, char);
void burstNCycles(int, int);
void trigDelay(int, int);
void output(int, bool);

void sortAscending(int[], int);
void sortDescending(int[], int);
