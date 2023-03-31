//*****************************************************************************/
/// \file           core_cmd_diagnostic.c
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
//  Included header file list
//*****************************************************************************/
#define public
    #include "core_cmd_diagnostic.h"
#undef public

#define public extern
    #include <string.h>
    #include <stdbool.h>
    #include "os_msg_queue.h"
    #include "task_main.h"
    #include "task_app.h"
    #include "task_display.h"

    #include "common_function.h"
    #include "core_cmd_diagnostic_moteur.h"
    #include "core_spec.h"
    #include "app_fwid.h"
    #include "protocol_modbus_diagnostic.h"
    #include "core_compteur_horaire.h"
	#include "core_voltmetre.h"

    #include "drv_board_gpio.h"
#undef public

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
#define	BASE_CODE_APPAREIL		( 0x19 )	// Type d'appareil XXXX
#define	BASE_CODE_APPAREIL_S	( 0x00 )	// Type d'appareil en mode Séquenceur = 0 (pas present)

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static uint8_t g_u8CodeAppareil           = 0;	// BASE_CODE_APPAREIL + EETYPE
static uint8_t g_u8CodeAppareilSequenceur = 0;	// BASE_CODE_APPAREIL_S + EETYPE

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static int32_t PC_ReadFwRev 	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_WriteSn   	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_WriteCpth 	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
//static int32_t PC_ReadCpth(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_ReadSpec  	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_WriteSpec 	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_Etalonnage	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
//static int32_t PC_ReadEtalonnage  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
//static int32_t PC_WriteEtalonnage (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_ReadMeasure	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);
static int32_t PC_SimuMeasure	  (t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize);

static void voSwapSpecByteOrder(t_specialisation *psSpec);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static int32_t PC_ReadFwRev(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
    // Protocol reply
    pOutFrame->reply_0xfc.u16Address = cmon_U8ArrayToU16LSB(&pInFrame->common.address[0]);
	pOutFrame->reply_0xfc.u16nb_word = cmon_U8ArrayToU16LSB(&pInFrame->common.nb_word[0]);
	FwId_GetVersion(&pOutFrame->reply_0xfc.vers_firmware[0]);

	//
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xfc);

    return( DIAG_RESP_STATUS_OK );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static int32_t PC_WriteSn(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	t_device_info sInfo;

    // Copy
	memcpy(&sInfo, &G_DeviceCfg.sInfo, sizeof(t_device_info));

	// Update type and sn
	sInfo.u8DeviceType = pInFrame->cmd_0xf8.u8Type;
	CmonFct_SwapLittleBigEndian((uint8_t *)&pInFrame->cmd_0xf8.u16SerialNumber, (uint8_t *)&sInfo.u16SerialNumber, sizeof(sInfo.u16SerialNumber));

	// Appareil est sequenceur
#if( BASE_CODE_APPAREIL_S != 0 )
	if( sInfo.u8DeviceType >= BASE_CODE_APPAREIL_S )
		sInfo.u8DeviceType = sInfo.u8DeviceType - BASE_CODE_APPAREIL_S;
#else
	sInfo.u8DeviceType = sInfo.u8DeviceType - BASE_CODE_APPAREIL;
#endif

	//
	if( CoreSpec_DeviceInfoWrite(&sInfo) != -1 )
	{
		voCoreCmdDiagnostic_Init();

		// Build reply : overwrite, because specific
		pOutFrame->common.fct  		 = 0xFE;

		return( DIAG_RESP_STATUS_OK );
	}

	return( DIAG_RESP_STATUS_KO );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static int32_t PC_WriteCpth(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	int32_t  Error;
	uint32_t u32Cpth;

    // Protocol reply
	u32Cpth = cmon_U8ArrayToU32MSB(&pInFrame->cmd_0xec.u8Cpth[0]);
	Error = s32CoreCompteurHoraire_Init(u32Cpth);

    // Build reply
	if( Error == -1 )
		pOutFrame->reply_0xec.u8Status = 1;
	else
		pOutFrame->reply_0xec.u8Status = 0;
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xec);

	return( DIAG_RESP_STATUS_OK );
}

static int32_t PC_ReadCpth(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	uint32_t u32Cpth;

    // Protocol reply
    pOutFrame->reply_0xeb.u16Address = cmon_U8ArrayToU16MSB(&pInFrame->common.address[0]);
	pOutFrame->reply_0xeb.u16nb_word = cmon_U8ArrayToU16MSB(&pInFrame->common.nb_word[0]);

	u32Cpth = u32ReadCptValue();
	cmon_U32ToU8ArrayMSB(u32Cpth, &pOutFrame->reply_0xeb.u8Cpth[0]);

	//
	*pu16OutFrameSize += 4 + sizeof(pOutFrame->reply_0xeb);

    return( DIAG_RESP_STATUS_OK );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static void voSwapSpecByteOrder(t_specialisation *psSpec)
{
	CmonFct_SwapLittleBigEndianVar((uint8_t *)&psSpec->u32Type, 	sizeof(psSpec->u32Type));
	CmonFct_SwapLittleBigEndianVar((uint8_t *)&psSpec->u32Rayon, 	sizeof(psSpec->u32Rayon));
	CmonFct_SwapLittleBigEndianVar((uint8_t *)&psSpec->s32Course4,  sizeof(psSpec->s32Course4));
	CmonFct_SwapLittleBigEndianVar((uint8_t *)&psSpec->s32Course20, sizeof(psSpec->s32Course20));
	CmonFct_SwapLittleBigEndianVar((uint8_t *)&psSpec->s32Offset,   sizeof(psSpec->s32Offset));
}

static int32_t PC_ReadSpec(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	t_specialisation *psSpec = &pOutFrame->reply_0xf5.sSpec;

    // Protocol reply
    pOutFrame->reply_0xf5.u16Address = cmon_U8ArrayToU16LSB(&pInFrame->common.address[0]);
	pOutFrame->reply_0xf5.u16nb_word = cmon_U8ArrayToU16LSB(&pInFrame->common.nb_word[0]);

	memcpy(psSpec, &G_DeviceCfg.sSpec, sizeof(G_DeviceCfg.sSpec));
	voSwapSpecByteOrder( psSpec );

	//
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf5);

    return( DIAG_RESP_STATUS_OK );
}

static int32_t PC_WriteSpec(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	int32_t  Error;
	t_specialisation *psSpec = &pInFrame->cmd_0xf4.sSpec;

	voSwapSpecByteOrder( psSpec );
	Error = CoreSpec_SpecWrite( psSpec );

	// Build reply
	if( Error == -1 )
		pOutFrame->reply_0xf4.u8Status = 1;
	else
		pOutFrame->reply_0xf4.u8Status = 0;
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf4);

	return( DIAG_RESP_STATUS_OK );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static int32_t PC_Etalonnage(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	uint8_t  u8Status;
	uint32_t u32Voie;

	CmonFct_SwapLittleBigEndian((uint8_t *)&pInFrame->cmd_0xf0.u32Voie, (uint8_t *)&u32Voie, sizeof(u32Voie));

	u8Status = (uint8_t)s8CoreVoltmetre_SetEtalonnage(pInFrame->cmd_0xf0.u8Cmd, (uint16_t)u32Voie);

	pOutFrame->reply_0xf0.u8Status = u8Status;
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf0);

	return( DIAG_RESP_STATUS_OK );
}
#if 0
static int32_t PC_ReadEtalonnage(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	t_specialisation *psSpec = &pOutFrame->reply_0xf5.sSpec;

    // Protocol reply
    pOutFrame->reply_0xf5.u16Address = cmon_U8ArrayToU16LSB(&pInFrame->common.address[0]);
	pOutFrame->reply_0xf5.u16nb_word = cmon_U8ArrayToU16LSB(&pInFrame->common.nb_word[0]);

	memcpy(psSpec, &G_DeviceCfg.sSpec, sizeof(G_DeviceCfg.sSpec));
	voSwapSpecByteOrder( psSpec );

	//
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf5);

    return( DIAG_RESP_STATUS_OK );
}

static int32_t PC_WriteEtalonnage(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	int32_t  Error;
	t_specialisation *psSpec = &pInFrame->cmd_0xf4.sSpec;

	voSwapSpecByteOrder( psSpec );
	Error = CoreSpec_SpecWrite( psSpec );

	// Build reply
	if( Error == -1 )
		pOutFrame->reply_0xf4.u8Status = 1;
	else
		pOutFrame->reply_0xf4.u8Status = 0;
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf4);

	return( DIAG_RESP_STATUS_OK );
}
#endif
//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-09-29
///
//----------------------------------------------------------------------------*/
static int32_t PC_ReadMeasure(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	t_app_measure    sAppMeasure;
	t_motor_position sMotorPosition;
	int16_t   s16Temp;

    // Protocol reply
    pOutFrame->reply_0xf6.u16Address = cmon_U8ArrayToU16LSB(&pInFrame->common.address[0]);
	pOutFrame->reply_0xf6.u16nb_word = cmon_U8ArrayToU16LSB(&pInFrame->common.nb_word[0]);

    //
    pOutFrame->reply_0xf6.u8ModeFct = u8TaskApp_GetModeFct();

    //
    voTaskApp_GetSMeasure( &sAppMeasure );

    // Sa Measure
    if( sAppMeasure.sMeasureSa.s8Status == false )
    	s16Temp = 0x8000;
    else
    	s16Temp = (int16_t)cmon_s32RoundFloatToInt(sAppMeasure.sMeasureSa.f32Value);

    CmonFct_SwapLittleBigEndian((uint8_t *)&s16Temp, (uint8_t *)&pOutFrame->reply_0xf6.s16MesureSa, sizeof(pOutFrame->reply_0xf6.s16MesureSa));

    // Sa Measure Course
    if( sAppMeasure.sMotorAngle.s8Valid == ANGLE_ERROR )
    	s16Temp = 0x8000;
    else
    	s16Temp = (int16_t)cmon_s32RoundFloatToInt( (float)(sAppMeasure.sMotorAngle.f32Course * 10) );

    CmonFct_SwapLittleBigEndian((uint8_t *)&s16Temp, (uint8_t *)&pOutFrame->reply_0xf6.s16Course, sizeof(pOutFrame->reply_0xf6.s16Course));

    // Sa Measure Angle
    if( sAppMeasure.sMotorAngle.s8Valid == ANGLE_ERROR )
    	s16Temp = 0x8000;
    else
    	s16Temp = (int16_t)cmon_s32RoundFloatToInt( sAppMeasure.sMotorAngle.f32Angle );

    CmonFct_SwapLittleBigEndian((uint8_t *)&s16Temp, (uint8_t *)&pOutFrame->reply_0xf6.s16Angle, sizeof(pOutFrame->reply_0xf6.s16Angle));

    //
    voTaskApp_GetMotorPosition(&sMotorPosition);
	CmonFct_SwapLittleBigEndian((uint8_t *)&sMotorPosition.u16Angle, (uint8_t *)&pOutFrame->reply_0xf6.u16AngleMot, sizeof(pOutFrame->reply_0xf6.u16AngleMot));

	//
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xf6);

    return( DIAG_RESP_STATUS_OK );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static int32_t PC_SimuMeasure(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	t_app_measure_msg	sMsgMeasure;

	sMsgMeasure.eEventType 			= APP_MEASURE_MSG_QUEUE_SIMU;
	sMsgMeasure.sMeasureSa.f32Value = (float)((uint16_t)(pInFrame->cmd_0xe1.u8Measure[0] << 8) | pInFrame->cmd_0xe1.u8Measure[1]);
	sMsgMeasure.sMeasureSa.s8Status	= true;
	OsMsgQueue_SendToBack(OS_MSG_QUEUE_MEASURE, &sMsgMeasure);

	// Build reply
	pOutFrame->reply_0xe1.u8Status = 0;		// 0 : mode simulation ok, 1 : mode simulation refusé
	*pu16OutFrameSize += sizeof(pOutFrame->reply_0xe1);

    return( DIAG_RESP_STATUS_OK );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static int32_t PC_ReglageSa(t_frame_fct_PC_cmd *pInFrame, t_frame_fct_PC_reply *pOutFrame, uint16_t *pu16OutFrameSize)
{
	int32_t  Error = DIAG_RESP_STATUS_KO;
	uint16_t u16Data;

	if( intCoreCmdDiagnosticMoteur(pInFrame->cmd_0xe0.u8Cmd, &u16Data) == 0 )
	{
		// Build reply
		cmon_U16ToU8ArrayMSB(u16Data, &pOutFrame->reply_0xe0.u8ValeurDeReglage[0]);

		//
		*pu16OutFrameSize += sizeof(pOutFrame->reply_0xe0);

		Error = DIAG_RESP_STATUS_OK;
	}

    return( Error );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
void voCoreCmdDiagnostic_Init(void)
{
	g_u8CodeAppareilSequenceur = 0;
	g_u8CodeAppareil      	   = 0;

	if( BASE_CODE_APPAREIL != 0 )
		g_u8CodeAppareil = BASE_CODE_APPAREIL + G_DeviceCfg.sInfo.u8DeviceType;

 	if( BASE_CODE_APPAREIL_S != 0 )
 		g_u8CodeAppareilSequenceur = BASE_CODE_APPAREIL_S + G_DeviceCfg.sInfo.u8DeviceType;
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
#define FUNCTION_EXEC_ALLOWED_1		( 1 )
#define FUNCTION_EXEC_ALLOWED_2		( 2 )
#define FUNCTION_EXEC_ALLOWED_3		( 4 )
int32_t CoreCmdDiagnostic_Process(Uartframe_t *psInframe, Uartframe_t *psOutframe)
{ 
    const struct {
        uint8_t  u8CodeFct;
        uint16_t u16DataLen;
        int32_t  (*pfunc)(t_frame_fct_PC_cmd *, t_frame_fct_PC_reply *, uint16_t *);
        uint8_t  u8ExecAllowed;
    }sData[] = { { FCT_READ_CPTH,    	 0x0040, PC_ReadCpth,  	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_WRITE_CPTH,   	 0x0004, PC_WriteCpth, 	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_WRITE_SN,     	 0x0003, PC_WriteSn,   	 (FUNCTION_EXEC_ALLOWED_1) },
				 { FCT_READ_FW_REV,  	 0x0016, PC_ReadFwRev, 	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_READ_SPEC,    	 0x0014, PC_ReadSpec,  	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_WRITE_SPEC,   	 0x0014, PC_WriteSpec, 	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_ETALONNAGE,  	 0x0005, PC_Etalonnage,  (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_READ_ETALONNAGE,  0x0014, PC_ReadSpec,  	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_WRITE_ETALONNAGE, 0x0014, PC_WriteSpec, 	 (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_READ_MEASURE,     0x0009, PC_ReadMeasure, (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_SIMU_MEASURE,	 0x0002, PC_SimuMeasure, (FUNCTION_EXEC_ALLOWED_2) },
				 { FCT_REGLAGE_SA,		 0x0003, PC_ReglageSa,   (FUNCTION_EXEC_ALLOWED_2) },
               };
    uint8_t   u8Cpt;
    uint16_t  u16Temp;
    uint8_t   u8FctCode;
    uint16_t  u16SerialNb;
    uint16_t  u16NbOfWord;
    int32_t   Error = -1;
    uint8_t   u8ExecAllowed;
    t_frame_fct_PC_cmd   *pInFrame  = (t_frame_fct_PC_cmd *)psInframe->pu8Buf;
	t_frame_fct_PC_reply *pOutFrame = (t_frame_fct_PC_reply *)psOutframe->pu8Buf;

    //--------------------------------------------------------------------------
    // Retrieve ADDRESS : it must be 0x0000 for all request
    //--------------------------------------------------------------------------  
    u16Temp = cmon_U8ArrayToU16MSB(&pInFrame->common.address[0]);
    if( u16Temp != 0x0000 )
        return( -1 );

    //--------------------------------------------------------------------------
    // Retrieve some data
    //--------------------------------------------------------------------------
    u16SerialNb = cmon_U8ArrayToU16MSB(&pInFrame->common.serial_nb[0]);
    u16NbOfWord = cmon_U8ArrayToU16MSB(&pInFrame->common.nb_word[0]);
    u8FctCode   = pInFrame->common.fct;

    //--------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------
    if( (pInFrame->common.type == 0x00) && (u16SerialNb == 0x0000) )
    {
    	u8ExecAllowed = FUNCTION_EXEC_ALLOWED_1;

    	// Output
        pOutFrame->common.fct  		= pInFrame->common.fct;
        pOutFrame->common.type 		= 0x00;
        cmon_U16ToU8ArrayMSB(0x0000, &pOutFrame->common.serial_nb[0]);
        psOutframe->u16Len          += sizeof(pOutFrame->common) - 1; // moins le tag
    }
    else if( (pInFrame->common.type == g_u8CodeAppareil) && (u16SerialNb == G_DeviceCfg.sInfo.u16SerialNumber) )
	{
    	u8ExecAllowed = FUNCTION_EXEC_ALLOWED_2;

    	// Output
        pOutFrame->common.fct  		= pInFrame->common.fct;
        pOutFrame->common.type 		= g_u8CodeAppareil;
        cmon_U16ToU8ArrayMSB(G_DeviceCfg.sInfo.u16SerialNumber, (uint8_t *)&pOutFrame->common.serial_nb);
        psOutframe->u16Len          += sizeof(pOutFrame->common) - 1; // moins le tag
	}
    else if( (pInFrame->common.type == g_u8CodeAppareil) && (u16SerialNb == 0x0000) )
	{
    	u8ExecAllowed = FUNCTION_EXEC_ALLOWED_3;
	}
    else
    {
    	u8ExecAllowed = 0;
    }

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    if( u8ExecAllowed != 0 )
    {
		for(u8Cpt = 0; u8Cpt < (sizeof(sData) / sizeof(sData[0])); u8Cpt++)
		{
			if( sData[u8Cpt].u8CodeFct == u8FctCode )
			{
				if( ((sData[u8Cpt].u16DataLen == u16NbOfWord) || (sData[u8Cpt].u16DataLen == 0xFFFF)) &&
					((sData[u8Cpt].u8ExecAllowed & u8ExecAllowed) != 0)								  &&
					(sData[u8Cpt].pfunc != NULL) )
				{
					Error = sData[u8Cpt].pfunc(pInFrame, pOutFrame, &psOutframe->u16Len);
				}

				break;
			}
		}
    }

    return( Error );
} 
