#ifndef LIN_GENERALTYPES_H
#define LIN_GENERALTYPES_H

#include <stdint.h>
/* Structure and Typedef */
typedef uint8_t uint8;

typedef uint8 Lin_FramePidType;	        /* The LIN identifier (0...0x3F) together with its two parity bits */

typedef enum Lin_FrameCsModelType
{
    LIN_ENHANCED_CS = 0x00,
    LIN_CLASSIC_CS
} Lin_FrameCsModelType;                  /* Specify the Checksum model to be used for the LIN Frame */

typedef enum Lin_FrameResponseType
{
    LIN_MASTER_RESPONSE = 0x00,
    LIN_SLAVE_RESPONSE,
    LIN_SLAVE_TO_SLAVE
} Lin_FrameResponseType;                 /* Specify whether the frame processor is required to transmit the response part of the LIN frame */

typedef uint8 Lin_FrameDlType;            /* Data length of a LIN Frame */

typedef struct Lin_PduType
{
    Lin_FramePidType Pid;               /* Pid */
    Lin_FrameCsModelType Cs;            /* Checksum */
    Lin_FrameResponseType Drc;          /* Frame response type */
    Lin_FrameDlType Dl;                 /* Data length */
    uint8* SduPtr;                        /*  SDU pointer from the LIN Interface to the LIN driver*/
} Lin_PduType;

typedef enum Lin_StatusType
{
    LIN_NOT_OK = 0x00,
    LIN_TX_OK,
    LIN_TX_BUSY,
    LIN_TX_HEADER_ERROR,
    LIN_TX_ERROR,
    LIN_RX_OK,
    LIN_RX_BUSY,
    LIN_RX_ERROR,
    LIN_RX_NO_RESPONSE,
    LIN_OPERATIONAL,
    LIN_CH_SLEEP    
} Lin_StatusType;

/* User type definition */
typedef enum Lin_ModuleState
{
    LIN_UNINIT = 0x00,
    LIN_INIT
} Lin_ModuleState;

typedef enum Lin_ChannelState
{
    LIN_CH_OPERATIONAL = 0x00,
    LIN_CH_SLEEP_PENDING,
    LIN_CH_SLEEP_STATE
} Lin_ChannelState;

#endif /* LIN_GENERALTYPES_H */