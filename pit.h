/*
 * pit.h
 *
 *  Created on: 18/04/2017
 *      Author: Evandro
 */

#ifndef SOURCES_PIT_H_
#define SOURCES_PIT_H_

#include "externs.h"
//#include "stdbool.h"

#define PIT_CH_0    0
#define PIT_CH_1	1

#define FLAG_TPM 	1
//#define COUNTER_TPM	2
#define NOFLAG_TPM	0
#define MODEISRTPM (NOFLAG_TPM)

#define OTHER_TPM   (DUAL_TPM)
#define COUNTER_TPM 1
#define DUAL_TPM    0

//void init_pit(uint32_t value);
bool pit_Init(uint32_t value,bool ch);
void pit_Start(bool ch);
void pit_Stop(bool ch);

#if (MODEISRTPM == FLAG_TPM)
	bool pit_GetFlag_Isr(bool ch);
	void pit_ClearFlag_Isr(bool ch);
#else //if (MODEISRTPM == COUNTER_TPM)

	#if (OTHER_TPM == COUNTER_TPM)
		void pit_ClearCounter_Isr(bool ch);
		uint64_t pit_GetCounter_Isr(bool ch);
	#else
		// Flag - channel 0
		bool pit_GetFlag_Isr(void);
		void pit_ClearFlag_Isr(void);

		// counter - channel 1
		void pit_ClearCounter_Isr(void);
		uint64_t pit_GetCounter_Isr(void);
	#endif
/*#else //if (MODEISRTPM == DUAL_TPM)
// Dual mode
	bool pit_GetFlag_Isr(bool ch);
	void pit_ClearFlag_Isr(bool ch);

	void pit_ClearCounter_Isr(bool ch);
	uint64_t pit_GetCounter_Isr(bool ch);*/
#endif

#endif /* SOURCES_PIT_H_ */
