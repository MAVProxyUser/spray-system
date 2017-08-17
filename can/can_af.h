/**
  ******************** (C) COPYRIGHT 2011 DJI **********************************
  *
  * @Project Name       : WKM2_CAN_LOADER.uvproj
  * @File Name          : can_af.h
  * @Environment        : keil mdk4.12/LPC1765/100M cclock
  * @Author&Date        : 2011-08-31 
  * @Version            : 1.10
  ******************************************************************************
  * @Description
  *	      
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_AF_H__
#define __CAN_AF_H__

/* Includes ------------------------------------------------------------------*/
#include <LPC17xx.h>

/* Private define ------------------------------------------------------------*/
#define MSG_ENABLE				        ((CPU_INT08U)(0))
#define MSG_DISABLE				        ((CPU_INT08U)(1))
#define CAN_CTL1                        (0)
#define CAN_CTL2                        (1) 

/* CHECK PARAMETER DEFINITIONS  ----------------------------------------------*/ 
/* Macro to determine if it is valid CAN peripheral or not */
#define PARAM_CANx(x)			((((CPU_INT32U*)x)==((CPU_INT32U *)LPC_CAN1)) || (((CPU_INT32U*)x)==((CPU_INT32U *)LPC_CAN2)))

/*	Macro to determine if it is valid CANAF or not*/
#define PARAM_CANAFx(x)			(((uint32_t*)x)== ((uint32_t*)LPC_CANAF))

/*	Macro to determine if it is valid CANAF RAM or not*/
#define PARAM_CANAFRAMx(x)		(((uint32_t*)x)== (uint32_t*)LPC_CANAF_RAM)

/** Macro define for struct AF_Section parameter */
#define PARAM_CTRL(n)	        ((n==CAN_CTL1)|(n==CAN_CTL2))

/*	Macro to determine if it is valid CANCR or not*/
#define PARAM_CANCRx(x)			(((uint32_t*)x)==((uint32_t*)LPC_CANCR))

/** Macro to check Frame Identifier */
#define PARAM_ID_11(n)			((n>>11)==0) /*-- 11 bit --*/
#define PARAM_ID_29(n)			((n>>29)==0) /*-- 29 bit --*/

/** Macro define for struct AF_Section parameter */
#define PARAM_MSG_DISABLE(n)	((n==MSG_ENABLE)|(n==MSG_DISABLE))

/** Macro to check ID format type */
#define PARAM_ID_FORMAT(n)		((n==STD_ID_FORMAT)||(n==EXT_ID_FORMAT))

/** CAN configuration structure */
/***********************************************************************
 * CAN device configuration commands (IOCTL commands and arguments)
 **********************************************************************/
/**
 * @brief CAN ID format definition
 */
typedef enum {
	STD_ID_FORMAT = 0, 	/**< Use standard ID format (11 bit ID) */
	EXT_ID_FORMAT = 1	/**< Use extended ID format (29 bit ID) */
} CAN_ID_FORMAT_Type;

/**
 * @brief AFLUT Entry type definition
 */
typedef enum {
	FULLCAN_ENTRY = 0,
	EXPLICIT_STANDARD_ENTRY,
	GROUP_STANDARD_ENTRY,
	EXPLICIT_EXTEND_ENTRY,
	GROUP_EXTEND_ENTRY,
} AFLUT_ENTRY_Type;

/**
 * @brief Symbolic names for type of CAN message
 */
typedef enum {
	DATA_FRAME = 0, 	/**< Data frame */
	REMOTE_FRAME = 1	/**< Remote frame */
} CAN_FRAME_Type;

/**
 * @brief Error values that functions can return
 */
typedef enum {
	CAN_OK = 1, 				/**< No error */
	CAN_OBJECTS_FULL_ERROR, 	/**< No more rx or tx objects available */
	CAN_FULL_OBJ_NOT_RCV, 		/**< Full CAN object not received */
	CAN_NO_RECEIVE_DATA, 		/**< No have receive data available */
	CAN_AF_ENTRY_ERROR, 		/**< Entry load in AFLUT is unvalid */
	CAN_CONFLICT_ID_ERROR, 		/**< Conflict ID occur */
	CAN_ENTRY_NOT_EXIT_ERROR	/**< Entry remove outo AFLUT is not exit */
} CAN_ERROR;

/**
 * @brief FullCAN Entry structure
 */
typedef struct {
	CPU_INT08U controller;		/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT08U disable;		/**< Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	CPU_INT16U id_11;			/**< Standard ID, should be 11-bit value */
} FullCAN_Entry;

/**
 * @brief Standard ID Frame Format Entry structure
 */
typedef struct {
	CPU_INT08U controller; 	/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT08U disable; 		/**< Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	CPU_INT16U id_11; 		/**< Standard ID, should be 11-bit value */
} SFF_Entry;

/**
 * @brief Group of Standard ID Frame Format Entry structure
 */
typedef struct {
	CPU_INT08U controller1; 	/**< First CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT08U disable1; 		/**< First Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0)
								 - MSG_DISABLE: disable bit = 1
							*/
	CPU_INT16U lowerID; 		/**< ID lower bound, should be 11-bit value */
	CPU_INT08U controller2; 	/**< Second CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT08U disable2; 		/**< Second Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0
								 - MSG_DISABLE: disable bit = 1
							*/
	CPU_INT16U upperID; 		/**< ID upper bound, should be 11-bit value and
								 equal or greater than lowerID
							*/
} SFF_GPR_Entry;

/**
 * @brief Extended ID Frame Format Entry structure
 */
typedef struct {
	CPU_INT08U controller; 	/**< CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT32U ID_29; 		/**< Extend ID, shoud be 29-bit value */
} EFF_Entry;


/**
 * @brief Group of Extended ID Frame Format Entry structure
 */
typedef struct {
	CPU_INT08U controller1; 	/**< First CAN Controller, should be:
								 - CAN1_CTRL: CAN1 Controller
								 - CAN2_CTRL: CAN2 Controller
							*/
	CPU_INT08U controller2; 	/**< Second Disable bit, should be:
								 - MSG_ENABLE: disable bit = 0(default)
								 - MSG_DISABLE: disable bit = 1
							*/
	CPU_INT32U lowerEID; 		/**< Extended ID lower bound, should be 29-bit value */
	CPU_INT32U upperEID; 		/**< Extended ID upper bound, should be 29-bit value */
} EFF_GPR_Entry;

/**
 * @brief Acceptance Filter Section Table structure
 */
typedef struct {
	FullCAN_Entry* FullCAN_Sec; 	/**< The pointer point to FullCAN_Entry */
	CPU_INT08U FC_NumEntry;			/**< FullCAN Entry Number */
	SFF_Entry* SFF_Sec; 			/**< The pointer point to SFF_Entry */
	CPU_INT08U SFF_NumEntry;			/**< Standard ID Entry Number */
	SFF_GPR_Entry* SFF_GPR_Sec; 	/**< The pointer point to SFF_GPR_Entry */
	CPU_INT08U SFF_GPR_NumEntry;		/**< Group Standard ID Entry Number */
	EFF_Entry* EFF_Sec; 			/**< The pointer point to EFF_Entry */
	CPU_INT08U EFF_NumEntry;			/**< Extended ID Entry Number */
	EFF_GPR_Entry* EFF_GPR_Sec; 	/**< The pointer point to EFF_GPR_Entry */
	CPU_INT08U EFF_GPR_NumEntry;		/**< Group Extended ID Entry Number */
} AF_SectionDef;

/* Function declaration ------------------------------------------------------*/
 
void CAN_InitAFTable(void);
CAN_ERROR CAN_SetupAFLUT(LPC_CANAF_TypeDef* CANAFx, AF_SectionDef* AFSection);
CAN_ERROR CAN_LoadExplicitEntry(LPC_CAN_TypeDef* CANx, uint32_t id, CAN_ID_FORMAT_Type format);
CAN_ERROR CAN_LoadFullCANEntry (LPC_CAN_TypeDef* CANx, uint16_t id);
CAN_ERROR CAN_LoadGroupEntry(LPC_CAN_TypeDef* CANx, uint32_t lowerID, uint32_t upperID, CAN_ID_FORMAT_Type format);
CAN_ERROR CAN_RemoveEntry(AFLUT_ENTRY_Type EntryType, uint16_t position);

/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/
#endif
