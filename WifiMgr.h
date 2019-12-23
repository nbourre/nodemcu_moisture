// This is file has been prepared by a cog script.

/// \file WifiMgr.h
/// \brief    DON'T FORGET TO DESCRIBE THE FILE HERE!
/// \author   Infinition - Nicolas Bourré
///

#ifndef WIFIMGR_H
#define WIFIMGR_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define SSID "YOUR_SSID"
#define PW "YOUR_PW"

////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
bool WifiMgr();
void WifiMgrTask();
bool WifiMgrConfigure();
bool WifiMgrConnect();

#endif
