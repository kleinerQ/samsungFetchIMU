#include "fetchimu.h"
#include "bluetooth/gatt/sensorService.h"
#include "bluetooth/gatt/sensorCharacteristic.h"
#include "bluetooth/gatt/sensorCharacteristicDescriptor.h"

const char *GATT_SERVICE_UUID_SENSOR = "0044";
bt_gatt_h gatt_senseorService_handle = 0;
static void CALEGattRemoteCharacteristicWriteCb(const char *remoteAddress, int request_id,
        bt_gatt_server_h server, bt_gatt_h charPath,
        bool response_needed, int offset, const char *charValue,
        int charValueLen, void *userData);
static void CALEGattRemoteCharacteristicReadCb(const char *remote_address, int request_id,
		bt_gatt_server_h server, bt_gatt_h gatt_handle,
		int offset, void *user_data);
static void __bt_gatt_server_notification_state_change_cb(bool notify,
        bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data);
bool create_gatt_sensorService()
{
	int retval;
	retval = bt_gatt_service_create(GATT_SERVICE_UUID_SENSOR, BT_GATT_SERVICE_TYPE_PRIMARY, &gatt_senseorService_handle);

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_gatt_service_create() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}

	if(!add_gatt_characteristic_to_gatt_sensorService())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to add a characteristic to a specified service.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
	{
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in adding a characteristic to a specified service.", __FILE__, __func__, __LINE__);
		return true;
	}
}

bool add_gatt_characteristic_to_gatt_sensorService()
{
	int retval;
	bt_gatt_h gatt_characteristic_handle = 0;

	if (!get_gatt_sensorCharacteristic_handle(&gatt_characteristic_handle))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the GATT characteristic's handle.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the GATT characteristic's handle.", __FILE__, __func__, __LINE__);

	retval = bt_gatt_service_add_characteristic(gatt_senseorService_handle, gatt_characteristic_handle);

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_gatt_service_add_characteristic() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


bool set_bluetooth_le_advertising_gatt_sensorService_uuid(bt_advertiser_h bluetooth_le_advertiser_handle)
{
	int retval;

	retval = bt_adapter_le_add_advertising_service_uuid(bluetooth_le_advertiser_handle, BT_ADAPTER_LE_PACKET_ADVERTISING, GATT_SERVICE_UUID_SENSOR);
	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Function bt_adapter_le_add_advertising_service_uuid() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}

bool set_bluetooth_le_write_value_requested_cb()
{

	bt_gatt_h gatt_descriptor_handle = 0;
	if (!get_gatt_sensorCharacteristicDescriptor_handle(&gatt_descriptor_handle))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the GATT descriptor's handle.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the GATT descriptor's handle.", __FILE__, __func__, __LINE__);


	bt_gatt_h gatt_handle = 0;
	if (!get_gatt_sensorCharacteristic_handle(&gatt_handle))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the GATT handle.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the GATT handle.", __FILE__, __func__, __LINE__);






	int retval, retval2, retval3;

	retval = bt_gatt_server_set_write_value_requested_cb(gatt_handle, CALEGattRemoteCharacteristicWriteCb, NULL);
	retval2 = bt_gatt_server_set_read_value_requested_cb(gatt_handle, CALEGattRemoteCharacteristicReadCb, NULL);
	retval3 = bt_gatt_server_set_characteristic_notification_state_change_cb(gatt_handle, __bt_gatt_server_notification_state_change_cb, NULL);
	if(retval != BT_ERROR_NONE | retval2 != BT_ERROR_NONE | retval3 != BT_ERROR_NONE )
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Function bt_gatt_server_set_write_value_requested_cb() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}



bool get_gatt_sensorService_handle(bt_gatt_h *gatt_handle)
{
	if(gatt_senseorService_handle != 0)
	{
		*gatt_handle = gatt_senseorService_handle;
		return true;
	}
	else
		return false;
}


bool destroy_gatt_sensorService()
{
	int retval;

	retval = bt_gatt_service_destroy(gatt_senseorService_handle);

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_gatt_service_destroy() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
	{
		gatt_senseorService_handle = 0;
		return true;
	}

}


void CALEGattRemoteCharacteristicReadCb(const char *remote_address, int request_id,
		bt_gatt_server_h server, bt_gatt_h gatt_handle,
		int offset, void *user_data)
{
	dlog_print(DLOG_ERROR, LOG_TAG,"__bt_gatt_server_read_value_requested_cb");
	dlog_print(DLOG_ERROR, LOG_TAG,"remote_address %s", remote_address);
	dlog_print(DLOG_ERROR, LOG_TAG,"req_id %d", request_id);
	dlog_print(DLOG_ERROR, LOG_TAG,"server %s", (char *)server);
	dlog_print(DLOG_ERROR, LOG_TAG,"gatt_handle %s", (char *)gatt_handle);
	dlog_print(DLOG_ERROR, LOG_TAG,"Offset %d", offset);

	//local buffer for current value
	char* value = " ";
	int len = 1;

	int ret = bt_gatt_get_value(gatt_handle, &value, &len);
	dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_get_value: %s \n", get_error_message(ret));

	/* Get the attribute new values here */
	ret = bt_gatt_server_send_response(request_id, BT_GATT_REQUEST_TYPE_READ, offset, BT_ERROR_NONE, value, len - offset);
	dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_server_send_response : %s \n", get_error_message(ret));

	//free value
	free(value);
}

void CALEGattRemoteCharacteristicWriteCb(const char *remoteAddress, int request_id,
                                         bt_gatt_server_h server, bt_gatt_h charPath,
                                         bool response_needed, int offset, const char *charValue,
                                         int charValueLen, void *userData)
{

	dlog_print(DLOG_INFO, LOG_TAG,"__bt_gatt_server_write_value_requested_cb");
	dlog_print(DLOG_ERROR, LOG_TAG,"__bt_gatt_server_write_value_requested_cb");
	dlog_print(DLOG_ERROR, LOG_TAG,"remote_address %s", remoteAddress);
	dlog_print(DLOG_ERROR, LOG_TAG,"req_id %d", request_id);
	dlog_print(DLOG_ERROR, LOG_TAG,"server %s", (char *)server);
	dlog_print(DLOG_ERROR, LOG_TAG,"gatt_handle %s", (char *)charPath);
	dlog_print(DLOG_ERROR, LOG_TAG,"Offset %d", offset);

	// Set the new attribute values -- only replaces the full value, i.e., offsets!=0 not supported as of now.
	if(offset!=0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "offset is non-zero! UNSUPPORTED ACTION -> send failure indication");
		//send failure response
		int ret = bt_gatt_server_send_response(request_id, BT_GATT_REQUEST_TYPE_WRITE, 0, BT_ERROR_NONE, "1", 1);
		dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_server_send_response : %s \n", get_error_message(ret));
	}

	int ret = bt_gatt_set_value(charPath, charValue, charValueLen);
	dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_set_characteristic_value : %s \n", get_error_message(ret));

	if (ret != BT_ERROR_NONE) {
		//send success response
		ret = bt_gatt_server_send_response(request_id, BT_GATT_REQUEST_TYPE_WRITE, 0, BT_ERROR_NONE, "0", 1);
		dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_server_send_response : %s \n", get_error_message(ret));
	} else {
		//send failure response
		ret = bt_gatt_server_send_response(request_id, BT_GATT_REQUEST_TYPE_WRITE, 0, BT_ERROR_NONE, "1", 1);
		dlog_print(DLOG_ERROR, LOG_TAG, "bt_gatt_server_send_response : %s \n", get_error_message(ret));
	}
}

void __bt_gatt_server_notification_state_change_cb(bool notify,
            bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data)
{
    // Extracting application data
    dlog_print(DLOG_ERROR, LOG_TAG, "__bt_gatt_server_notification_state_change_cb");
    dlog_print(DLOG_ERROR, LOG_TAG, "notify %d", notify);
    dlog_print(DLOG_ERROR, LOG_TAG, "server %s", (char *)server);
    dlog_print(DLOG_ERROR, LOG_TAG, "gatt_handle %s", (char *)gatt_handle);
}
