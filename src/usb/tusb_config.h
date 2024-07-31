#ifndef __TUSB_CONFIG_H
#define __TUSB_CONFIG_H

#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE

#define CFG_TUD_CDC             2
#define CFG_TUD_CDC_RX_BUFSIZE  (256)
#define CFG_TUD_CDC_TX_BUFSIZE  (256)

#include <tusb_option.h>

#ifdef __cplusplus
extern "C" {
#endif

void usbd_serial_init(void);

#ifdef __cplusplus
}
#endif

#endif // __TUSB_CONFIG_H
