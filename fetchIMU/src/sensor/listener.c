#include "fetchimu.h"
#include "sensor/listener.h"
#include "bluetooth/gatt/characteristic.h"

sensor_listener_h hrm_sensor_listener_handle = 0;
sensor_listener_h acce_sensor_listener_handle = 0;
unsigned int hrm_sensor_listener_event_update_interval_ms = 1000;
unsigned int acce_sensor_listener_event_update_interval_ms = 1000;

//static void hrm_sensor_listener_event_callback(sensor_h sensor, sensor_event_s events[], void *user_data);

bool create_sensor_listener(int mode, sensor_h sensor_handle, sensor_event_cb callback, void *user_data)
{
	int retval;

	switch (mode) {
	case 0:
		retval = sensor_create_listener(sensor_handle, &hrm_sensor_listener_handle);
		break;
	case 1:
		retval = sensor_create_listener(sensor_handle, &acce_sensor_listener_handle);
		break;
	default:
		return false;
	}


	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_create_listener() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}

	if(!set_sensor_listener_attribute(mode))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to set an attribute to control the behavior of a HRM sensor listener.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in setting an attribute to control the behavior of a HRM sensor listener.", __FILE__, __func__, __LINE__);


	if(!set_sensor_listener_event_callback(mode, callback, user_data))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to register the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in registering the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);


	if(!set_sensor_listener_event_callback(mode, callback, user_data))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to register the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in registering the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);

	return true;
}

bool set_sensor_listener_attribute(int mode)
{
	int retval;

	/*
	* By default, the listener automatically stops listening for the sensor data,
	* if the display is switched off or the device goes to the power-save mode. You can override such behavior:
	*/

	switch (mode) {
	case 0:
		retval = sensor_listener_set_option(hrm_sensor_listener_handle,  SENSOR_OPTION_ALWAYS_ON);
		retval = sensor_listener_set_attribute_int(hrm_sensor_listener_handle, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE);
		break;

	case 1:
		retval = sensor_listener_set_option(acce_sensor_listener_handle,  SENSOR_OPTION_ALWAYS_ON);
		retval = sensor_listener_set_attribute_int(acce_sensor_listener_handle, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE);
		break;
	default:
		return false;
	}
	//	retval = sensor_listener_set_attribute_int(acce_sensor_listener_handle, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE);
	/*
	* The above function makes the listener listen for the sensor data regardless of the display state and the power-save mode.
	* However, it does not prevent the device from going to the sleep mode.
	* To listen for the sensor data, the device must be awake anyway.
	*/

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_set_attribute_int() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}

bool set_sensor_listener_event_callback(int mode, sensor_event_cb callback, void *user_data)
{
	/* Register callback */
	int retval;
	switch (mode) {
	case 0:
		retval = sensor_listener_set_event_cb(hrm_sensor_listener_handle, hrm_sensor_listener_event_update_interval_ms, callback, user_data);
		break;
	case 1:
		retval = sensor_listener_set_event_cb(acce_sensor_listener_handle, acce_sensor_listener_event_update_interval_ms, callback, user_data);
		break;
	default:
		break;
	}

	if(retval != SENSOR_ERROR_NONE) {
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_set_event_cb() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


bool start_sensor_listener(int mode)
{
	int retval;

	switch (mode) {
	case 0:
		retval = sensor_listener_start(hrm_sensor_listener_handle);
		break;
	case 1:
		retval = sensor_listener_start(acce_sensor_listener_handle);
		break;
	}

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_start() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


bool stop_sensor_listener(int mode)
{
	int retval;
	switch (mode) {
	case 0:
		retval = sensor_listener_stop(hrm_sensor_listener_handle);
		break;
	case 1:
		retval = sensor_listener_stop(acce_sensor_listener_handle);
		break;
	}

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_stop() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


bool destroy_sensor_listener(int mode)
{
	int retval;
	switch (mode) {
	case 0:
		retval = sensor_destroy_listener(hrm_sensor_listener_handle);
		break;
	case 1:
		retval = sensor_destroy_listener(acce_sensor_listener_handle);
		break;
	}

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_destroy_listener() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
	{
		hrm_sensor_listener_handle = 0;
		acce_sensor_listener_handle = 0;
		return true;
	}
}


bool check_sensor_listener_is_created(int mode)
{

	switch (mode) {
	case 0:
		if (hrm_sensor_listener_handle != 0)
				return true;
			else
				return false;
		break;
	case 1:
		if (acce_sensor_listener_handle != 0)
				return true;
			else
				return false;
		break;

	default:
		return false;
	}
}
