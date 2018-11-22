#include "USBCDC.h"

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#include <ErrorHandler.h>

#include "usbd/usbd_core.h"
#include "usbd/usbd_desc.h"

USB_CDC* g_usb_cdc_ptr = nullptr;

// Define size for the receive and transmit buffer over CDC
#define APP_RX_DATA_SIZE 1000
#define APP_TX_DATA_SIZE 1000

USBD_HandleTypeDef hUsbDeviceFS;

// Received data over USB are stored in this buffer
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

// Data to send over USB CDC are stored in this buffer
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t* Len);

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS = {
    CDC_Init_FS,
    CDC_DeInit_FS,
    CDC_Control_FS,
    CDC_Receive_FS
};

/**
  * @brief Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
    // Set Application Buffers
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);

    return USBD_OK;
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
    return USBD_OK;
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
    switch (cmd) {
    case CDC_SEND_ENCAPSULATED_COMMAND:

        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

        break;

    case CDC_SET_COMM_FEATURE:

        break;

    case CDC_GET_COMM_FEATURE:

        break;

    case CDC_CLEAR_COMM_FEATURE:

        break;

        /*******************************************************************************/
        /* Line Coding Structure                                                       */
        /*-----------------------------------------------------------------------------*/
        /* Offset | Field       | Size | Value  | Description                          */
        /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
        /* 4      | bCharFormat |   1  | Number | Stop bits                            */
        /*                                        0 - 1 Stop bit                       */
        /*                                        1 - 1.5 Stop bits                    */
        /*                                        2 - 2 Stop bits                      */
        /* 5      | bParityType |  1   | Number | Parity                               */
        /*                                        0 - None                             */
        /*                                        1 - Odd                              */
        /*                                        2 - Even                             */
        /*                                        3 - Mark                             */
        /*                                        4 - Space                            */
        /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
        /*******************************************************************************/
    case CDC_SET_LINE_CODING:

        break;

    case CDC_GET_LINE_CODING:

        break;

    case CDC_SET_CONTROL_LINE_STATE:

        break;

    case CDC_SEND_BREAK:

        break;

    default:
        break;
    }

    return USBD_OK;
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t* Len)
{
    uint16_t len16 = *Len;

    if (g_usb_cdc_ptr)
        g_usb_cdc_ptr->receive(Buf, len16);

    return USBD_OK;
}

USB_CDC::USB_CDC()
{
    g_usb_cdc_ptr = this;

    // We can't do initialisation here, because the clock is not yet initialised.
}

void USB_CDC::initialise()
{
    // Init Device Library
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

    // Add supported class
    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
    // Add interface
    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    // Start the library
    USBD_Start(&hUsbDeviceFS);
}

bool USB_CDC::transmit(uint8_t* buf, uint16_t len)
{
    uint8_t result = USBD_OK;

    USBD_CDC_HandleTypeDef* hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
    if (hcdc->TxState != 0) {
        return USBD_BUSY;
    }

    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, buf, len);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);

    return result == USBD_OK;
}

void USB_CDC::receive(uint8_t* buf, uint16_t len)
{
    if (receiver)
        receiver->receive(buf, len);

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
}