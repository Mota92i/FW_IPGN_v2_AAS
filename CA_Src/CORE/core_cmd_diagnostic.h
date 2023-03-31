//*****************************************************************************/
/// \file           core_cmd_diagnostic.h
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
//  Software configuration management
//*****************************************************************************/

//*****************************************************************************/
//  Inclusion checking
//*****************************************************************************/
#ifndef _CORE_CMD_DIAGNOSTIC_H_
#define _CORE_CMD_DIAGNOSTIC_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "ca_prototype.h"

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    DIAG_RESP_STATUS_OK_DELAY   = 1,
    DIAG_RESP_STATUS_OK         = 0,
    DIAG_RESP_STATUS_KO         = -1
};

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern void    voCoreCmdDiagnostic_Init(void);
extern int32_t CoreCmdDiagnostic_Process(Uartframe_t *psInframe, Uartframe_t *psOutframe);

#endif // _CORE_CMD_DIAGNOSTIC_H_
