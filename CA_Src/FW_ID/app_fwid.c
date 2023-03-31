//*****************************************************************************/
/// \file           fw_id.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2018-10-11
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup        
/// \brief          
/// \details        
///                 
//*****************************************************************************/

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#define public
    #include "app_fwid.h"
#undef public

#define public extern
    #include <string.h>
    #include "drv_eeprom.h"
#undef public

/******************************************************************************/
//  constant declaration : Used by the bootloader to identify the binary file
/******************************************************************************/
//const uint8_t Fw_Version[16] @ "app_version" = "CA_ETH_0110";

//extern const uint16_t ielftool_checksum;
//const uint64_t  checksum_start @ "checksum_mark_start" = 0x8BADF00D1BADB002;
//const uint64_t  checksum_end   @ "checksum_mark_end"   = 0x02B0AD1B0DF0AD8B;

/******************************************************************************/
//  constant declaration : used by the application
/******************************************************************************/
const uint8_t G_APP_Fw_Version[] = "642  V2.20 du 08/06/15";
#define FW_VERSION_SIZE		(sizeof(G_APP_Fw_Version) - 1)

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
uint32_t FwId_GetVersion(uint8_t *p_Data)
{
    memcpy(p_Data, &G_APP_Fw_Version[0], FW_VERSION_SIZE);

    return( FW_VERSION_SIZE );
}
