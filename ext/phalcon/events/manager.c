
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/string.h"
#include "kernel/concat.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */
/**
 * Phalcon\Events\Manager
 *
 * Phalcon Events Manager, offers an easy way to intercept and manipulate, if needed,
 * the normal flow of operation. With the EventsManager the developer can create hooks or
 * plugins that will offer monitoring of data, manipulation, conditional execution and much more.
 *
 */
ZEPHIR_INIT_CLASS(Phalcon_Events_Manager) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Events, Manager, phalcon, events_manager, phalcon_events_manager_method_entry, 0);

	zend_declare_property_null(phalcon_events_manager_ce, SL("_events"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(phalcon_events_manager_ce, SL("_collect"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(phalcon_events_manager_ce, SL("_enablePriorities"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_events_manager_ce, SL("_responses"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_events_manager_ce TSRMLS_CC, 1, phalcon_events_managerinterface_ce);

	return SUCCESS;

}

/**
 * Attach a listener to the events manager
 *
 * @param string eventType
 * @param object handler
 * @param int priority
 */
PHP_METHOD(Phalcon_Events_Manager, attach) {

	zend_class_entry *_1;
	int priority;
	zval *eventType_param = NULL, *handler, *priority_param = NULL, *priorityQueue, *events, *_0, *_2 = NULL;
	zval *eventType = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &eventType_param, &handler, &priority_param);

	if (Z_TYPE_P(eventType_param) != IS_STRING && Z_TYPE_P(eventType_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'eventType' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(eventType_param) == IS_STRING) {
		eventType = eventType_param;
	} else {
		ZEPHIR_INIT_VAR(eventType);
		ZVAL_EMPTY_STRING(eventType);
	}
	if (!priority_param) {
		priority = 100;
	} else {
		if (Z_TYPE_P(priority_param) != IS_LONG) {
			zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'priority' must be a long/integer") TSRMLS_CC);
			RETURN_MM_NULL();
		}

		priority = Z_LVAL_P(priority_param);
	}


	if ((Z_TYPE_P(handler) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_events_exception_ce, "Event handler must be an Object");
		return;
	}
	ZEPHIR_OBS_VAR(events);
	zephir_read_property_this(&events, this_ptr, SL("_events"), PH_NOISY_CC);
	if ((Z_TYPE_P(events) != IS_ARRAY)) {
		ZEPHIR_INIT_BNVAR(events);
		array_init(events);
	}
	ZEPHIR_OBS_VAR(priorityQueue);
	if (zephir_array_isset_fetch(&priorityQueue, events, eventType, 0 TSRMLS_CC)) {
		ZEPHIR_INIT_BNVAR(priorityQueue);
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_enablePriorities"), PH_NOISY_CC);
		if (zephir_is_true(_0)) {
			_1 = zend_fetch_class(SL("SplPriorityQueue"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
			object_init_ex(priorityQueue, _1);
			if (zephir_has_constructor(priorityQueue TSRMLS_CC)) {
				zephir_call_method_noret(priorityQueue, "__construct");
			}
			ZEPHIR_INIT_VAR(_2);
			ZVAL_LONG(_2, 1);
			zephir_call_method_p1_noret(priorityQueue, "setextractflags", _2);
			zephir_array_update_zval(&events, eventType, &priorityQueue, PH_COPY | PH_SEPARATE);
			zephir_update_property_this(this_ptr, SL("_events"), events TSRMLS_CC);
		} else {
			array_init(priorityQueue);
		}
	} else {
		ZEPHIR_INIT_BNVAR(priorityQueue);
		array_init(priorityQueue);
	}
	if ((Z_TYPE_P(priorityQueue) == IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_LONG(_2, priority);
		zephir_call_method_p2_noret(priorityQueue, "insert", handler, _2);
	} else {
		zephir_array_append(&priorityQueue, handler, PH_SEPARATE);
		zephir_array_update_zval(&events, eventType, &priorityQueue, PH_COPY | PH_SEPARATE);
		zephir_update_property_this(this_ptr, SL("_events"), events TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Set if priorities are enabled in the EventsManager
 *
 * @param boolean enablePriorities
 */
PHP_METHOD(Phalcon_Events_Manager, enablePriorities) {

	zval *enablePriorities_param = NULL;
	zend_bool enablePriorities;

	zephir_fetch_params(0, 1, 0, &enablePriorities_param);

		enablePriorities = zephir_get_boolval(enablePriorities_param);


	zephir_update_property_this(this_ptr, SL("_enablePriorities"), enablePriorities ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);

}

/**
 * Returns if priorities are enabled
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Events_Manager, arePrioritiesEnabled) {


	RETURN_MEMBER(this_ptr, "_enablePriorities");

}

/**
 * Tells the event manager if it needs to collect all the responses returned by every
 * registered listener in a single fire
 *
 * @param boolean collect
 */
PHP_METHOD(Phalcon_Events_Manager, collectResponses) {

	zval *collect_param = NULL;
	zend_bool collect;

	zephir_fetch_params(0, 1, 0, &collect_param);

		collect = zephir_get_boolval(collect_param);


	zephir_update_property_this(this_ptr, SL("_collect"), collect ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);

}

/**
 * Check if the events manager is collecting all all the responses returned by every
 * registered listener in a single fire
 */
PHP_METHOD(Phalcon_Events_Manager, isCollecting) {


	RETURN_MEMBER(this_ptr, "_collect");

}

/**
 * Returns all the responses returned by every handler executed by the last 'fire' executed
 *
 * @return array
 */
PHP_METHOD(Phalcon_Events_Manager, getResponses) {


	RETURN_MEMBER(this_ptr, "_responses");

}

/**
 * Removes all events from the EventsManager
 *
 * @param string type
 */
PHP_METHOD(Phalcon_Events_Manager, dettachAll) {

	zval *type_param = NULL, *events;
	zval *type = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &type_param);

	if (!type_param) {
		ZEPHIR_INIT_VAR(type);
		ZVAL_EMPTY_STRING(type);
	} else {
		zephir_get_strval(type, type_param);
	}


	ZEPHIR_OBS_VAR(events);
	zephir_read_property_this(&events, this_ptr, SL("_events"), PH_NOISY_CC);
	if (ZEPHIR_IS_STRING(type, "")) {
		ZEPHIR_INIT_BNVAR(events);
		ZVAL_NULL(events);
	} else {
		if (zephir_array_isset(events, type)) {
			zephir_array_update_zval(&events, type, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		}
	}
	zephir_update_property_this(this_ptr, SL("_events"), events TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Internal handler to call a queue of events
 *
 * @param \SplPriorityQueue|array queue
 * @param Phalcon\Events\Event event
 * @return mixed
 */
PHP_METHOD(Phalcon_Events_Manager, fireQueue) {

	HashTable *_5;
	HashPosition _4;
	zend_bool collect, cancelable, _0;
	zval *queue = NULL, *event, *status = NULL, *arguments = NULL, *eventName, *data, *iterator, *source, *handler = NULL, *_1 = NULL, *_2, *_3 = NULL, **_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &queue, &event);

	ZEPHIR_SEPARATE_PARAM(queue);


	_0 = (Z_TYPE_P(queue) != IS_ARRAY);
	if (_0) {
		_0 = (Z_TYPE_P(queue) != IS_OBJECT);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_events_exception_ce, "The queue is not valid");
		return;
	}
	if ((Z_TYPE_P(event) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_events_exception_ce, "The event is not valid");
		return;
	}
	ZEPHIR_INIT_VAR(status);
	ZVAL_NULL(status);
	ZEPHIR_INIT_VAR(arguments);
	ZVAL_NULL(arguments);
	ZEPHIR_INIT_VAR(eventName);
	zephir_call_method(eventName, event, "gettype");
	if ((Z_TYPE_P(eventName) != IS_STRING)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_events_exception_ce, "The event type not valid");
		return;
	}
	ZEPHIR_INIT_VAR(source);
	zephir_call_method(source, event, "getsource");
	ZEPHIR_INIT_VAR(data);
	zephir_call_method(data, event, "getdata");
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method(_1, event, "getcancelable");
	cancelable = zephir_get_boolval(_1);
	ZEPHIR_OBS_VAR(_2);
	zephir_read_property_this(&_2, this_ptr, SL("_collect"), PH_NOISY_CC);
	collect = zephir_get_boolval(_2);
	if (ZEPHIR_IS_STRING(queue, "object")) {
		ZEPHIR_INIT_VAR(iterator);
		if (zephir_clone(iterator, queue TSRMLS_CC) == FAILURE) {
			RETURN_MM();
		}
		zephir_call_method_noret(iterator, "top");
		while (1) {
			ZEPHIR_INIT_NVAR(_1);
			zephir_call_method(_1, iterator, "valid");
			if (!(zephir_is_true(_1))) {
				break;
			}
			ZEPHIR_INIT_NVAR(handler);
			zephir_call_method(handler, iterator, "current");
			if ((Z_TYPE_P(handler) == IS_OBJECT)) {
				if (zephir_is_instance_of(handler, SL("Closure") TSRMLS_CC)) {
					if ((Z_TYPE_P(arguments) == IS_NULL)) {
						ZEPHIR_INIT_NVAR(arguments);
						array_init_size(arguments, 5);
						zephir_array_fast_append(arguments, event);
						zephir_array_fast_append(arguments, source);
						zephir_array_fast_append(arguments, data);
					}
					ZEPHIR_INIT_NVAR(status);
					ZEPHIR_CALL_USER_FUNC_ARRAY(status, handler, arguments);
					if (collect) {
						zephir_update_property_array_append(this_ptr, SL("_responses"), status TSRMLS_CC);
					}
					if (cancelable) {
						ZEPHIR_INIT_NVAR(_3);
						zephir_call_method(_3, event, "isstopped");
						if (zephir_is_true(_3)) {
							break;
						}
					}
				} else {
					if ((zephir_method_exists(handler, eventName TSRMLS_CC)  == SUCCESS)) {
						ZEPHIR_INIT_NVAR(status);
						zephir_call_method_zval_p3(status, handler, eventName, event, source, data);
						if (collect) {
							zephir_update_property_array_append(this_ptr, SL("_responses"), status TSRMLS_CC);
						}
						if (cancelable) {
							ZEPHIR_INIT_NVAR(_3);
							zephir_call_method(_3, event, "isstopped");
							if (zephir_is_true(_3)) {
								break;
							}
						}
					}
				}
			}
		}
	} else {
		zephir_is_iterable(handler, &_5, &_4, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
			; zend_hash_move_forward_ex(_5, &_4)
		) {
			ZEPHIR_GET_HVALUE(queue, _6);
			if ((Z_TYPE_P(handler) == IS_OBJECT)) {
				if (zephir_is_instance_of(handler, SL("Closure") TSRMLS_CC)) {
					if ((Z_TYPE_P(arguments) == IS_NULL)) {
						ZEPHIR_INIT_NVAR(arguments);
						array_init_size(arguments, 5);
						zephir_array_fast_append(arguments, event);
						zephir_array_fast_append(arguments, source);
						zephir_array_fast_append(arguments, data);
					}
					ZEPHIR_INIT_NVAR(status);
					ZEPHIR_CALL_USER_FUNC_ARRAY(status, handler, arguments);
					if (collect) {
						zephir_update_property_array_append(this_ptr, SL("_responses"), status TSRMLS_CC);
					}
					if (cancelable) {
						ZEPHIR_INIT_NVAR(_3);
						zephir_call_method(_3, event, "isstopped");
						if (zephir_is_true(_3)) {
							break;
						}
					}
				} else {
					if ((zephir_method_exists(handler, eventName TSRMLS_CC)  == SUCCESS)) {
						ZEPHIR_INIT_NVAR(status);
						zephir_call_method_zval_p3(status, handler, eventName, event, source, data);
						if (collect) {
							zephir_update_property_array_append(this_ptr, SL("_responses"), status TSRMLS_CC);
						}
						if (cancelable) {
							ZEPHIR_INIT_NVAR(_3);
							zephir_call_method(_3, event, "isstopped");
							if (zephir_is_true(_3)) {
								break;
							}
						}
					}
				}
			}
		}
	}
	RETURN_CCTOR(status);

}

/**
 * Fires an event in the events manager causing that active listeners be notified about it
 *
 *<code>
 *	$eventsManager->fire('db', $connection);
 *</code>
 *
 * @param string eventType
 * @param object source
 * @param mixed  data
 * @param boolean cancelable
 * @return mixed
 */
PHP_METHOD(Phalcon_Events_Manager, fire) {

	zend_bool cancelable, _3;
	zval *eventType_param = NULL, *source, *data = NULL, *cancelable_param = NULL, *events, *eventParts, *type, *eventName, *event, *status, *fireEvents = NULL, *_0, *_2;
	zval *eventType = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 2, &eventType_param, &source, &data, &cancelable_param);

	if (Z_TYPE_P(eventType_param) != IS_STRING && Z_TYPE_P(eventType_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'eventType' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(eventType_param) == IS_STRING) {
		eventType = eventType_param;
	} else {
		ZEPHIR_INIT_VAR(eventType);
		ZVAL_EMPTY_STRING(eventType);
	}
	if (!data) {
		data = ZEPHIR_GLOBAL(global_null);
	}
	if (!cancelable_param) {
		cancelable = 1;
	} else {
		cancelable = zephir_get_boolval(cancelable_param);
	}


	ZEPHIR_OBS_VAR(events);
	zephir_read_property_this(&events, this_ptr, SL("_events"), PH_NOISY_CC);
	if ((Z_TYPE_P(events) != IS_ARRAY)) {
		RETURN_MM_NULL();
	}
	if (!(zephir_memnstr_str(eventType, SL(":"), "phalcon/events/manager.zep", 427))) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, phalcon_events_exception_ce);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SV(_1, "Invalid event type ", eventType);
		zephir_call_method_p1_noret(_0, "__construct", _1);
		zephir_throw_exception(_0 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(eventParts);
	zephir_fast_explode_str(eventParts, SL(":"), eventType, LONG_MAX TSRMLS_CC);
	ZEPHIR_OBS_VAR(type);
	zephir_array_fetch_long(&type, eventParts, 0, PH_NOISY TSRMLS_CC);
	ZEPHIR_OBS_VAR(eventName);
	zephir_array_fetch_long(&eventName, eventParts, 1, PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(status);
	ZVAL_NULL(status);
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("_collect"), PH_NOISY_CC);
	if (zephir_is_true(_2)) {
		zephir_update_property_this(this_ptr, SL("_responses"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(event);
	ZVAL_NULL(event);
	ZEPHIR_OBS_VAR(fireEvents);
	if (zephir_array_isset_fetch(&fireEvents, events, type, 0 TSRMLS_CC)) {
		_3 = (Z_TYPE_P(fireEvents) == IS_OBJECT);
		if (!(_3)) {
			_3 = (Z_TYPE_P(fireEvents) == IS_ARRAY);
		}
		if (_3) {
			ZEPHIR_INIT_BNVAR(event);
			object_init_ex(event, phalcon_events_event_ce);
			zephir_call_method_p4_noret(event, "__construct", eventName, source, data, (cancelable ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
			ZEPHIR_INIT_BNVAR(status);
			zephir_call_method_p2(status, this_ptr, "firequeue", fireEvents, event);
		}
	}
	ZEPHIR_OBS_NVAR(fireEvents);
	if (zephir_array_isset_fetch(&fireEvents, events, eventType, 0 TSRMLS_CC)) {
		_3 = (Z_TYPE_P(fireEvents) == IS_OBJECT);
		if (!(_3)) {
			_3 = (Z_TYPE_P(fireEvents) == IS_ARRAY);
		}
		if (_3) {
			if ((Z_TYPE_P(event) == IS_NULL)) {
				ZEPHIR_INIT_BNVAR(event);
				object_init_ex(event, phalcon_events_event_ce);
				zephir_call_method_p4_noret(event, "__construct", eventName, source, data, (cancelable ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
			}
			ZEPHIR_INIT_BNVAR(status);
			zephir_call_method_p2(status, this_ptr, "firequeue", fireEvents, event);
		}
	}
	RETURN_CCTOR(status);

}

/**
 * Check whether certain type of event has listeners
 *
 * @param string type
 * @return boolean
 */
PHP_METHOD(Phalcon_Events_Manager, hasListeners) {

	zval *type_param = NULL, *_0;
	zval *type = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type_param);

	if (Z_TYPE_P(type_param) != IS_STRING && Z_TYPE_P(type_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'type' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(type_param) == IS_STRING) {
		type = type_param;
	} else {
		ZEPHIR_INIT_VAR(type);
		ZVAL_EMPTY_STRING(type);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_events"), PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_array_isset(_0, type));

}

/**
 * Returns all the attached listeners of a certain type
 *
 * @param string type
 * @return array
 */
PHP_METHOD(Phalcon_Events_Manager, getListeners) {

	zval *type_param = NULL, *events, *fireEvents;
	zval *type = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type_param);

	if (Z_TYPE_P(type_param) != IS_STRING && Z_TYPE_P(type_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'type' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(type_param) == IS_STRING) {
		type = type_param;
	} else {
		ZEPHIR_INIT_VAR(type);
		ZVAL_EMPTY_STRING(type);
	}


	events = zephir_fetch_nproperty_this(this_ptr, SL("_events"), PH_NOISY_CC);
	if ((Z_TYPE_P(events) == IS_ARRAY)) {
		if (zephir_array_isset_fetch(&fireEvents, events, type, 1 TSRMLS_CC)) {
			RETURN_CTOR(fireEvents);
		}
	}
	array_init(return_value);
	RETURN_MM();

}

