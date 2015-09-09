#pragma once
// StationList.h : Code to maintain the TMC station List
//
#ifndef _STATIONLIST_H
#define _STATIONLIST_H

namespace navman
{

typedef struct tStationList
{
  WORD stnEntries;
  WORD stnIndex;
  WORD stnCurrent;
  WORD stnFrequency;
  WORD stnPI;
  WORD stnSignal;
  WORD stnLocationTable;
  WORD stnODA;
  WORD stnTMC;
  WORD removeCounter;
  char stnName[20];
} tStationList;

typedef enum 
{
  tunerNoHardware = 0,
  tunerCheckModule,
  tunerListInit,
  tunerListComplete,
  tunerIdle,
  tunerListUpdate,
  tunerListSend
} tTunerState;

typedef struct tSignalStrength
{
  WORD stnPI;
  WORD stnFrequency;
  WORD stnSignal;
} tSignalStrength;

void initStationList(UINT8 fullScan);

void processTunerMessageWasabi(BYTE *msg, WORD len);
void processTunerMessageGNS(BYTE *msg, WORD len);

tTunerState tunerTick(BYTE *command, bool *toSend);

WORD tunedFrequency(void);

bool retrieveStationListEntry(int index, tStationList *listEntry);

void updateStationList(void);

UINT8 frequencyInStationList(WORD frequency);

void setToFrequency(WORD frequency);

UINT8 stationListChanged(void);

WORD loadStationsFromFile(void);
WORD saveStationsToFile(void);

UINT8 signalStrengthUpdate(tSignalStrength *signal);

UINT8 updateTMCFlag(WORD piCode, WORD flag);

bool  isNavmanModule(void);

/**
  Sets the station scan level sensitivity. Allowable values are 1, 2 and 3
  @param newValue Specifies the new sensitivity level
*/
void stationlist_setScanSensitivity(UINT32 newValue);

/**
  Sets the RDS time. Allowable values are 1, 2, 5, 10 and 30 seconds.
  @param newValue Specifies the new RDS time (in milliseconds)
*/
void stationlist_setRDSTime(UINT32 newValue);

/**
Sets the RDS data time.
@param newValue Specifies the new RDS data time (in milliseconds)
*/
void stationlist_setRDSDataTime(UINT32 newValue);

/**
  Sets the minimum signal strength
  @param newValue Specifies the new signal strength
*/
void stationlist_setMinSignalStrength(UINT32 newValue);

#define COMMAND_BUFFER_SIZE 30

extern const UINT8 hexChars[];

}

#endif
