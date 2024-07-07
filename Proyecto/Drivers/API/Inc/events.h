/*
 * events.h
 *
 *  Created on: Jul 7, 2024
 *      Author: Mirko Serra
 */

#ifndef API_INC_EVENTS_H_
#define API_INC_EVENTS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t event_tick_t;

typedef event_tick_t (*get_tick_callback_t)();
typedef void (*event_callback_t)(void*);

/* Describes one event */
typedef struct {
    bool enabled;
    event_tick_t duration;
    event_tick_t last_active;
    event_callback_t callback;
    void* additional_data;
} event_t;

/* Describes a list of events */
typedef struct {
    get_tick_callback_t callback;
    size_t max_events;
    size_t num_events;
    event_t events[];
} event_list_t;

#define DECLARE_EVENT_LIST(elements, name) struct { \
    get_tick_callback_t callback; \
    size_t max_events; \
    size_t num_events; \
    event_t events[elements]; \
} _ ## name; \
event_list_t* name = (event_list_t*) &_ ## name;

/* Initializes the list of events */
void eventListInit(event_list_t* event_list, size_t max_events, get_tick_callback_t callback);
/* Adds one event. Returns -1 in case of failure (list full) or current event index */
int eventListAdd(event_list_t* event_list, bool enabled,
                event_tick_t duration, event_callback_t callback,
                void* additional_data);
/* Checks all events duration and calls their callbacks. Returns true if at least one callback called */
bool eventListCheck(event_list_t* event_list);

/* Enables or disables an event. Returns previous state */
bool eventSetEnable(event_list_t* event_list, size_t index, bool enable,
                bool reset);
/* Sets new duration for task. Returns previous value */
event_tick_t eventSetDuration(event_list_t* event_list, size_t index,
                event_tick_t duration, bool reset);
/* Returns enabled state of event */
bool eventIsEnabled(event_list_t* event_list, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* API_INC_EVENTS_H_ */
