///
/// \file     SystemTime.h
/// \brief    SystemTime Utility Peripheral module
/// \author   NOVO - Dany Thiffeault, ing. (134853)
///

#ifndef SYSTEMTIME_H_
#define SYSTEMTIME_H_


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "TypeDefs.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define SYSTEMTIME_RTC_YEAR_MIN     2015    ///< Minimum RTC year (default at first startup).
#define SYSTEMTIME_RTC_YEAR_MAX     2200    ///< Maximum RTC year (virtual).
#define SYSTEMTIME_RTC_MONTH_MIN    1     ///< Minimum RTC month.
#define SYSTEMTIME_RTC_MONTH_MAX    12      ///< Maximum RTC month per year.
#define SYSTEMTIME_RTC_DAY_MIN      1     ///< Minimum RTC day.
#define SYSTEMTIME_RTC_DAY_MAX      31      ///< Maximum RTC day per month.
#define SYSTEMTIME_RTC_HOUR_MIN_24    0     ///< Minimum RTC hour (24h format).
#define SYSTEMTIME_RTC_HOUR_MAX_24    23      ///< Maximum RTC hour per day (24h format).
#define SYSTEMTIME_RTC_HOUR_MIN_12    1     ///< Minimum RTC hour (12h format).
#define SYSTEMTIME_RTC_HOUR_MAX_12    12      ///< Maximum RTC hour per day (12h format).
#define SYSTEMTIME_RTC_MINUTE_MIN   0     ///< Minimum RTC minute.
#define SYSTEMTIME_RTC_MINUTE_MAX   59      ///< Maximum RTC second per hour.
#define SYSTEMTIME_RTC_SECOND_MIN   0     ///< Minimum RTC second.
#define SYSTEMTIME_RTC_SECOND_MAX   59      ///< Maximum RTC second per minute.


////////////////////////////////////////////////////////////////////////////////
// Type definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Prototype
////////////////////////////////////////////////////////////////////////////////
BOOL  SystemTimeInit();
void  SystemTimeDelay(unsigned long tick);
UINT32  SystemTimeGetTime();
UINT32  SystemTimeGetTimeDiff(UINT32 u32SysTimeToCompare);

// RTC related.
BOOL  SystemTimeRTCInit();
BOOL  SystemTimeRTCIsInit();
BOOL  SystemTimeRTCGetDate(UINT16* pu16Year, UINT8* pu8Month, UINT8* pu8Day);
BOOL  SystemTimeRTCGetTime(UINT8* pu8Hour, UINT8* pu8Minute, UINT8* pu8Second);
BOOL  SystemTimeRTCGetDateTime(UINT16* pu16Year, UINT8* pu8Month, UINT8* pu8DayMonth, UINT8* pu8Hour, UINT8* pu8Minute, UINT8* pu8Second);
BOOL  SystemTimeRTCSetDate(UINT16 u16Year, UINT8 u8Month, UINT8 u8Day);
BOOL  SystemTimeRTCSetTime(UINT8 u8Hour, UINT8 u8Minute, UINT8 u8Second);
BOOL  SystemTimeRTCSetDateTime(UINT16 u16Year, UINT8 u8Month, UINT8 u8DayMonth, UINT8 u8Hour, UINT8 u8Minute, UINT8 u8Second);

#endif
