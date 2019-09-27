/**************************************************************************************************
  Filename:       osport.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "OSAL_Clock.h"

/*********************************************************************
 * MACROS
 */

#define TICK_IN_MS 1 /* 1 millisecond */ 

/*********************************************************************
 * EXTERN FUNCTIONS
 */

extern int __putchar(int ch);

 /*********************************************************************
  * FUNCTIONS
  */

/***************************************************************************************************
 * @fn      SysTickIntEnable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntEnable(void)
{
    return;
}


/***************************************************************************************************
 * @fn      SysTickIntDisable
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntDisable(void)
{
    return;
}

/*******************************************************************************
 * @fn          halSleep
 *
 * @brief       This function is called from the OSAL task loop using and
 *              existing OSAL interface. It determines if an OSAL timer is
 *              pending, in which case it sets up the RTC to wake the device
 *              for that event, and puts the device to sleep. The OSAL timers
 *              are adjusted upon wake in case the device was awoken sooner due
 *              to some other interrupt. If no OSAL timer event is pending,
 *              then the device is put to sleep.
 *
 *              Note: Presently, only CM3 is powered down.
 *
 * input parameters
 *
 * @param       osal_timeout - Next OSAL timer timeout, in msec.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void halSleep( uint32 osal_timeout )
{
    //Sleep the task for the specified duration
    return;
}

/***************************************************************************************************
 * @fn      SysTickIntHandler
 *
 * @brief   The Systick Interrupt module
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void SysTickIntHandler(void)
{
  /* Update OSAL timer and clock */
  osalAdjustTimer(TICK_IN_MS);
}

/***************************************************************************************************
 * @fn      _putchar
 *
 * @brief   put char to console
 *
 * @param   None
 *
 * @return  None
 ***************************************************************************************************/
void _putchar(char character)
{
  // send char to console etc.
  int c = (int)character;

  if (c == '\n')
    __putchar('\r');
  __putchar(c);
}