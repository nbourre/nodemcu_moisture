// This is file has been prepared by a cog script.

/// \file MoistSensorMgr.c
/// \brief    Moisture Sensor Manager
/// \author   Infinition - Nicolas Bourré
///

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Arduino.h"
#include "MoistSensorMgr.h"
#include "SystemTime.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define MOISTURE_DELAY 15000
#define POLL_DELAY 100
#define POLLING_TIME 10 * POLL_DELAY

#define SERIAL_DELAY 1000

#define MAP_MAX 1024
#define MAP_MIN 350


////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////
enum moisture_SM {BOOTING, WAITING, POLLING, REPORTING};


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
void booting_state(UINT32);
void waiting_state(UINT32);
void polling_state(UINT32);
void reporting(UINT32);

////////////////////////////////////////////////////////////////////////////////
/// Local variables
////////////////////////////////////////////////////////////////////////////////
oMoistSensorMgrTy oMoistSensorMgr			= {FALSE};

enum moisture_SM current_state = BOOTING;

UINT32 moisture_acc = MOISTURE_DELAY;
int polling_time_acc = 0;
int poll_acc = 0;
int serial_acc = 0;

int poll_count = 0;

UINT32 pT = 0;
UINT32 cT = 0;
UINT32 dT = 0;

UINT16 moisture_average = 0;
UINT16 moisture_min = 0;
UINT16 moisture_max = 1024;
UINT16 moisture_sum = 0;

bool is_dirty = false;

////////////////////////////////////////////////////////////////////////////////
/// \brief 		TODO DESCRIPTION HERE
/// \public
///
/// \return		TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////

poMoistSensorMgrTy MoistSensorMgr(UINT8 u8PinNumber)
{
	poMoistSensorMgrTy this = &oMoistSensorMgr;

	this->u8Pin = u8PinNumber;

	this->bNewResultAvail = false;

	this->u16CurrentValueRaw = 0;

	this->u8CurrentValue = 0; 	
	this->u8MaximumValue = 0;
	this->u8MinimumValue = MAX_VAL_UINT32; ///< Values are inverted for moisture sensor
	this->u8AverageValue = 0;

	this->u16ReadingInterval = MOISTURE_DELAY;
    this->u16PollingInterval = POLL_DELAY;
    this->u16PollingDuration = POLLING_TIME;


	return this;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief 		TODO DESCRIPTION HERE
/// \public
///
/// \return		TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////

bool MoistSensorMgrConfigure(poMoistSensorMgrTy this)
{
	bool bRet = false;

	this->bIsConfigured = true;

	pinMode(this->u8Pin, OUTPUT);


	cT = SystemTimeGetTime();
	pT = 0;


	bRet = true;
END:
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief 		TODO DESCRIPTION HERE
/// \public
///
/// \return		TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////

bool MoistSensorMgrTask()
{
	bool bRet = false;

	cT = SystemTimeGetTime();
	dT = cT - pT;
	pT = cT;

	switch (current_state)
	{
	case BOOTING:
    	booting_state(dT);
		break;
	case WAITING:
		waiting_state(dT);
		break;
	case POLLING:
		polling_state(dT);
    break;
	}

	reporting(dT);

	bRet = true;
END:
	return bRet;
}

void booting_state(UINT32 dT) {
  digitalWrite (oMoistSensorMgr.u8Pin, true);
  oMoistSensorMgr.u16CurrentValueRaw = analogRead(A0);
  digitalWrite (oMoistSensorMgr.u8Pin, false);

}

void waiting_state(UINT32 delta) {
  moisture_acc += (delta & 0xFFFF);
  
  if (moisture_acc >= oMoistSensorMgr.u16ReadingInterval) {
    moisture_acc = 0;

    current_state = POLLING;
    digitalWrite (oMoistSensorMgr.u8Pin, true);
  }
}

void polling_state(UINT32 delta) {
  poll_acc += (delta & 0xFFFF);
  
  polling_time_acc += (delta & 0xFFFF);

  if (poll_acc >= oMoistSensorMgr.u16PollingInterval) {
    poll_acc = 0;
    poll_count++;
    
    oMoistSensorMgr.u16CurrentValueRaw = analogRead(A0);    

    // Values are inverted, that's the reason
    // for the inverted comparators
    if (oMoistSensorMgr.u16CurrentValueRaw < moisture_max) {
      moisture_max = oMoistSensorMgr.u16CurrentValueRaw;
    }
  
    if (oMoistSensorMgr.u16CurrentValueRaw > moisture_min) {
      moisture_min = oMoistSensorMgr.u16CurrentValueRaw;
    }

    moisture_sum += oMoistSensorMgr.u16CurrentValueRaw;    
  }

  if (polling_time_acc >= oMoistSensorMgr.u16PollingDuration) {
    polling_time_acc = 0;
    
    if (poll_count > 0) {
      moisture_average = moisture_sum / poll_count;
      moisture_sum = 0;
      poll_count = 0;
      digitalWrite (oMoistSensorMgr.u8Pin, false);
    }
    
    is_dirty = true;
  }
}

void reporting (UINT32 dT) {
	if (is_dirty) {
		is_dirty = false;
    	current_state = WAITING;

		oMoistSensorMgr.u8CurrentValue = map (oMoistSensorMgr.u16CurrentValueRaw, MAP_MAX, MAP_MIN, 0, 100);		
		oMoistSensorMgr.u8AverageValue = map (moisture_average, MAP_MAX, MAP_MIN, 0, 100);
		oMoistSensorMgr.u8AverageValue = map (moisture_max, MAP_MAX, MAP_MIN, 0, 100);
		oMoistSensorMgr.u8AverageValue = map (moisture_min, MAP_MAX, MAP_MIN, 0, 100);

		oMoistSensorMgr.bNewResultAvail = true;		
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief 		MoistSensorMgrIsNewResultAvail - Check if a new processed result is available.
/// \public
///
/// \param[out]	pbNewResultAvail	A new result is available or not.
///
/// \return		TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
bool MoistSensorMgrIsNewResultAvail(BOOL* pbNewResultAvail) {

	if (oMoistSensorMgr.bIsConfigured && pbNewResultAvail)
	{
		*pbNewResultAvail 	= oMoistSensorMgr.bNewResultAvail;

		if (oMoistSensorMgr.bNewResultAvail)
		{
			oMoistSensorMgr.bNewResultAvail 	= false;
		}

		return true;
	}

	return false;	
}
