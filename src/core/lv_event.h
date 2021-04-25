/**
 * @file lv_templ.h
 *
 */

#ifndef LV_EVENT_H
#define LV_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

struct _lv_obj_t;
struct _lv_event_dsc_t;

/**
 * Type of event being sent to the object.
 */
typedef enum {
    LV_EVENT_ALL = 0,

    /** Input device events*/
    LV_EVENT_PRESSED,             /**< The object has been pressed*/
    LV_EVENT_PRESSING,            /**< The object is being pressed (called continuously while pressing)*/
    LV_EVENT_PRESS_LOST,          /**< The object is still being pressed but slid cursor/finger off of the object */
    LV_EVENT_SHORT_CLICKED,       /**< The object was pressed for a short period of time, then released it. Not called if scrolled.*/
    LV_EVENT_LONG_PRESSED,        /**< Object has been pressed for at least `long_press_time`.  Not called if scrolled.*/
    LV_EVENT_LONG_PRESSED_REPEAT, /**< Called after `long_press_time` in every `long_press_repeat_time` ms.  Not called if scrolled.*/
    LV_EVENT_CLICKED,             /**< Called on release if not scrolled (regardless to long press)*/
    LV_EVENT_RELEASED,            /**< Called in every cases when the object has been released*/
    LV_EVENT_SCROLL_BEGIN,        /**< Scrolling begins*/
    LV_EVENT_SCROLL_END,          /**< Scrolling ends*/
    LV_EVENT_SCROLL,              /**< Scrolling*/
    LV_EVENT_GESTURE,             /**< A gesture is detected. Get the gesture with `lv_indev_get_gesture_dir(lv_indev_get_act());` */
    LV_EVENT_KEY,                 /**< A key is sent to the object. Get the key with `lv_indev_get_key(lv_indev_get_act());`*/
    LV_EVENT_FOCUSED,             /**< The object is focused*/
    LV_EVENT_DEFOCUSED,           /**< The object is defocused*/
    LV_EVENT_LEAVE,               /**< The object is defocused but still selected*/
    LV_EVENT_HIT_TEST,            /**< Perform advanced hit-testing*/

    /** Drawing events*/
    LV_EVENT_COVER_CHECK,        /**< Check if the object fully covers an area. The event parameter is `lv_cover_check_info_t *`.*/
    LV_EVENT_REFR_EXT_DRAW_SIZE, /**< Get the required extra draw area around the object (e.g. for shadow). The event parameter is `lv_coord_t *` to store the size.*/
    LV_EVENT_DRAW_MAIN_BEGIN,    /**< Starting the main drawing phase*/
    LV_EVENT_DRAW_MAIN,          /**< Perform the main drawing*/
    LV_EVENT_DRAW_MAIN_END,      /**< Finishing the main drawing phase*/
    LV_EVENT_DRAW_POST_BEGIN,    /**< Starting the post draw phase (when all children are drawn)*/
    LV_EVENT_DRAW_POST,          /**< Perform the post draw phase (when all children are drawn)*/
    LV_EVENT_DRAW_POST_END,      /**< Finishing the post draw phase (when all children are drawn)*/
    LV_EVENT_DRAW_PART_BEGIN,    /**< Starting to draw a part. The event parameter is `lv_obj_draw_dsc_t *`. */
    LV_EVENT_DRAW_PART_END,      /**< Finishing to draw a part. The event parameter is `lv_obj_draw_dsc_t *`. */

    /** Special events*/
    LV_EVENT_VALUE_CHANGED,       /**< The object's value has changed (i.e. slider moved)*/
    LV_EVENT_INSERT,              /**< A text is inserted to the object. The event data is `char *` being inserted.*/
    LV_EVENT_REFRESH,             /**< Notify the object to refresh something on it (for the user)*/
    LV_EVENT_READY,               /**< A process has finished*/
    LV_EVENT_CANCEL,              /**< A process has been cancelled */

    /** Other events*/
    LV_EVENT_DELETE,              /**< Object is being deleted*/
    LV_EVENT_CHILD_CHANGED,       /**< Child was removed/added*/
    LV_EVENT_SIZE_CHANGED,       /**< Object coordinates/size have changed*/
    LV_EVENT_STYLE_CHANGED,       /**< Object's style has changed*/
    LV_EVENT_BASE_DIR_CHANGED,    /**< The base dir has changed*/
    LV_EVENT_GET_SELF_SIZE,       /**< Get the internal size of a widget*/

    _LV_EVENT_LAST                /** Number of default events*/
}lv_event_code_t;


typedef struct _lv_event_t {
    struct _lv_obj_t * target;
    struct _lv_obj_t * original_target;
    lv_event_code_t code;
    void * user_data;
    void * param;
}lv_event_t;

/**
 * @brief Event callback.
 * Events are used to notify the user of some action being taken on the object.
 * For details, see ::lv_event_t.
 */
typedef void (*lv_event_cb_t)(lv_event_t * e);

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Send an event to the object
 * @param obj pointer to an object
 * @param event the type of the event from `lv_event_t`
 * @param param arbitrary data depending on the object type and the event. (Usually `NULL`)
 * @return LV_RES_OK: `obj` was not deleted in the event; LV_RES_INV: `obj` was deleted in the event
 */
lv_res_t lv_event_send(struct _lv_obj_t * obj, lv_event_code_t event, void * param);

lv_res_t lv_obj_event_base(const lv_obj_class_t * class_p, lv_event_t * e);

struct _lv_obj_t * lv_event_get_target(lv_event_t * e);

lv_event_code_t lv_event_get_code(lv_event_t * e);

void * lv_event_get_param(lv_event_t * e);

void * lv_event_get_user_data(lv_event_t * e);

/**
 * Get the original target of the event. It's different than the "normal" target if the event is bubbled.
 * @return      pointer to the object the originally received the event before bubbling it to the parents
 */
struct _lv_obj_t * lv_event_get_original_target(void);

/**
 * Register a new, custom event ID.
 * It can be used the same way as e.g. `LV_EVENT_CLICKED` to send custom events
 * @return      the new event id
 * @example
 * uint32_t LV_EVENT_MINE = 0;
 * ...
 * e = lv_event_register_id();
 * ...
 * lv_event_send(obj, LV_EVENT_MINE, &some_data);
 */
uint32_t lv_event_register_id(void);

/**
 * Nested events can be called and one of them might belong to an object that is being deleted.
 * Mark this object's `event_temp_data` deleted to know that it's `lv_event_send` should return `LV_RES_INV`
 * @param obj pointer to an obejct to mark as deleted
 */
void _lv_event_mark_deleted(struct _lv_obj_t * obj);


/**
 * Add an event handler function for an object.
 * Used by the user to react on event which happens with the object.
 * An object can have multiple event handler. They will be called in the same order as they were added.
 * @param obj       pointer to an object
 * @param filter    and event code (e.g. `LV_EVENT_CLICKED`) on which the event should be called. `LV_EVENT_ALL` can be sued the receive all the events.
 * @param event_cb  the new event function
 * @param           user_data custom data data will be available in `event_cb`
 * @return          a pointer the event descriptor. Can be used in ::lv_obj_remove_event_dsc
 */
struct _lv_event_dsc_t * lv_obj_add_event_cb(struct _lv_obj_t * obj, lv_event_cb_t event_cb, lv_event_code_t filter, void * user_data);

/**
 * Remove an event handler function for an object.
 * @param obj       pointer to an object
 * @param event_cb  the event function to remove
 * @return          true if any event handlers were removed
 */
bool lv_obj_remove_event_cb(struct _lv_obj_t * obj, lv_event_cb_t event_cb);

/**
 * Remove an event handler function for an object.
 * @param obj       pointer to an object
 * @param event_dsc pointer to an event descriptor to remove (returned by ::lv_obj_add_event_cb)
 * @return          true if any event handlers were removed
 */
bool lv_obj_remove_event_dsc(struct _lv_obj_t * obj, struct _lv_event_dsc_t * event_dsc);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_EVENT_H*/
