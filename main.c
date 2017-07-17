// Projeto: Dual Blink LED
// Autor: Evandro Teixeira
// Endereço da Lib: /home/evandro/Documentos/lib-frdm-kl25z/Library-FRDM-KL25Z-master
#include "/home/evandro/Documentos/lib-frdm-kl25z/Library-FRDM-KL25Z-master/externs.h"
#include "MKL25Z4.h"

static int i = 0;

int main(void)
{
    // Inicializa PIT canal 0
	pit_Init(10000000,PIT_CH_0);
	// Inicia PIT canal 0
	pit_Start(PIT_CH_0);
	// Incializa PIT canal 1
	pit_Init(100000,PIT_CH_1);
	// Inicia PIT canal 1
	pit_Start(PIT_CH_1);
	// Inicializa PTB18 - LED Vermelho
	gpio_Init(GPIOB,18,OUTPUT,NO_PULL_RESISTOR);
	// Inicializa PTB18 - LED Verde
	gpio_Init(GPIOB,19,OUTPUT,NO_PULL_RESISTOR);
	// Apaga LED Vermelho
	gpio_Set(GPIOB,18,1);
	// Aciona LED Verde
	gpio_Set(GPIOB,19,0);

    for (;;)
    {
        // Checa de Flag
    	if(pit_GetFlag_Isr()==true)
    	{
    		// Limpa Flag
    		pit_ClearFlag_Isr();
    		// Inverte o valor do LED Vermelho
    		gpio_Toggle(GPIOB,18);
    	}
    	// Verifica o valor do contador
    	if(pit_GetCounter_Isr() >= 100)
    	{
    		// Reset contador
    		pit_ClearCounter_Isr();
    		// Inverte o valor do LED Verde
    		gpio_Toggle(GPIOB,19);
    	}
    }
    /* Never leave main */
    return 0;
}