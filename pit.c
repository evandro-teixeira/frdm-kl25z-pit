/*
 * pit.c
 *
 *  Created on: 18/04/2017
 *      Author: Evandro
 */

#include "pit.h"

/*****************************************************************************************
* Global Variable
*****************************************************************************************/
#if (MODEISRTPM == FLAG_TPM)
	static bool pitIsrFlag[2] = {false};
#else // if (MODEISRTPM == COUNTER_TPM)
	#if (OTHER_TPM == COUNTER_TPM)
		static uint64_t tpm_i[2] = {0};
	#else
		static bool pitIsrFlag = false;
		static uint64_t tpm_i = 0;
	#endif
#endif

/*****************************************************************************************
*
*****************************************************************************************/
bool pit_Init(uint32_t value,bool ch)
{
	// Verifica parametros
	if((ch < 2) && (value < 0x100000000))
	{
		// Habilita clock para o PIT
		SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

		// Habilita PIT Module Control Register (PIT_MCR)
		PIT_MCR = 0;

		// Configura contador
		PIT_LDVAL_REG(PIT,ch) = value;

		// Enable interrupt and enable timer
		//PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
		PIT_TCTRL_REG(PIT,ch) |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;

		// Enable External Interrupt
		NVIC_EnableIRQ(PIT_IRQn);

		return true;
	}
	else
	{
		return false;
	}
}
/*****************************************************************************************
*
*****************************************************************************************/
void pit_Start(bool ch)
{
	PIT_TCTRL_REG(PIT,ch) |= PIT_TCTRL_TEN_MASK;
}
/*****************************************************************************************
*
*****************************************************************************************/
void pit_Stop(bool ch)
{
	PIT_TCTRL_REG(PIT,ch) &= ~PIT_TCTRL_TEN_MASK;
}
/*****************************************************************************************
*
*****************************************************************************************/
#if (MODEISRTPM == FLAG_TPM)
	bool pit_GetFlag_Isr(bool ch)
	{
		return pitIsrFlag[ch];
	}
	/********************************************************************************
	*
	*********************************************************************************/
	void pit_ClearFlag_Isr(bool ch)
	{
		 pitIsrFlag[ch] = false;
	}
#else //if (MODEISRTPM == COUNTER_TPM)
	#if (OTHER_TPM == COUNTER_TPM)
		void pit_ClearCounter_Isr(bool ch)
		{
			tpm_i[ch] = 0;
		}
		/******************************************************************************
		*
		*******************************************************************************/
		uint64_t pit_GetCounter_Isr(bool ch)
		{
			return tpm_i[ch];
		}
	#else
		// Flag - channel 0
		bool pit_GetFlag_Isr(void)
		{
			return pitIsrFlag;
		}
		/******************************************************************************
		*
		*******************************************************************************/
		void pit_ClearFlag_Isr(void)
		{
			pitIsrFlag = false;
		}
		/******************************************************************************
		*
		*******************************************************************************/
		// counter - channel 1
		void pit_ClearCounter_Isr(void)
		{
			tpm_i = 0;
		}
		/******************************************************************************
		*
		*******************************************************************************/
		uint64_t pit_GetCounter_Isr(void)
		{
			return tpm_i;
		}
	#endif
#endif
/*****************************************************************************************
* Handles PIT interrupt if enabled
*****************************************************************************************/
void PIT_IRQHandler(void)
{
	uint8_t index = 0;

	for(index=0;index<2;index++)
	{
		if( PIT_TFLG_REG(PIT,index) )
		{
			// Clear interrupt
			//PIT_TFLG0 = PIT_TFLG_TIF_MASK;
			PIT_TFLG_REG(PIT,index) = PIT_TFLG_TIF_MASK;
#if (MODEISRTPM == FLAG_TPM)
			pitIsrFlag[index] = true;
#else //if (MODEISRTPM == COUNTER_TPM)
	#if (OTHER_TPM == COUNTER_TPM)
			tpm_i[index] += 1;
	#else
			if(index)
				tpm_i++;
			else
				pitIsrFlag = true;
	#endif
#endif
		}
	}
}
/****************************************************************************************/
