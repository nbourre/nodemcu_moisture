// This is file has been prepared by a cog script.

/// \file MoistSensorMgr.h
/// \brief    DON'T FORGET TO DESCRIBE THE FILE HERE!
/// \author   Infinition - Nicolas Bourré
///



#ifndef MOISTSENSORMGR_H
#define MOISTSENSORMGR_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "Arduino.h"
#include "TypeDefs.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	bool			bIsConfigured;					///< Flag indicating that the module is configured or not.

    UINT16          u16ReadingInterval;
    UINT16          u16PollingInterval;
    UINT16          u16PollingDuration;
	
    // Hardware configuration
    UINT8           u8Pin;

 	// Housekeeping results.
	bool			bNewResultAvail;				///< Flag indicating that a new processed result is available.
	UINT16			u16CurrentValueRaw;			    ///< The last processed raw value.
    UINT8			u8CurrentValue;			        ///< The last processed value.
	UINT8			u8MaximumValue;				    ///< The last processed maximum value.
	UINT8			u8MinimumValue;				    ///< The last processed minimum value.
    UINT8			u8AverageValue;				    ///< The last processed average value.

} oMoistSensorMgrTy, *poMoistSensorMgrTy;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
poMoistSensorMgrTy MoistSensorMgr(UINT8 pin);
bool MoistSensorMgrTask();
bool MoistSensorMgrConfigure(poMoistSensorMgrTy);
bool MoistSensorMgrIsNewResultAvail(bool* pbNewResultAvail);

#endif
