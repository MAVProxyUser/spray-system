;/**
;  ******************* (C) COPYRIGHT 2011 DJI **********************************
;  *
;  * @Project Name       £ºBL_WKM2_LED_IAP.uvproj
;  * @File Name          : critical.asm
;  * @Environment        : keil mdk4.12/LPC1765/100M cclock
;  * @Author&Date        : 2011-07-28 
;  * @Version            : 1.00
;  ******************************************************************************
;  * @Description
;  *	    Begginning of application   
;  */

                                                                ; Functions declared in this file.
    EXPORT  CPU_SR_Save
    EXPORT  CPU_SR_Restore



;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

    AREA |.text|,CODE,READONLY,ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8	

;********************************************************************************************************
;                                      CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
;               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
;               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR cpu_sr);
;
; Note(s)     : (1) These functions are used in general like this :
;
;                       void  Task (void  *p_arg)
;                       {
;                           CPU_SR_ALLOC();                     /* Allocate storage for CPU status register */
;                               :
;                               :
;                           CPU_CRITICAL_ENTER();               /* cpu_sr = CPU_SR_Save();                  */
;                               :
;                               :
;                           CPU_CRITICAL_EXIT();                /* CPU_SR_Restore(cpu_sr);                  */
;                               :
;                       }
;********************************************************************************************************

CPU_SR_Save
        MRS     R0, PRIMASK                     ; Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR


CPU_SR_Restore                                  ; See Note #2.
        MSR     PRIMASK, R0
        BX      LR

	END
;/*******************  (C) COPYRIGHT 2011 DJI ************END OF FILE***********/

