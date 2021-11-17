/*
 * sensorCharacteristicDescriptor.c
 *
 *  Created on: Jun 15, 2021
 *      Author: wondercise
 */


#include "fetchimu.h"
#include "bluetooth/gatt/sensorCharacteristicDescriptor.h"

bt_gatt_h gatt_sensorCharacteristicDescriptor_handle = 0;

bool create_gatt_sensorCharacteristicDescriptor()
{
	int retval;
	const char *GATT_DESCRIPTOR_UUID = "00002902-0000-1000-8000-00805F9B34FB";
	const char GATT_DESCRIPTOR_VALUE[] = {1, 0};

	retval = bt_gatt_descriptor_create(GATT_DESCRIPTOR_UUID,
			BT_GATT_PERMISSION_READ|BT_GATT_PERMISSION_WRITE,
			GATT_DESCRIPTOR_VALUE, sizeof(GATT_DESCRIPTOR_VALUE),
			&gatt_sensorCharacteristicDescriptor_handle);
	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_gatt_descriptor_create() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}

bool get_gatt_sensorCharacteristicDescriptor_handle(bt_gatt_h *gatt_handle)
{
	if(gatt_sensorCharacteristicDescriptor_handle != 0)
	{
		*gatt_handle = gatt_sensorCharacteristicDescriptor_handle;
		return true;
	}
	else
		return false;
}
