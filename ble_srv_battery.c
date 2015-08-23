#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_bas.h"
#include "ble_date_time.h"
#include "ble_srv_battery.h"
#include "bsp.h"
#include "nordic_common.h"

static ble_bas_t                        m_bas;                                     /**< Structure used to identify the battery service. */

void service_battery_init(void)
{
    uint32_t         err_code;
    ble_bas_init_t   bas_init;

    // Initialize Battery Service.
    memset(&bas_init, 0, sizeof(bas_init));

    // Here the sec level for the Battery Service can be changed/increased.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

    bas_init.evt_handler          = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref         = NULL;
    bas_init.initial_batt_level   = 100;

    err_code = ble_bas_init(&m_bas, &bas_init);
    APP_ERROR_CHECK(err_code);
}
