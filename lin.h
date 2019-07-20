#ifndef LIN_H
#define LIN_H

#include "Lin_GeneralTypes.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f303xc.h"

/* User definitions */
typedef struct Lin_ChannelConfigType
{
    int channelID;
    Lin_ChannelState u8ChannelState;
    USART_TypeDef *pUSARTType;
    LL_USART_InitTypeDef *pUSARTInit;
} Lin_ChannelConfigType;

/* Structure */
typedef struct Lin_ConfigType
{
    int numberOfChannel;
    Lin_ChannelConfigType *pLinChannelConfig;
} Lin_ConfigType;

typedef enum Std_ReturnType
{
    E_OK = 0x00,
    E_NOT_OK
} Std_ReturnType;

/* Function prototypes */
void Lin_Init(const Lin_ConfigType *Config);                 /* Initializes the LIN module */
Std_ReturnType Lin_CheckWakeup(int Channel);                 /* Checks if a wakeup has occurred on the addressed LIN channel */
Std_ReturnType Lin_GoToSleep(int Channel);                   /* Instructs the driver to transmit a go-to-sleep-command on the addressed LIN channel */
Std_ReturnType Lin_GoToSleepInternal(int Channel);           /* Sets the channel state to LIN_CH_SLEEP, enables the wake-up detection and
                                                    optionally sets the LIN hardware unit to reduced power operation mode (if supported by HW */
Std_ReturnType Lin_Wakeup(int Channel);                      /* Generates a wake up pulse and sets the channel state to LIN_CH_OPERATIONAL */
Std_ReturnType Lin_WakeupInternal(int Channel);              /* Sets the channel state to LIN_CH_OPERATIONAL without generating a wake up pulse */
Lin_StatusType Lin_GetStatus(int Channel, int **Lin_SduPtr); /* Get the status of the LIN driver */

/* Marco defintions */
#define NULL_PTR 0x00
#define LIN_MAX_HW_CHANNEL (1U)

#endif /* LIN_H */