
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
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
 * Phalcon\Filter
 *
 * The Phalcon\Filter component provides a set of commonly needed data filters. It provides
 * object oriented wrappers to the php filter extension. Also allows the developer to
 * define his/her own filters
 *
 *<code>
 *	$filter = new Phalcon\Filter();
 *	$filter->sanitize("some(one)@exa\\mple.com", "email"); // returns "someone@example.com"
 *	$filter->sanitize("hello<<", "string"); // returns "hello"
 *	$filter->sanitize("!100a019", "int"); // returns "100019"
 *	$filter->sanitize("!100a019.01a", "float"); // returns "100019.01"
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Filter) {

	ZEPHIR_REGISTER_CLASS(Phalcon, Filter, phalcon, filter, phalcon_filter_method_entry, 0);

	zend_declare_property_null(phalcon_filter_ce, SL("_filters"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_filter_ce TSRMLS_CC, 1, phalcon_filterinterface_ce);

	return SUCCESS;

}

/**
 * Adds a user-defined filter
 *
 * @param string name
 * @param callable handler
 * @return Phalcon\Filter
 */
PHP_METHOD(Phalcon_Filter, add) {

	zval *name_param = NULL, *handler;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &name_param, &handler);

	if (Z_TYPE_P(name_param) != IS_STRING && Z_TYPE_P(name_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'name' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(name_param) == IS_STRING) {
		name = name_param;
	} else {
		ZEPHIR_INIT_VAR(name);
		ZVAL_EMPTY_STRING(name);
	}


	if ((Z_TYPE_P(handler) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_filter_exception_ce, "Filter must be an object");
		return;
	}
	zephir_update_property_array(this_ptr, SL("_filters"), name, handler TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Sanitizes a value with a specified single or set of filters
 *
 * @param  value
 * @param  filters
 * @param  noRecursive
 * @return mixed
 */
PHP_METHOD(Phalcon_Filter, sanitize) {

	zend_function *_8 = NULL, *_9 = NULL, *_13 = NULL;
	HashTable *_1, *_5, *_11;
	HashPosition _0, _4, _10;
	zend_bool noRecursive, _3;
	zval *value, *filters, *noRecursive_param = NULL, *filter = NULL, *arrayValue = NULL, *itemKey = NULL, *itemValue = NULL, *sanitizedValue, **_2, **_6, *_7 = NULL, **_12;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &value, &filters, &noRecursive_param);

	if (!noRecursive_param) {
		noRecursive = 0;
	} else {
		noRecursive = zephir_get_boolval(noRecursive_param);
	}


	if ((Z_TYPE_P(filters) == IS_ARRAY)) {
		if ((Z_TYPE_P(value) != IS_NULL)) {
			zephir_is_iterable(filters, &_1, &_0, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
				; zend_hash_move_forward_ex(_1, &_0)
			) {
				ZEPHIR_GET_HVALUE(filter, _2);
				_3 = (Z_TYPE_P(value) == IS_ARRAY);
				if (_3) {
					_3 = !noRecursive;
				}
				if (_3) {
					ZEPHIR_INIT_NVAR(arrayValue);
					array_init(arrayValue);
					zephir_is_iterable(value, &_5, &_4, 0, 0);
					for (
						; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
						; zend_hash_move_forward_ex(_5, &_4)
					) {
						ZEPHIR_GET_HMKEY(itemKey, _5, _4);
						ZEPHIR_GET_HVALUE(itemValue, _6);
						ZEPHIR_INIT_NVAR(_7);
						zephir_call_method_p2_cache(_7, this_ptr, "_sanitize", &_8, itemValue, filter);
						zephir_array_update_zval(&arrayValue, itemKey, &_7, PH_COPY | PH_SEPARATE);
					}
					RETURN_CCTOR(arrayValue);
				}
				zephir_call_method_p2_cache(return_value, this_ptr, "_sanitize", &_9, value, filter);
				RETURN_MM();
			}
		}
		RETURN_MM_NULL();
	}
	if ((Z_TYPE_P(value) == IS_ARRAY)) {
		ZEPHIR_INIT_VAR(sanitizedValue);
		array_init(sanitizedValue);
		zephir_is_iterable(value, &_11, &_10, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_11, (void**) &_12, &_10) == SUCCESS
			; zend_hash_move_forward_ex(_11, &_10)
		) {
			ZEPHIR_GET_HMKEY(itemKey, _11, _10);
			ZEPHIR_GET_HVALUE(itemValue, _12);
			ZEPHIR_INIT_NVAR(_7);
			zephir_call_method_p2_cache(_7, this_ptr, "_sanitize", &_13, itemValue, filters);
			zephir_array_update_zval(&sanitizedValue, itemKey, &_7, PH_COPY | PH_SEPARATE);
		}
		RETURN_CCTOR(sanitizedValue);
	}
	zephir_call_method_p2(return_value, this_ptr, "_sanitize", value, filters);
	RETURN_MM();

}

/**
 * Internal sanitize wrapper to filter_var
 *
 * @param  mixed $value
 * @param  string $filter
 * @return mixed
 */
PHP_METHOD(Phalcon_Filter, _sanitize) {

	zval *_1 = NULL;
	zval *filter = NULL, *_7;
	zval *value, *filter_param = NULL, *filterObject, *_0, *_2, _3 = zval_used_for_init, _4, _5, *_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &filter_param);

	if (Z_TYPE_P(filter_param) != IS_STRING && Z_TYPE_P(filter_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'filter' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(filter_param) == IS_STRING) {
		filter = filter_param;
	} else {
		ZEPHIR_INIT_VAR(filter);
		ZVAL_EMPTY_STRING(filter);
	}


	ZEPHIR_OBS_VAR(filterObject);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_filters"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&filterObject, _0, filter, 0 TSRMLS_CC)) {
		if (zephir_is_instance_of(filterObject, SL("Closure") TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(_1);
			array_init_size(_1, 2);
			zephir_array_fast_append(_1, value);
			ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, filterObject, _1);
			RETURN_MM();
		}
		zephir_call_method_p1(return_value, filterObject, "filter", value);
		RETURN_MM();
	}
	do {
		if (ZEPHIR_IS_STRING(filter, "email")) {
			ZEPHIR_INIT_VAR(_2);
			ZEPHIR_SINIT_VAR(_3);
			ZVAL_STRING(&_3, "'", 0);
			ZEPHIR_SINIT_VAR(_4);
			ZVAL_STRING(&_4, "", 0);
			zephir_fast_str_replace(_2, &_3, &_4, value);
			ZEPHIR_SINIT_VAR(_5);
			ZVAL_LONG(&_5, 517);
			zephir_call_func_p2(return_value, "filter_var", _2, &_5);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "int")) {
			ZEPHIR_SINIT_NVAR(_3);
			ZVAL_LONG(&_3, 519);
			zephir_call_func_p2(return_value, "filter_var", value, &_3);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "string")) {
			ZEPHIR_SINIT_NVAR(_3);
			ZVAL_LONG(&_3, 513);
			zephir_call_func_p2(return_value, "filter_var", value, &_3);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "float")) {
			ZEPHIR_INIT_NVAR(_1);
			array_init_size(_1, 2);
			add_assoc_long_ex(_1, SS("flags"), 4096);
			ZEPHIR_SINIT_NVAR(_3);
			ZVAL_LONG(&_3, 520);
			zephir_call_func_p3(return_value, "filter_var", value, &_3, _1);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "alphanum")) {
			zephir_call_func_p1(return_value, "phalcon_filter_alphanum", value);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "trim")) {
			zephir_fast_trim(return_value, value, NULL , ZEPHIR_TRIM_BOTH TSRMLS_CC);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "striptags")) {
			zephir_call_func_p1(return_value, "strip_tags", value);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "lower")) {
			if ((zephir_function_exists_ex(SS("mb_strtolower") TSRMLS_CC) == SUCCESS)) {
				zephir_call_func_p1(return_value, "mb_strtolower", value);
				RETURN_MM();
			}
			zephir_fast_strtolower(return_value, value);
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(filter, "upper")) {
			if ((zephir_function_exists_ex(SS("mb_strtoupper") TSRMLS_CC) == SUCCESS)) {
				zephir_call_func_p1(return_value, "mb_strtoupper", value);
				RETURN_MM();
			}
			zephir_fast_strtoupper(return_value, value);
			RETURN_MM();
		}
		ZEPHIR_INIT_VAR(_6);
		object_init_ex(_6, phalcon_filter_exception_ce);
		ZEPHIR_INIT_VAR(_7);
		ZEPHIR_CONCAT_SVS(_7, "Sanitize filter '", filter, "' is not supported");
		zephir_call_method_p1_noret(_6, "__construct", _7);
		zephir_throw_exception(_6 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	} while(0);

	ZEPHIR_MM_RESTORE();

}

/**
 * Return the user-defined filters in the instance
 *
 * @return object[]
 */
PHP_METHOD(Phalcon_Filter, getFilters) {


	RETURN_MEMBER(this_ptr, "_filters");

}

