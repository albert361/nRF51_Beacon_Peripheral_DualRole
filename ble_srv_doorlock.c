#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_date_time.h"
#include "ble_srv_doorlock.h"
#include "bsp.h"
#include "nordic_common.h"

ble_doorlock_t          m_doorlock;
ble_doorlock_init_t     m_doorlock_init;
volatile int            xxx = 0;

/**@brief Function for handling the Connect event. 
 * 
 * @param[in]   p_doorlock  Door Lock Service structure. 
 * @param[in]   p_ble_evt   Event received from the BLE stack. 
 */ 
static void on_connect(ble_doorlock_t * p_doorlock, ble_evt_t * p_ble_evt) 
{ 
    p_doorlock->conn_handle = p_ble_evt->evt.gap_evt.conn_handle; 
} 




/**@brief Function for handling the Disconnect event. 
 * 
 * @param[in]   p_doorlock  Door Lock Service structure. 
 * @param[in]   p_ble_evt   Event received from the BLE stack. 
 */ 
static void on_disconnect(ble_doorlock_t * p_doorlock, ble_evt_t * p_ble_evt) 
{ 
    UNUSED_PARAMETER(p_ble_evt); 
    p_doorlock->conn_handle = BLE_CONN_HANDLE_INVALID; 
} 




/**@brief Function for handling the Write event. 
 * 
 * @param[in]   p_doorlock  Door Lock Service structure. 
 * @param[in]   p_ble_evt   Event received from the BLE stack. 
 */ 
static void on_write(ble_doorlock_t * p_doorlock, ble_evt_t * p_ble_evt) 
{ 
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write; 
     
    if ((p_evt_write->handle == p_doorlock->lock_char_handles.value_handle) && 
        (p_evt_write->len == 1) && 
        (p_doorlock->lock_control_handler != NULL)) 
    { 
        p_doorlock->lock_control_handler(p_doorlock, p_evt_write->data[0]); 
    } 
} 


void ble_doorlock_on_ble_evt(ble_evt_t * p_ble_evt) 
{ 
    ble_doorlock_t      *p_doorlock = &m_doorlock;
    switch (p_ble_evt->header.evt_id) 
    { 
        case BLE_GAP_EVT_CONNECTED: 
            on_connect(p_doorlock, p_ble_evt); 
            break; 
             
        case BLE_GAP_EVT_DISCONNECTED: 
            on_disconnect(p_doorlock, p_ble_evt); 
            break; 
             
        case BLE_GATTS_EVT_WRITE: 
            on_write(p_doorlock, p_ble_evt); 
            break; 
             
        default: 
            // No implementation needed. 
            break; 
    } 
} 

/**@brief Function for adding the LED characteristic. 
 * 
 */ 
static uint32_t lock_char_add(ble_doorlock_t * p_doorlock, const ble_doorlock_init_t * p_lbs_init) 
{ 
    ble_gatts_char_md_t char_md; 
    ble_gatts_attr_t    attr_char_value; 
    ble_uuid_t          ble_uuid; 
    ble_gatts_attr_md_t attr_md; 


    memset(&char_md, 0, sizeof(char_md)); 
     
    char_md.char_props.read   = 1; 
    char_md.char_props.write  = 1; 
    char_md.p_char_user_desc  = NULL; 
    char_md.p_char_pf         = NULL; 
    char_md.p_user_desc_md    = NULL; 
    char_md.p_cccd_md         = NULL; 
    char_md.p_sccd_md         = NULL; 
     
    ble_uuid.type = p_doorlock->uuid_type; 
    ble_uuid.uuid = DOORLOCK_UUID_LOCK_CHAR; 
      
     memset(&attr_md, 0, sizeof(attr_md)); 
 

     BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm); 
     BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm); 
     attr_md.vloc       = BLE_GATTS_VLOC_STACK; 
     attr_md.rd_auth    = 0; 
     attr_md.wr_auth    = 0; 
     attr_md.vlen       = 0; 
      
     memset(&attr_char_value, 0, sizeof(attr_char_value)); 
 

     attr_char_value.p_uuid       = &ble_uuid; 
     attr_char_value.p_attr_md    = &attr_md; 
     attr_char_value.init_len     = sizeof(uint8_t); 
     attr_char_value.init_offs    = 0; 
     attr_char_value.max_len      = sizeof(uint8_t); 
     attr_char_value.p_value      = NULL; 
      
     return sd_ble_gatts_characteristic_add(p_doorlock->service_handle, &char_md, 
                                                &attr_char_value, 
                                                &p_doorlock->lock_char_handles); 
 } 

static void lock_control_handler(ble_doorlock_t * p_doorlock, uint8_t led_state) 
{ 
    if (led_state) 
    { 
        xxx++;
    } 
    else 
    { 
        xxx++;
    } 
} 

void service_doorlock_init(void)
{
    uint32_t             err_code;
    ble_uuid_t           ble_uuid;
    ble_doorlock_t      *p_doorlock = &m_doorlock;
    ble_doorlock_init_t *p_doorlock_init = &m_doorlock_init;
    
    m_doorlock_init.lock_control_handler = lock_control_handler;

    // Initialize service structure
    p_doorlock->conn_handle          = BLE_CONN_HANDLE_INVALID;
    p_doorlock->lock_control_handler = p_doorlock_init->lock_control_handler;
    
    // Add service
    ble_uuid128_t base_uuid = {DOORLOCK_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_doorlock->uuid_type);
    APP_ERROR_CHECK(err_code);
    
    ble_uuid.type = p_doorlock->uuid_type;
    ble_uuid.uuid = DOORLOCK_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_doorlock->service_handle);
    APP_ERROR_CHECK(err_code);

    err_code = lock_char_add(p_doorlock, p_doorlock_init);
    APP_ERROR_CHECK(err_code);
}
