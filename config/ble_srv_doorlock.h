#ifndef BLE_DOORLOCK_H__
#define BLE_DOORLOCK_H__

#define DOORLOCK_UUID_BASE          {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
#define DOORLOCK_UUID_SERVICE       0x1523
#define DOORLOCK_UUID_LOCK_CHAR     0x1525
#define DOORLOCK_UUID_STATE_CHAR    0x1524

typedef struct ble_doorlock_s ble_doorlock_t;

///**@brief Door Lock Service event handler type. */
typedef void (*ble_doorlock_lock_control_handler_t) (ble_doorlock_t * p_doorlock, uint8_t new_state);

typedef struct 
{ 
    ble_doorlock_lock_control_handler_t lock_control_handler;                    /**< Event handler to be called when LOCK characteristic is written. */ 
} ble_doorlock_init_t; 


/**@brief Door Lock Service init structure. This contains all options and data
 *        needed for initialization of the service. */
typedef struct ble_doorlock_s
{ 
    uint16_t                             service_handle; 
    ble_gatts_char_handles_t             lock_char_handles; 
    ble_gatts_char_handles_t             state_char_handles; 
    uint8_t                              uuid_type; 
    uint16_t                             conn_handle; 
    ble_doorlock_lock_control_handler_t  lock_control_handler; 
} ble_doorlock_t; 


void ble_doorlock_on_ble_evt(ble_evt_t * p_ble_evt);
void service_doorlock_init(void);

#endif // BLE_DOORLOCK_H__
