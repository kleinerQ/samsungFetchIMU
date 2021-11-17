#include "fetchimu.h"
#include "sensor/listener.h"
#include "bluetooth/gatt/characteristic.h"

sensor_listener_h hrm_sensor_listener_handle = 0;
unsigned int hrm_sensor_listener_event_update_interval_ms = 1000;

//static void hrm_sensor_listener_event_callback(sensor_h sensor, sensor_event_s events[], void *user_data);

bool create_hrm_sensor_listener(sensor_h sensor_handle, sensor_event_cb callback, void *user_data)
{
	int retval;

	retval = sensor_create_listener(sensor_handle, &hrm_sensor_listener_handle);

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_create_listener() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}

	if(!set_hrm_sensor_listener_attribute())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to set an attribute to control the behavior of a HRM sensor listener.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in setting an attribute to control the behavior of a HRM sensor listener.", __FILE__, __func__, __LINE__);

	if(!set_hrm_sensor_listener_event_callback(callback, user_data))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to register the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in registering the callback function to be invoked when sensor events are delivered via a HRM sensor listener.", __FILE__, __func__, __LINE__);

	return true;
}

bool set_hrm_sensor_listener_attribute()
{
	int retval;

	/*
	* By default, the listener automatically stops listening for the sensor data,
	* if the display is switched off or the device goes to the power-save mode. You can override such behavior:
	*/

	retval = sensor_listener_set_attribute_int(hrm_sensor_listener_handle, SENSOR_ATTRIBUTE_PAUSE_POLICY, SENSOR_PAUSE_NONE);

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

bool set_hrm_sensor_listener_event_callback(sensor_event_cb callback, void *user_data)
{
	/* Register callback */
	int retval;

	retval = sensor_listener_set_event_cb(hrm_sensor_listener_handle, hrm_sensor_listener_event_update_interval_ms, callback, user_data);

	if(retval != SENSOR_ERROR_NONE) {
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_set_event_cb() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


bool start_hrm_sensor_listener()
{
	int retval;

	retval = sensor_listener_start(hrm_sensor_listener_handle);

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_start() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}

bool stop_hrm_sensor_listener()
{
	int retval;

	retval = sensor_listener_stop(hrm_sensor_listener_handle);

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_listener_stop() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}

bool destroy_hrm_sensor_listener()
{
	int retval;

	retval = sensor_destroy_listener(hrm_sensor_listener_handle);

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_destroy_listener() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
	{
		hrm_sensor_listener_handle = 0;
		return true;
	}
}

bool check_hrm_sensor_listener_is_created()
{
	if (hrm_sensor_listener_handle != 0)
		return true;
	else
		return false;
}
