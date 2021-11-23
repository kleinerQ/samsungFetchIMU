#ifndef SENSOR_LISTENER_H_
#define SENSOR_LISTENER_H_

bool create_sensor_listener(int, sensor_h , sensor_event_cb , void *);
bool set_sensor_listener_attribute();
bool set_sensor_listener_event_callback(int, sensor_event_cb, void *);
bool start_sensor_listener(int);
bool stop_sensor_listener(int);
bool destroy_sensor_listener(int);
bool check_sensor_listener_is_created(int);

#endif /* SENSOR_LISTENER_H_ */
