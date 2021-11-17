/*
 * sensorCharacteristic.h
 *
 *  Created on: Jun 15, 2021
 *      Author: wondercise
 */

#ifndef BLUETOOTH_GATT_SENSORCHARACTERISTIC_H_
#define BLUETOOTH_GATT_SENSORCHARACTERISTIC_H_

bool create_gatt_sensorCharacteristic();
bool add_gatt_descriptor_to_gatt_sensorCharacteristic();
bool set_gatt_sensorCharacteristic_value(int value);
bool notify_gatt_sensorCharacteristic_value_changed();
bool get_gatt_sensorCharacteristic_handle(bt_gatt_h *handle);

#endif /* BLUETOOTH_GATT_SENSORCHARACTERISTIC_H_ */
