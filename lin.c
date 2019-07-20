#include "lin.h"

/* Global Variables */
Lin_ModuleState gLinModuleState = LIN_UNINIT;
const Lin_ConfigType *pLinCurrentConfig;

/* Static function */
static Std_ReturnType Lin_Ipw_SendHeader(uint8_t Channel, const Lin_PduType *pcPduInfoPtr);
static void Lin_USART_SetBaudrate(USART_TypeDef *USARTx, uint32_t u32Baudrate);
static uint32_t Lin_USART_Get_Peripheral_Clk(USART_TypeDef *USARTx);

void Lin_Init(const Lin_ConfigType *Config)
{
    ErrorStatus linErrorStatus[LIN_MAX_HW_CHANNEL] = {0};
    Lin_ChannelConfigType *pLinCurrentChannelConfig = Config->pLinChannelConfig;
    USART_TypeDef *pCurrentUSARTType = NULL_PTR;
    uint8_t u8ChannelCounter = 0;
    uint32_t u32Baudrate = 0;

    if (Config != NULL_PTR)
    {
        if (gLinModuleState == LIN_UNINIT)
        {
            for (u8ChannelCounter = 0; u8ChannelCounter < Config->numberOfChannel; u8ChannelCounter++)
            {
                pCurrentUSARTType = (pLinCurrentChannelConfig + u8ChannelCounter)->pUSARTType;

                /* First disable USART mode by clear UE bit */
                pCurrentUSARTType->CR1 &= (~USART_CR1_UE_Msk);

                /* Disable USART synchronous mode */
                pCurrentUSARTType->CR2 &= (~USART_CR2_CLKEN_Msk);

                /* Disable smartcard mode, half duplex mode and iR mode */
                pCurrentUSARTType->CR3 &= (~(USART_CR3_SCEN_Msk | USART_CR3_HDSEL_Msk | USART_CR3_IREN_Msk));

                /* Configuration of USART: Datawidth = 8, StopBits = 1, No Parity */
                /* PCE = 0, M bits = 00, stop bits = 00 */
                pCurrentUSARTType->CR1 &= (~(USART_CR1_M_Msk | USART_CR1_PCE_Msk | USART_CR2_STOP_Msk));

                /* COnfigure Baudrate */
                u32Baudrate = (pLinCurrentChannelConfig + u8ChannelCounter)->baudrate;

                if (u32Baudrate != 0)
                {
                    Lin_USART_SetBaudrate(pCurrentUSARTType,u32Baudrate);
                }
                else
                {
                    linErrorStatus[u8ChannelCounter] = ERROR;
                    continue;
                }

                /* After baudrate configuration, BRR should be greater or equal to 16 */
                if (pCurrentUSARTType->BRR < (uint32_t)16U)
                {
                    linErrorStatus[u8ChannelCounter] = ERROR;
                    continue;
                }

                /* Enable LIN mode */
                pCurrentUSARTType->CR2 |= USART_CR2_LINEN_Msk;

                /* Enable Transmister and Receiver */
                pCurrentUSARTType->CR1 |= (USART_CR1_TE_Msk | USART_CR1_RE_Msk);

                /* Wait until the premable frame was sent successfully */
                while ((pCurrentUSARTType->ISR &= USART_ISR_TXE_Msk)!=(uint32_t)0U);

                linErrorStatus[u8ChannelCounter] = SUCCESS;

            }

            for (u8ChannelCounter = 0; u8ChannelCounter < Config->numberOfChannel; u8ChannelCounter++)
            {
                if (linErrorStatus[u8ChannelCounter] == SUCCESS)
                {
                    pLinCurrentChannelConfig->u8ChannelState = LIN_CH_OPERATIONAL;
                }
            }
            gLinModuleState = LIN_INIT;
        }
    }
}

Std_ReturnType Lin_CheckWakeup(uint8 Channel)
{
    if (Channel < LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    /* TODO: SWS_Lin_00098 */

    return (Std_ReturnType)E_OK;
}

Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType *PduInfoPtr)
{
    Lin_ChannelConfigType *pLinCurrentChannelConfig = pLinCurrentConfig->pLinChannelConfig;
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (PduInfoPtr == NULL_PTR)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if ((pLinCurrentChannelConfig + Channel)->u8ChannelState == LIN_CH_SLEEP_STATE)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (E_OK == Lin_Ipw_SendHeader(Channel, PduInfoPtr))
    {

    }

    return (Std_ReturnType)E_OK;
}

Std_ReturnType Lin_GoToSleep(uint8 Channel)
{
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    return (Std_ReturnType)E_OK;
}

Std_ReturnType Lin_GoToSleepInternal(uint8 Channel)
{
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    return (Std_ReturnType)E_OK;
}

Std_ReturnType Lin_Wakeup(uint8 Channel)
{
    Lin_ChannelConfigType *pLinCurrentChannelConfig = pLinCurrentConfig->pLinChannelConfig;
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if ((pLinCurrentChannelConfig + Channel)->u8ChannelState == LIN_CH_SLEEP_STATE)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    return (Std_ReturnType)E_OK;
}

Std_ReturnType Lin_WakeupInternal(uint8 Channel)
{
    Lin_ChannelConfigType *pLinCurrentChannelConfig = pLinCurrentConfig->pLinChannelConfig;
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    if ((pLinCurrentChannelConfig + Channel)->u8ChannelState == LIN_CH_SLEEP_STATE)
    {
        return (Std_ReturnType)E_NOT_OK;
    }

    return (Std_ReturnType)E_OK;
}

Lin_StatusType Lin_GetStatus(uint8 Channel, uint8 **Lin_SduPtr)
{
    Lin_StatusType linCurrentStatus;
    if (Channel >= LIN_MAX_HW_CHANNEL)
    {
        return (Lin_StatusType)LIN_NOT_OK;
    }

    if (gLinModuleState == LIN_UNINIT)
    {
        return (Lin_StatusType)LIN_NOT_OK;
    }

    if (Lin_SduPtr == NULL_PTR)
    {
        return (Lin_StatusType)LIN_NOT_OK;
    }

    return linCurrentStatus;
}

static Std_ReturnType Lin_Ipw_SendHeader(uint8_t Channel, const Lin_PduType *pcPduInfoPtr)
{

    return E_OK;
}

static void Lin_USART_SetBaudrate(USART_TypeDef *USARTx, uint32_t u32Baudrate)
{
    uint32_t u32UsartSrcClk;

    /* Set Oversampling by 16 */
    USARTx->CR1 &= (~USART_CR1_OVER8_Msk);

    u32UsartSrcClk = Lin_USART_Get_Peripheral_Clk(USARTx);

    LL_USART_SetBaudRate(USARTx, u32UsartSrcClk, LL_USART_OVERSAMPLING_16, u32Baudrate);
}

static uint32_t Lin_USART_Get_Peripheral_Clk(USART_TypeDef *USARTx)
{
    uint32_t periphclk = LL_RCC_PERIPH_FREQUENCY_NO;
    /*---------------------------- USART BRR Configuration ---------------------
     * Retrieve Clock frequency used for USART Peripheral
     */
    if (USARTx == USART1)
    {
      periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART1_CLKSOURCE);
    }
    else if (USARTx == USART2)
    {
#if defined (RCC_CFGR3_USART2SW)
      periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART2_CLKSOURCE);
#else
      /* USART2 clock is PCLK */
      LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;
#endif
    }
    else if (USARTx == USART3)
    {
#if defined (RCC_CFGR3_USART3SW)
      periphclk = LL_RCC_GetUSARTClockFreq(LL_RCC_USART3_CLKSOURCE);
#else
      /* USART3 clock is PCLK */
      LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;
#endif
    }
#if defined(UART4)
    else if (USARTx == UART4)
    {
      periphclk = LL_RCC_GetUARTClockFreq(LL_RCC_UART4_CLKSOURCE);
    }
#endif /* UART4 */
#if defined(UART5)
    else if (USARTx == UART5)
    {
      periphclk = LL_RCC_GetUARTClockFreq(LL_RCC_UART5_CLKSOURCE);
    }
#endif /* UART5 */
    else
    {
      /* Nothing to do, as error code is already assigned to ERROR value */
    }

    return periphclk;
}