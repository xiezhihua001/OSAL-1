/*******************************************************************************
  Filename:       OSAL_Flashutil32.c
  Revised:        $Date: 2013-05-09 21:41:33 -0700 (Thu, 09 May 2013) $
  Revision:       $Revision: 34219 $

  Description:    Utility functions to erase/write flash memory pages.
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "OSAL_Flashutil.h"

/*********************************************************************
 * MACROS
 */

/* Remainder when divided by 4 */
#define byte_offset(addr)               ((uint32)addr & 3)

/* Greatest-multiple-of-4 <= addr */
#define aligned_address(addr)           ((uint32)addr & ~3)

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * @fn      flash_write_word
 *
 * @brief   Writes 4bytes of data to address ulAddress
 *
 * @param   ulAddress - Address to which data has to be written
 *          address has to be 4byte-aligned.
 *
 * @param   data - 4byte data
 *
 * @return  none
 */
static void flash_write_word( uint32 *ulAddress, uint32 data )
{
  stm32_flash_write( (uint32_t)ulAddress, (const uint8_t *)&data, sizeof(uint32) );
}

/*********************************************************************
 * @fn      flashErasePage
 *
 * @brief   Erases the page pointed by addr
 *
 * @param   addr - Address of the page to be erased.
 *          addr has to be page aligned.
 *
 * @return  none
 */
void flashErasePage( uint8 *addr )
{
  halIntState_t IntState;

  HAL_ENTER_CRITICAL_SECTION( IntState );

  /* Erase flash */
  stm32_flash_erase( (uint32_t)addr, HAL_FLASH_PAGE_SIZE );
  HAL_EXIT_CRITICAL_SECTION( IntState );
}

/*********************************************************************
 * @fn      flashWrite
 *
 * @brief   Copies the data from buf(pointer) to
 * addr(pointer to flash memory). addr need not be aligned.
 * One can write the flash only in multiples of 4. The below logic is
 * required to implement data transfer of any number of bytes at any address
 *
 * @param  addr - To-address of the data
 * @param  len - Number of bytes to be transfered
 * @param  buf - From-address of the data
 *
 * @return len - None
 */
void flashWrite( uint8 *addr, uint16 len, uint8 *buf )
{
  if ( len > 0 )
  {
    /* 4-byte aligned pointer */
    uint32 *uint32ptr;
    /* 4-byte temporary variable */
    uint32 temp_u32;
    uint16 i = 0, j;
  
    /* start_bytes - unaligned byte count at the beggining
     * middle_bytes - aligned byte count at the middle
     * end_bytes - unaligned byte count at the end
     */
    uint16 start_bytes = 0, middle_bytes = 0, end_bytes = 0;
    halIntState_t IntState;
  
    /* Extract 4-byte aligned address */
    uint32ptr = (uint32 *)aligned_address(addr);
  
    /* Calculate the start_bytes */
    /* If the addr is not 4-byte aligned */
    if( byte_offset(addr) )
    {
      /* If the start-address and the end-address are in the
       * same 4-byte-aligned-chunk.
       */
      if((((uint32)addr) >> 2) == ((((uint32)addr) + len) >> 2))
      {
        start_bytes = len;
      }
      else
      {
        start_bytes = 4 - (byte_offset(addr));
      }
    }
  
    /* Calculate the middle_bytes and end_bytes */
    /* If there are any bytes left */
    if( (len - start_bytes) > 0 )
    {
      /* Highest-multiple-of-4 less than (len - start_bytes) */
      middle_bytes = ((len - start_bytes) & (~3));
      /* Remainder when divided by 4 */
      end_bytes = (len - start_bytes) & 3;
    }
  
    HAL_ENTER_CRITICAL_SECTION( IntState );
  
    /* Write the start bytes to the flash */
    if( start_bytes > 0 )
    {
      /* Take the first 4-byte chunk into a temp_u32 */
      temp_u32 = *uint32ptr;
      /* Write the required bytes into temp_u32 */
      for(; i < start_bytes; i++)
      {
        *(((uint8 *)(&temp_u32)) + i + byte_offset(addr)) = buf[i];
      }
      /* Write the 4-byte chunk into the flah */
      flash_write_word(uint32ptr, temp_u32);
      /* Increment the 4-byte-aligned-address by 4 */
      uint32ptr++;
    }
  
    /* Write the middle bytes to the flash */
    while( i < start_bytes + middle_bytes )
    {
      /* Extract 4 bytes into from the buf */
      *((uint8*)(&temp_u32)) = buf[i++];
      *((uint8*)(&temp_u32) + 1) = buf[i++];
      *((uint8*)(&temp_u32) + 2) = buf[i++];
      *((uint8*)(&temp_u32) + 3) = buf[i++];
  
      /* Write the 4-byte chunk into the flash */
      flash_write_word( uint32ptr, temp_u32 );
      /* Increment the 4-byte-aligned-address by 4 */
      uint32ptr++;
    }
  
    /* Write the end bytes to the flash */
    if( end_bytes > 0 )
    {
      j = 0;
      /* Take the first 4-byte chunk into a temp_u32 */
      temp_u32 = *uint32ptr;
      for(; i < len; i++)
      {
        *((uint8 *)&temp_u32 + j) = buf[i];
        j++;
      }
      /* Write the 4-byte chunk into the flash */
      flash_write_word( uint32ptr, temp_u32 );
    }
  
    HAL_EXIT_CRITICAL_SECTION( IntState );
  }
}

/*********************************************************************
*********************************************************************/
