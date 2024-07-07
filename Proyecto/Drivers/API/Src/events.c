/*
 * events.c
 *
 *  Created on: Jul 7, 2024
 *      Author: Mirko Serra
 */

#include <assert.h>

#include "events.h"

void eventListInit(event_list_t* event_list, size_t max_events, get_tick_callback_t callback)
{
    assert(event_list);
    assert(max_events > 0);
    assert(callback);
    event_list->num_events = 0;
    event_list->max_events = max_events;
    event_list->callback = callback;
}

int eventListAdd(event_list_t* event_list, bool enabled, event_tick_t duration,
                event_callback_t callback, void* additional_data)
{
    assert(event_list);
    assert(callback);
    assert(event_list->callback);
    if (event_list->num_events >= event_list->max_events) {
        return -1;
    }
    event_tick_t now = event_list->callback();
    event_t evn = { enabled, duration, now, callback, additional_data };
    event_list->events[event_list->num_events] = evn;
    event_list->num_events++;
    return event_list->num_events - 1;
}

bool eventListCheck(event_list_t* event_list)
{
    assert(event_list);
    assert(event_list->callback);
    event_tick_t now = event_list->callback();
    bool called = false;
    for (size_t evn_num = 0; evn_num < event_list->num_events; evn_num++) {
        event_t* this_event = &event_list->events[evn_num];
        if (this_event->enabled && this_event->duration + this_event->last_active <= now) {
            called = true;
            this_event->last_active = now;
            this_event->callback(this_event->additional_data);
        }
    }
    return called;
}

bool eventSetEnable(event_list_t* event_list, size_t index, bool enable, bool reset)
{
    assert(event_list);
    assert(event_list->num_events > index);

    event_t* this_event = &event_list->events[index];

    bool old_state = this_event->enabled;

    this_event->enabled = enable;
    if (reset) {
        assert(event_list->callback);
        this_event->last_active = event_list->callback();
    }

    return old_state;
}

bool eventIsEnabled(event_list_t* event_list, size_t index)
{
    assert(event_list);
    assert(event_list->num_events > index);

    return event_list->events[index].enabled;
}

event_tick_t eventSetDuration(event_list_t* event_list, size_t index, event_tick_t duration, bool reset)
{
    assert(event_list);
    assert(event_list->num_events > index);

    event_t* this_event = &event_list->events[index];

    event_tick_t old_value = this_event->duration;

    this_event->duration = duration;
    if (reset) {
        assert(event_list->callback);
        this_event->last_active = event_list->callback();
    }

    return old_value;
}
