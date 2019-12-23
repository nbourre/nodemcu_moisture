////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "TypeDefs.h"
#include "SystemTime.h"
#include "MoistSensorMgr.h"



////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Data types
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	APP_SM_INIT		= 0,		///< Application state - Initialization state.
	APP_SM_NORMAL,				///< Application state - Normal operation state.

} ApplicationStateTy;


typedef struct {
  bool isInit;


  // Modules
  poMoistSensorMgrTy  poMoistSensorMgr;

} oApplicationTy, *poApplicationTy;

////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
bool ApplicationInit();


////////////////////////////////////////////////////////////////////////////////
/// Local variables
////////////////////////////////////////////////////////////////////////////////
oApplicationTy oApplication = {false};


void setup() {

  //ApplicationInit();
  // For testing purpose, this line just crashes...
  SystemTimeInit();

}

void loop() {
  // put your main code here, to run repeatedly:

}


bool ApplicationInit() {
  bool bRet = false;

  if (!oApplication.isInit) {

    // Before anything, initialize the SystemTime utility.
    bRet = SystemTimeInit();
    if (!bRet) goto END;

    // // Once the system time is initialized, wait some time for electrical setup.
    // SystemTimeDelay(100);

    // // Initializing the moist sensor to D8
    // oApplication.poMoistSensorMgr = MoistSensorMgr(D8);
    // if (oApplication.poMoistSensorMgr == NULL) {
    //   goto END;
    // }

    // bRet = MoistSensorMgrConfigure(oApplication.poMoistSensorMgr);
    // if (!bRet) goto END;

  }

  bRet = true;
END:
  return bRet;

}
