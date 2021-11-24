#include "fetchimu.h"
#include <privacy_privilege_manager.h>
#include <device/power.h>
#include <tizen.h>
#include "bluetooth/gatt/server.h"
#include "bluetooth/gatt/service.h"
#include "bluetooth/gatt/sensorService.h"
#include "bluetooth/gatt/characteristic.h"
#include "bluetooth/gatt/descriptor.h"
#include "bluetooth/gatt/sensorCharacteristic.h"
#include "bluetooth/gatt/sensorCharacteristicDescriptor.h"
#include "bluetooth/le/advertiser.h"
#include "sensor/listener.h"
typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *navi;
	Evas_Object *box;
	Evas_Object *idLabel;
	Evas_Object *button;
	Evas_Object *heartRateLabel;
} appdata_s;


uint16_t RandomManufactureData;
sensor_h hrm_sensor_handle = 0;
sensor_h acce_sensor_handle = 0;
const char *sensor_privilege = "http://tizen.org/privilege/healthinfo";

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}


/* Callback for the "clicked" signal */
/* Called when the button is clicked by the user */
void button_clicked_callback(void *data, Evas_Object *obj, void *event_info)
{
	dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Shut down button is clicked.", __FILE__, __func__, __LINE__);
	ui_app_exit();
}

static void
create_base_gui(appdata_s *ad)
{


	/* Window */
		/* Create and initialize elm_win.
		   elm_win is mandatory to manipulate window. */
		ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
		elm_win_autodel_set(ad->win, EINA_TRUE);

		if (elm_win_wm_rotation_supported_get(ad->win)) {
			int rots[4] = { 0, 90, 180, 270 };
			elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
		}

		evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
		eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

		/* Conformant */
		/* Create and initialize elm_conformant.
		   elm_conformant is mandatory for base gui to have proper size
		   when indicator or virtual keypad is visible. */
		ad->conform = elm_conformant_add(ad->win);
		elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
		elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
		evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_win_resize_object_add(ad->win, ad->conform);
		evas_object_show(ad->conform);


		ad->navi = elm_naviframe_add(ad->conform);
		evas_object_show(ad->navi);
		elm_object_content_set(ad->conform, ad->navi);


		ad->box = elm_box_add(ad->navi);
		evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(ad->box);
		elm_object_content_set(ad->navi, ad->box);

		elm_naviframe_item_push(ad->navi, "Wondercise", NULL, NULL, ad->box, NULL);


		ad->idLabel = elm_label_add(ad->box);
		char tmp[16];
		snprintf(tmp, sizeof(tmp), "UnitId: %d", RandomManufactureData);
		elm_object_text_set(ad->idLabel, tmp);
		evas_object_show(ad->idLabel);
	    elm_box_pack_end(ad->box, ad->idLabel);


	    ad->heartRateLabel = elm_label_add(ad->box);
	    	elm_object_text_set(ad->heartRateLabel, "-- bpm");
	    	evas_object_show(ad->heartRateLabel);
	    elm_box_pack_end(ad->box, ad->heartRateLabel);


		ad->button = elm_button_add(ad->box);
		elm_object_text_set(ad->button, "Push To Stop");
		evas_object_smart_callback_add(ad->button, "clicked", button_clicked_callback, NULL);
		evas_object_size_hint_weight_set(ad->button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(ad->button, EVAS_HINT_FILL, 0.5);
		evas_object_show(ad->button);
		elm_box_pack_end(ad->box, ad->button);

		evas_object_show(ad->win);
//	/* Window */
//	/* Create and initialize elm_win.
//	   elm_win is mandatory to manipulate window. */
//	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
//	elm_win_autodel_set(ad->win, EINA_TRUE);
//
//	if (elm_win_wm_rotation_supported_get(ad->win)) {
//		int rots[4] = { 0, 90, 180, 270 };
//		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
//	}
//
//	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
//	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);
//
//	/* Conformant */
//	/* Create and initialize elm_conformant.
//	   elm_conformant is mandatory for base gui to have proper size
//	   when indicator or virtual keypad is visible. */
//	ad->conform = elm_conformant_add(ad->win);
//	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
//	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
//	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_win_resize_object_add(ad->win, ad->conform);
//	evas_object_show(ad->conform);
//
//	/* Label */
//	/* Create an actual view of the base gui.
//	   Modify this part to change the view. */
//	ad->label = elm_label_add(ad->conform);
//	elm_object_text_set(ad->label, "<align=center>Hello Tizen</align>");
//	evas_object_size_hint_weight_set(ad->label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	elm_object_content_set(ad->conform, ad->label);
//
//	/* Show window after base gui is set up */
//	evas_object_show(ad->win);
}



bool check_sensor_is_supported(sensor_type_e sensor_type)
{
	int retval;
	bool supported = false;

	retval = sensor_is_supported(sensor_type, &supported);

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_is_supported() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to checks whether a HRM sensor is supported in the current device.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in checking whether a HRM sensor is supported in the current device.", __FILE__, __func__, __LINE__);

	if(!supported)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_is_supported() output supported = %d", __FILE__, __func__, __LINE__, supported);
		return false;
	}
	else
		return true;
}

bool initialize_sensor(int mode, sensor_type_e sensor_type)
{
	int retval;
	switch (mode) {
	case 0:
		retval = sensor_get_default_sensor(sensor_type, &hrm_sensor_handle);
		break;
	case 1:
		retval = sensor_get_default_sensor(sensor_type, &acce_sensor_handle);
		break;
	default:
		return false;
	}

	if(retval != SENSOR_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function sensor_get_default_sensor() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	else
		return true;
}


void heartRate_sensor_listener_event_callback(sensor_h sensor, sensor_event_s events[], void *user_data) {

	appdata_s *ad = user_data;
	int value1 = (int)events[0].values[0];
	int value2 = (int)events[0].values[1];
	int value3 = (int)events[0].values[2];
	dlog_print(DLOG_INFO, LOG_TAG, "HRM value_count %d", events->value_count);
	if (value1 < 0)
	{
		return;
	}

	char valueStr[10];
	sprintf(valueStr, "%d, %d, %d", value1, value2, value3);
	elm_object_text_set(ad->heartRateLabel, valueStr);





	if(!set_gatt_characteristic_value(0, value1, 0, 0))
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to update the value of a characteristic's GATT handle.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in updating the value of a characteristic's GATT handle.", __FILE__, __func__, __LINE__);

	if(!notify_gatt_characteristic_value_changed())
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to notify value change of the characteristic to the remote devices which enable a Client Characteristic Configuration Descriptor.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in notifying value change of the characteristic to the remote devices which enable a Client Characteristic Configuration Descriptor.", __FILE__, __func__, __LINE__);


}



void acce_sensor_listener_event_callback(sensor_h sensor, sensor_event_s events[], void *user_data) {

	appdata_s *ad = user_data;
	int16_t value1 = (events[0].values[0] / (9.8 / 2048.0)) ;
	int16_t value2 = (events[0].values[1] / (9.8 / 2048.0)) ;
	int16_t value3 = (events[0].values[2] / (9.8 / 2048.0)) ;
	dlog_print(DLOG_INFO, LOG_TAG, "ACCE value:  %d, %d %d", value1, value2, value3);
//	if (value < 0)
//	{
//		value = 0;
//		return;
//	}


	char valueStr[100];
	sprintf(valueStr, "%d, %d, %d", value1, value2, value3);
	elm_object_text_set(ad->heartRateLabel, valueStr);


	if(!set_gatt_characteristic_value(1, value1, value2, value3))
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to update the value of a characteristic's GATT handle.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in updating the value of a characteristic's GATT handle.", __FILE__, __func__, __LINE__);

	if(!notify_gatt_characteristic_value_changed())
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to notify value change of the characteristic to the remote devices which enable a Client Characteristic Configuration Descriptor.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in notifying value change of the characteristic to the remote devices which enable a Client Characteristic Configuration Descriptor.", __FILE__, __func__, __LINE__);


}

void request_sensor_permission_response_callback(ppm_call_cause_e cause, ppm_request_result_e result, const char *privilege, void *user_data) {
	if (cause == PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR)
	{
		/* Log and handle errors */
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function request_sensor_permission_response_callback() output cause = PRIVACY_PRIVILEGE_MANAGER_CALL_CAUSE_ERROR", __FILE__, __func__, __LINE__);
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Function request_sensor_permission_response_callback() was called because of an error.", __FILE__, __func__, __LINE__);
	}
	else
	{
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Function request_sensor_permission_response_callback() was called with a valid answer.", __FILE__, __func__, __LINE__);

		switch (result) {
		case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_ALLOW_FOREVER:
			/* Update UI and start accessing protected functionality */
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: The user granted permission to use a sensor privilege for an indefinite period of time.", __FILE__, __func__, __LINE__);

			if(!initialize_sensor(0, SENSOR_HRM))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);

			if(!initialize_sensor(1, SENSOR_ACCELEROMETER))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);



			if(!create_sensor_listener(0, hrm_sensor_handle, heartRate_sensor_listener_event_callback, user_data))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create a HRM sensor listener.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating a HRM sensor listener.", __FILE__, __func__, __LINE__);


			if(!create_sensor_listener(1, acce_sensor_handle, acce_sensor_listener_event_callback, user_data))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create a HRM sensor listener.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating a HRM sensor listener.", __FILE__, __func__, __LINE__);


			if(!start_sensor_listener(0))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to start observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in starting observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);

			if(!start_sensor_listener(1))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to start observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
				ui_app_exit();
			}
			else
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in starting observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);

			break;
		case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER:
			/* Show a message and terminate the application */
			dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function request_sensor_permission_response_callback() output result = PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_FOREVER", __FILE__, __func__, __LINE__);
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: The user denied granting permission to use a sensor privilege for an indefinite period of time.", __FILE__, __func__, __LINE__);
			ui_app_exit();
			break;
		case PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE:
			/* Show a message with explanation */
			dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function request_sensor_permission_response_callback() output result = PRIVACY_PRIVILEGE_MANAGER_REQUEST_RESULT_DENY_ONCE", __FILE__, __func__, __LINE__);
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: The user denied granting permission to use a sensor privilege once.", __FILE__, __func__, __LINE__);
			ui_app_exit();
			break;
		}
	}
}


bool request_sensor_permission(void *user_data)
{
	int retval;

	retval = ppm_request_permission(sensor_privilege, request_sensor_permission_response_callback, user_data);

	/* Log and handle errors */
	if (retval == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE)
		return true;
	else if (retval == PRIVACY_PRIVILEGE_MANAGER_ERROR_ALREADY_IN_PROGRESS)
		return true;
	else
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function ppm_request_permission() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
}


bool check_and_request_sensor_permission(void *user_data) {
	int retval;
	ppm_check_result_e result;

	retval = ppm_check_permission(sensor_privilege, &result);

	if (retval == PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE)
	{
		switch (result)
		{
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ALLOW:
			/* Update UI and start accessing protected functionality */
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: The application has permission to use a sensor privilege.", __FILE__, __func__, __LINE__);


			if(!check_sensor_listener_is_created(0))
			{
				if(!initialize_sensor(0, SENSOR_HRM))
				{
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);
					return false;
				}
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);

				if(!create_sensor_listener(0, hrm_sensor_handle, heartRate_sensor_listener_event_callback, user_data))
				{
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create a HRM sensor listener.", __FILE__, __func__, __LINE__);
					return false;
				}
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating a HRM sensor listener.", __FILE__, __func__, __LINE__);

				if(!start_sensor_listener(0))
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to start observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in starting observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
			}

			if(!check_sensor_listener_is_created(1))
			{
				if(!initialize_sensor(1, SENSOR_ACCELEROMETER))
				{
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);
					return false;
				}
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the handle for the default sensor of a HRM sensor.", __FILE__, __func__, __LINE__);

				if(!create_sensor_listener(1, acce_sensor_handle, acce_sensor_listener_event_callback, user_data))
				{
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create a HRM sensor listener.", __FILE__, __func__, __LINE__);
					return false;
				}
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating a HRM sensor listener.", __FILE__, __func__, __LINE__);

				if(!start_sensor_listener(1))
					dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to start observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
				else
					dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in starting observing the sensor events regarding a HRM sensor listener.", __FILE__, __func__, __LINE__);
			}

			return true;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY:
			/* Show a message and terminate the application */
			dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function ppm_check_permission() output result = PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_DENY", __FILE__, __func__, __LINE__);
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: The application doesn't have permission to use a sensor privilege.", __FILE__, __func__, __LINE__);
			return false;
		case PRIVACY_PRIVILEGE_MANAGER_CHECK_RESULT_ASK:
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: The user has to be asked whether to grant permission to use a sensor privilege.", __FILE__, __func__, __LINE__);

			if(!request_sensor_permission(user_data))
			{
				dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to request a user's response to obtain permission for using the sensor privilege.", __FILE__, __func__, __LINE__);
				return false;
			}
			else
			{
				dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in requesting a user's response to obtain permission for using the sensor privilege.", __FILE__, __func__, __LINE__);
				return true;
			}
		}
	}
	else
	{
		/* retval != PRIVACY_PRIVILEGE_MANAGER_ERROR_NONE */
		/* Handle errors */
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function ppm_check_permission() return %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		return false;
	}
	return false;
}


static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	int retval;
	bt_adapter_state_e bluetooth_adapter_state;
	appdata_s *ad = data;

	RandomManufactureData = rand();


	create_base_gui(ad);

	if(!check_sensor_is_supported(0))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: A HRM sensor is not supported.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: A HRM sensor is supported.", __FILE__, __func__, __LINE__);

	if(!check_sensor_is_supported(1))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: A HRM sensor is not supported.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: A HRM sensor is supported.", __FILE__, __func__, __LINE__);

	retval = bt_initialize();

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_initialize() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to initialize the Bluetooth API.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in initializing the Bluetooth API.", __FILE__, __func__, __LINE__);

	retval = bt_adapter_get_state(&bluetooth_adapter_state);

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_adapter_get_state() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to get the current state of local Bluetooth adapter.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in getting the current state of local Bluetooth adapter.", __FILE__, __func__, __LINE__);

	if (bluetooth_adapter_state == BT_ADAPTER_DISABLED)
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Bluetooth adapter is disabled.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Bluetooth adapter is enabled.", __FILE__, __func__, __LINE__);

	retval = bt_gatt_server_initialize();

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_gatt_server_initialize() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to initialize the GATT server.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in initializing the GATT server.", __FILE__, __func__, __LINE__);

	if(!create_gatt_descriptor())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT characteristic descriptor.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT characteristic descriptor.", __FILE__, __func__, __LINE__);

	if(!create_gatt_characteristic())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT characteristic.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT characteristic.", __FILE__, __func__, __LINE__);


	if(!create_gatt_sensorCharacteristicDescriptor())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT sensorCharacteristic descriptor.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT sensorCharacteristic descriptor.", __FILE__, __func__, __LINE__);

	if(!create_gatt_sensorCharacteristic())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT sensorCharacteristic.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT sensorCharacteristic.", __FILE__, __func__, __LINE__);

	if(!create_gatt_service())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT service.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT service.", __FILE__, __func__, __LINE__);

	if(!create_gatt_sensorService())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT service.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT service.", __FILE__, __func__, __LINE__);



	if(!create_gatt_server())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create the GATT server's handle.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating the GATT server's handle.", __FILE__, __func__, __LINE__);

	if(!start_gatt_server())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to register the application along with the GATT services of the application it is hosting.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in registering the application along with the GATT services of the application it is hosting.", __FILE__, __func__, __LINE__);

	if(!create_bluetooth_le_advertiser())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to create advertiser to advertise device's existence.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in creating advertiser to advertise device's existence.", __FILE__, __func__, __LINE__);

	if(!start_bluetooth_le_advertising())
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to start advertising with passed advertiser and advertising parameters.", __FILE__, __func__, __LINE__);
		return false;
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in starting advertising with passed advertiser and advertising parameters.", __FILE__, __func__, __LINE__);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
	if (!check_and_request_sensor_permission(data))
	{
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to check if an application has permission to use the sensor privilege.", __FILE__, __func__, __LINE__);
		ui_app_exit();
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in checking if an application has permission to use the sensor privilege.", __FILE__, __func__, __LINE__);
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
	int retval;

	if(check_sensor_listener_is_created(0))
	{
		if(!destroy_sensor_listener(0))
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to release all the resources allocated for a HRM sensor listener.", __FILE__, __func__, __LINE__);
		else
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in releasing all the resources allocated for a HRM sensor listener.", __FILE__, __func__, __LINE__);
	}

	if(check_sensor_listener_is_created(1))
	{
		if(!destroy_sensor_listener(1))
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to release all the resources allocated for a HRM sensor listener.", __FILE__, __func__, __LINE__);
		else
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in releasing all the resources allocated for a HRM sensor listener.", __FILE__, __func__, __LINE__);
	}


	if(!destroy_gatt_service())
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to destroy the GATT handle of service.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in destroying the GATT handle of service.", __FILE__, __func__, __LINE__);

	if(!destroy_gatt_sensorService())
			dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to destroy the GATT handle of service.", __FILE__, __func__, __LINE__);
		else
			dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in destroying the GATT handle of service.", __FILE__, __func__, __LINE__);


	if(!destroy_gatt_server())
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to destroy the GATT server's handle.", __FILE__, __func__, __LINE__);
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in destroying the GATT server's handle.", __FILE__, __func__, __LINE__);

	retval = bt_deinitialize();

	if(retval != BT_ERROR_NONE)
	{
		dlog_print(DLOG_DEBUG, LOG_TAG, "%s/%s/%d: Function bt_deinitialize() return value = %s", __FILE__, __func__, __LINE__, get_error_message(retval));
		dlog_print(DLOG_ERROR, LOG_TAG, "%s/%s/%d: Failed to release all resources of the Bluetooth API.", __FILE__, __func__, __LINE__);
	}
	else
		dlog_print(DLOG_INFO, LOG_TAG, "%s/%s/%d: Succeeded in releasing all resources of the Bluetooth API.", __FILE__, __func__, __LINE__);

}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	device_power_request_lock(POWER_LOCK_DISPLAY, 0);
	device_power_request_lock(POWER_LOCK_CPU, 0);
	return ret;
}
