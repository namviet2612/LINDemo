#include "lin.h"

/* Global Variables */
Lin_ModuleState gLinModuleState = LIN_UNINIT;
const Lin_ConfigType *pLinCurrentConfig;

void Lin_Init(const Lin_ConfigType *Config)
{
    if (Config == NULL_PTR)
    {
        if (gLinModuleState == LIN_UNINIT)
        {
            pLinCurrentConfig = Config;
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

    if ((pLinCurrentChannelConfig + Channel)->currentChannelState == LIN_CH_SLEEP)
    {
        return (Std_ReturnType)E_NOT_OK;
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

    if (&pLinCurrentChannelConfig[Channel].currentChannelState != LIN_CH_SLEEP)
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

    if (&pLinCurrentChannelConfig[Channel].currentChannelState != LIN_CH_SLEEP)
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