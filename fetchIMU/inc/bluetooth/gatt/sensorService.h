/*
 * sensorService.h
 *
 *  Created on: Jun 15, 2021
 *      Author: wondercise
 */

#ifndef BLUETOOTH_GATT_SENSORSERVICE_H_
#define BLUETOOTH_GATT_SENSORSERVICE_H_

bool create_gatt_sensorService();
bool add_gatt_characteristic_to_gatt_sensorService();
bool get_gatt_sensorService_handle(bt_gatt_h *handle);
bool set_bluetooth_le_write_value_requested_cb();
bool destroy_gatt_sensorService();
bool set_bluetooth_le_advertising_gatt_sensorService_uuid(bt_advertiser_h bluetooth_le_advertiser_handle);

#endif /* BLUETOOTH_GATT_SENSORSERVICE_H_ */
