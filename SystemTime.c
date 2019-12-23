///
/// \file 		SystemTime.c
/// \brief 		SystemTime Utility Peripheral module
/// \author 	NOVO - Dany Thiffeault, ing. (134853)
///

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "SystemTime.h"
//#include "NBSP.h"

#ifdef RTC_H
#include "NRTC.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define SYSTEMTIME_RTC_INIT_FIRST_STEP_DELAY_MS		2100	// In ms. Required by MCU, according to the User Manual.
#define SYSTEMTIME_RTC_INIT_STEPS_DELAY_MS			1001	// In ms. Required to avoid lockdown


////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////
///
/// \struct	oSystemTimeTy
/// \brief 	SystemTime object.
///
/// Object defining the SystemTime module.
typedef struct
{
  BOOL		  bIsInitialized;				///< Flag indicating if this object is configured and ready to use.
#ifdef RTC_H
	poNRTCTy	poNRTC;						///< Pointer to the RTC object instance.
#endif
	UINT8		  u8RTCInitStep;				///< Needed to avoid system lock (MCU requirement).
	UINT32		u32TimeStampLastRTCInit;	///< Timestamp for RTC initialization. Needed to add time in-between RTC init steps to avoid system lock (MCU requirement)
	
}oSystemTimeTy, *poSystemTimeTy;


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
oSystemTimeTy	oSystemTime = {FALSE};		///< System time object instance.

extern volatile unsigned long timer0_overflow_count;


////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeInit - Initializes the system time module.
/// \public
/// \details	This function initializes the time base that will reference the
///				system time going forward. It defines the units for all other
///				time references in the application.
///
/// \return 	TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeInit()
{
	BOOL bRet = FALSE;
	
	if (!oSystemTime.bIsInitialized)
	{
#ifdef RTC_H
		oSystemTime.poNRTC 					= NULL;
#endif
		oSystemTime.u32TimeStampLastRTCInit = 0;
		oSystemTime.u8RTCInitStep			= 0;

#ifdef NBSP_H
		// If not already done, initialize the BSP since it's needed for system time.
		bRet = NBSPInit();
		if (!bRet)
		{
			goto END;
		}
#endif
		
		oSystemTime.bIsInitialized = TRUE;
		bRet = TRUE;
	}
	else
	{
		bRet = TRUE;
	}
		
END:
	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeGetTime - Gets the current system time.
/// \public
///
/// \return 	The current system time.
////////////////////////////////////////////////////////////////////////////////
UINT32 SystemTimeGetTime()
{
	if (oSystemTime.bIsInitialized)
	{
		return millis();
	}
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeGetTimeDiff - Gets the time difference between current
///				system time and the argument.
/// \public
/// \details	This function handles the possible wrap-around when calculating
///				the difference. Note that the reference time in argument must
///				use the same units as the system time.
///
/// \param[in] 	u32SysTimeToCompare 	The time to compare with the current
///										system time.
///
/// \return 	The time difference.
////////////////////////////////////////////////////////////////////////////////
UINT32 SystemTimeGetTimeDiff(UINT32 u32SysTimeToCompare)
{
	UINT32 u32CurrentSysTime 	= 0;
	UINT32 u32SysTimeDiff		= 0;
	
	if (oSystemTime.bIsInitialized)
	{
		u32CurrentSysTime = SystemTimeGetTime();
		
		// Check is a wrap-around occured.
		if (u32CurrentSysTime >= u32SysTimeToCompare)
		{
			u32SysTimeDiff = u32CurrentSysTime - u32SysTimeToCompare;
		}
		else
		{
			u32SysTimeDiff = (MAX_VAL_UINT32 - u32SysTimeToCompare) + u32CurrentSysTime;
		}
	}
	
	return u32SysTimeDiff;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeDelay - Delay the caller for a specific time.
/// \public
/// \details	This function is useful when the caller wants to wait some
///				time before continuing its behavior. For example, waiting
///				a specific amount of time between two actions.
///
/// \param[in] 	tick 	Number of tick to wait (block). A tick equals to 1 unit
///						of the time base. For example, if the time base is 1ms,
///						a tick is 1 ms.
////////////////////////////////////////////////////////////////////////////////
void SystemTimeDelay(unsigned long tick)
{
	unsigned long systickcnt;

	if (oSystemTime.bIsInitialized)
	{
		systickcnt = SystemTimeGetTime();
		while (SystemTimeGetTimeDiff(systickcnt) < tick);
	}
	else
	{
		// ERROR !
		while(1);
	}
}

#ifdef RTC_H
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCInit - Initializes the RTC time.
/// \public
///
/// \return 	TRUE if success, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCInit()
{
	BOOL 	bRet 		= FALSE;
	UINT32	u32TimeDiff	= 0;
	
	if (oSystemTime.bIsInitialized)
	{

		// If available, initialize the RTC. Don't validate a NULL pointer, because
		// it is possible to have that if RTC is not supported in the current application.
		if (!oSystemTime.poNRTC)
		{
			oSystemTime.poNRTC = NRTC(NRTC_0);
		}

		
		if (oSystemTime.poNRTC && (SystemTimeRTCIsInit() == FALSE))
		{
			// First step. Activate the RTC clock.
			if (oSystemTime.u8RTCInitStep == 0)
			{
				NBSPEnableRTCClock();
					
				// Update the timestamp and move to the next step.
				oSystemTime.u32TimeStampLastRTCInit = SystemTimeGetTime();
				++oSystemTime.u8RTCInitStep;
			}
			else if (oSystemTime.u8RTCInitStep == 1)
			{
				// Wait the required delay for this step.
				u32TimeDiff = SystemTimeGetTimeDiff(oSystemTime.u32TimeStampLastRTCInit);
				if (u32TimeDiff > SYSTEMTIME_RTC_INIT_FIRST_STEP_DELAY_MS)
				{
					bRet = NRTCConfigure(oSystemTime.poNRTC);
					if (!bRet) goto END;
					
					// Update the timestamp and move to the next step.
					oSystemTime.u32TimeStampLastRTCInit = SystemTimeGetTime();
					++oSystemTime.u8RTCInitStep;
				}
			}
			else
			{
				u32TimeDiff = SystemTimeGetTimeDiff(oSystemTime.u32TimeStampLastRTCInit);
				if (u32TimeDiff > SYSTEMTIME_RTC_INIT_STEPS_DELAY_MS)
				{
					bRet = NRTCConfigure(oSystemTime.poNRTC);
					if (!bRet) goto END;
					
					// Final step. Just update the timestamp.
					oSystemTime.u32TimeStampLastRTCInit = SystemTimeGetTime();
				}
			}
		}


		bRet = TRUE;
	}
		
END:
	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCIsInit - Check if the RTC is initialized.
/// \public
///
/// \return		TRUE if RTC is initialized, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCIsInit()
{
	BOOL bRet 		= FALSE;
	BOOL bIsInit 	= FALSE;

	if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
	{
		bRet = NRTCIsConfigured(oSystemTime.poNRTC, &bIsInit);
		if (bRet)
		{
			bRet = bIsInit;
		}
	}

	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCGetDate - Get the current RTC date.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[out] pu16Year 	Current RTC year.
/// \param[out] pu8Month 	Current RTC month of the year.
/// \param[out] pu8Day 		Current RTC day of the month.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCGetDate(UINT16* pu16Year, UINT8* pu8Month, UINT8* pu8Day)
{
	BOOL 	bRet 		= FALSE;
	UINT16 	u16DayYear	= 0;
	UINT8 	u8DayWeek	= 0;
	
	if (pu16Year && pu8Month && pu8Day)
	{
		// Only use RTC if it is available in the current application.
		if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
		{
			bRet = NRTCGetAllDate(oSystemTime.poNRTC, pu16Year, pu8Month, &u16DayYear, pu8Day, &u8DayWeek);
		}
		else
		{
			*pu16Year 	= SYSTEMTIME_RTC_YEAR_MIN;
			*pu8Month 	= SYSTEMTIME_RTC_MONTH_MIN;
			*pu8Day 	= SYSTEMTIME_RTC_DAY_MIN;
		}
	}
	
	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCGetTime - Get the current RTC time.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[out] pu8Hour 	Current RTC hour of the day.
/// \param[out] pu8Minute 	Current RTC minute of the hour.
/// \param[out] pu8Second 	Current RTC second of the minute.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCGetTime(UINT8* pu8Hour, UINT8* pu8Minute, UINT8* pu8Second)
{
	BOOL bRet = FALSE;

	if (pu8Hour && pu8Minute && pu8Second)
	{
		// Only use RTC if it is available in the current application.
		if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
		{
			bRet = NRTCGetAllTime(oSystemTime.poNRTC, pu8Hour, pu8Minute, pu8Second);
		}
		else
		{
			*pu8Hour	= SYSTEMTIME_RTC_HOUR_MIN_24;
			*pu8Minute	= SYSTEMTIME_RTC_MINUTE_MIN;
			*pu8Second	= SYSTEMTIME_RTC_SECOND_MIN;
		}
	}

	
	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCGetDateTime - Get the current RTC date and time.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[out] pu16Year 	Current RTC year.
/// \param[out] pu8Month 	Current RTC month of the year.
/// \param[out] pu8DayMonth	Current RTC day of the month.
/// \param[out] pu8Hour 	Current RTC hour of the day.
/// \param[out] pu8Minute 	Current RTC minute of the hour.
/// \param[out] pu8Second 	Current RTC second of the minute.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCGetDateTime(UINT16* pu16Year, UINT8* pu8Month, UINT8* pu8DayMonth, UINT8* pu8Hour, UINT8* pu8Minute, UINT8* pu8Second)
{
	BOOL bRet = FALSE;
  

	if (pu16Year && pu8Month && pu8DayMonth && pu8Hour && pu8Minute && pu8Second)
	{

		// Only use RTC if it is available in the current application.
		if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
		{
			bRet = NRTCGetDateTime(oSystemTime.poNRTC, pu16Year, pu8Month, pu8DayMonth, pu8Hour, pu8Minute, pu8Second);
		}
		else
		{
			*pu16Year		= SYSTEMTIME_RTC_YEAR_MIN;
			*pu8Month		= SYSTEMTIME_RTC_MONTH_MIN;
			*pu8DayMonth	= SYSTEMTIME_RTC_DAY_MIN;
			*pu8Hour		= SYSTEMTIME_RTC_HOUR_MIN_24;
			*pu8Minute		= SYSTEMTIME_RTC_MINUTE_MIN;
			*pu8Second		= SYSTEMTIME_RTC_SECOND_MIN;

		}
	}



	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCSetDate - Set the current RTC date.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[in] 	u16Year 	New RTC year.
/// \param[in] 	u8Month 	New RTC month of the year.
/// \param[in] 	u8Day 		New RTC day of the month.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCSetDate(UINT16 u16Year, UINT8 u8Month, UINT8 u8Day)
{
	BOOL bRet = FALSE;

	// Only use RTC if it is available in the current application.
	if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
	{
		bRet = NRTCSetSimpleDate(oSystemTime.poNRTC, u16Year, u8Month, u8Day);
	}

	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCSetTime - Set the current RTC time.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[in] 	u8Hour 		New RTC hour of the day.
/// \param[in] 	u8Minute 	New RTC minute of the hour.
/// \param[in] 	u8Second 	New RTC second of the minute.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCSetTime(UINT8 u8Hour, UINT8 u8Minute, UINT8 u8Second)
{
	BOOL bRet = FALSE;

	// Only use RTC if it is available in the current application.
	if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
	{
    
		bRet = NRTCSetAllTime(oSystemTime.poNRTC, u8Hour, u8Minute, u8Second);
    
	}

	return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// \brief 		SystemTimeRTCSetDateTime - Set the current RTC date and time.
/// \public
/// \details	This function is only available if the RTC is used in the
///				application.
///
/// \param[in]	u16Year 	New RTC year.
/// \param[in] 	u8Month 	New RTC month of the year.
/// \param[in] 	u8DayMonth	New RTC day of the month.
/// \param[in] 	u8Hour 		New RTC hour of the day.
/// \param[in] 	u8Minute 	New RTC minute of the hour.
/// \param[in] 	u8Second 	New RTC second of the minute.
///
/// \return		TRUE if RTC is available, FALSE otherwise.
////////////////////////////////////////////////////////////////////////////////
BOOL SystemTimeRTCSetDateTime(UINT16 u16Year, UINT8 u8Month, UINT8 u8DayMonth, UINT8 u8Hour, UINT8 u8Minute, UINT8 u8Second)
{
	BOOL bRet = FALSE;

	// Only use RTC if it is available in the current application.
	if (oSystemTime.bIsInitialized && oSystemTime.poNRTC)
	{
		bRet = NRTCSetDateTime(oSystemTime.poNRTC, u16Year, u8Month, u8DayMonth, u8Hour, u8Minute, u8Second);
	}

	return bRet;
}

#endif
