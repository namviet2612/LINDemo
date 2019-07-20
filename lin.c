#include "lin.h"

/* Global Variables */
Lin_ModuleState gLinModuleState = LIN_UNINIT;
const Lin_ConfigType *pLinCurrentConfig;

/* Static function */
static Std_ReturnType Lin_Ipw_SendHeader(int Channel, const Lin_PduType *pcPduInfoPtr);

void Lin_Init(const Lin_ConfigType *Config)
{
    ErrorStatus linErrorStatus = ERROR;
    Lin_ChannelConfigType *pLinCurrentChannelConfig = Config->pLinChannelConfig;
    USART_TypeDef *pCurrentUSARTType = NULL_PTR;
    LL_USART_InitTypeDef *pCurrentUSARTInitType = NULL_PTR;
    int u8ChannelCounter = 0;

    if (Config != NULL_PTR)
    {
        if (gLinModuleState == LIN_UNINIT)
        {
            for (u8ChannelCounter = 0; u8ChannelCounter < Config->numberOfChannel; u8ChannelCounter++)
            {
                pCurrentUSARTType = (pLinCurrentChannelConfig + u8ChannelCounter)->pUSARTType;
                pCurrentUSARTInitType = (pLinCurrentChannelConfig + u8ChannelCounter)->pUSARTInit;

                /* First disable USART mode by clear UE bit */
                pCurrentUSARTType->CR1 &= (~USART_CR1_UE_Msk);
                /* Clear STOP and CLKEN bits in CR2 as guidance from reference manual */
                pCurrentUSARTType->CR2 &= (~(USART_CR2_STOP_Msk | USART_CR2_CLKEN_Msk));
                /* Clear SCEN, HDSEL and IREn in CR3 as guidance from reference manual */
                pCurrentUSARTType->CR3 &= (~(USART_CR3_SCEN_Msk | USART_CR3_HDSEL_Msk | USART_CR3_IREN_Msk));
                /* Then enable LIN mode */
                pCurrentUSARTType->CR2 |= USART_CR2_LINEN_Msk;

                if (linErrorStatus == SUCCESS)
                {
                    pLinCurrentChannelConfig->u8ChannelState = LIN_CH_OPERATIONAL;
                }
            }
            gLinModuleState = LIN_INIT;
        }
    }
}

Std_ReturnType Lin_CheckWakeup(int Channel)
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

Std_ReturnType Lin_SendFrame(int Channel, const Lin_PduType *PduInfoPtr)
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

Std_ReturnType Lin_GoToSleep(int Channel)
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

Std_ReturnType Lin_GoToSleepInternal(int Channel)
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

Std_ReturnType Lin_Wakeup(int Channel)
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

Std_ReturnType Lin_WakeupInternal(int Channel)
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

Lin_StatusType Lin_GetStatus(int Channel, int **Lin_SduPtr)
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

static Std_ReturnType Lin_Ipw_SendHeader(int Channel, const Lin_PduType *pcPduInfoPtr)
{

    return E_OK;
}