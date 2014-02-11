
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
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "ext/spl/spl_exceptions.h"


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
 * Phalcon\Forms\Form
 *
 * This component allows to build forms using an object-oriented interface
 */
ZEPHIR_INIT_CLASS(Phalcon_Forms_Form) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Forms, Form, phalcon, forms_form, phalcon_di_injectable_ce, phalcon_forms_form_method_entry, 0);

	zend_declare_property_null(phalcon_forms_form_ce, SL("_position"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_entity"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_options"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_data"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_elements"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_elementsIndexed"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_messages"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_forms_form_ce, SL("_action"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_forms_form_ce TSRMLS_CC, 1, spl_ce_Countable);
	zend_class_implements(phalcon_forms_form_ce TSRMLS_CC, 1, zend_ce_iterator);

	return SUCCESS;

}

/**
 * Phalcon\Forms\Form constructor
 *
 * @param object entity
 * @param array userOptions
 */
PHP_METHOD(Phalcon_Forms_Form, __construct) {

	zval *entity = NULL, *userOptions = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &entity, &userOptions);

	if (!entity) {
		entity = ZEPHIR_GLOBAL(global_null);
	}
	if (!userOptions) {
		userOptions = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(entity) != IS_NULL)) {
		if ((Z_TYPE_P(entity) != IS_OBJECT)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_forms_exception_ce, "The base entity is not valid");
			return;
		}
		zephir_update_property_this(this_ptr, SL("_entity"), entity TSRMLS_CC);
	}
	if ((Z_TYPE_P(userOptions) == IS_ARRAY)) {
		zephir_update_property_this(this_ptr, SL("_options"), userOptions TSRMLS_CC);
	}
	if ((zephir_method_exists_ex(this_ptr, SS("initialize") TSRMLS_CC) == SUCCESS)) {
		zephir_call_method_p2_noret(this_ptr, "initialize", entity, userOptions);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the form's action
 *
 * @param string action
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, setAction) {

	zval *action;

	zephir_fetch_params(0, 1, 0, &action);



	zephir_update_property_this(this_ptr, SL("_action"), action TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns the form's action
 *
 * @return string
 */
PHP_METHOD(Phalcon_Forms_Form, getAction) {


	RETURN_MEMBER(this_ptr, "_action");

}

/**
 * Sets an option for the form
 *
 * @param string option
 * @param mixed value
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, setUserOption) {

	zval *option, *value;

	zephir_fetch_params(0, 2, 0, &option, &value);



	zephir_update_property_array(this_ptr, SL("_options"), option, value TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns the value of an option if present
 *
 * @param string option
 * @param mixed defaultValue
 * @return mixed
 */
PHP_METHOD(Phalcon_Forms_Form, getUserOption) {

	zval *option, *defaultValue = NULL, *value, *options;

	zephir_fetch_params(0, 1, 1, &option, &defaultValue);

	if (!defaultValue) {
		defaultValue = ZEPHIR_GLOBAL(global_null);
	}


	options = zephir_fetch_nproperty_this(this_ptr, SL("_options"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&value, options, option, 1 TSRMLS_CC)) {
		RETURN_CTORW(value);
	}
	RETURN_CCTORW(defaultValue);

}

/**
 * Sets options for the element
 *
 * @param array options
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, setUserOptions) {

	zval *options;

	zephir_fetch_params(0, 1, 0, &options);



	if ((Z_TYPE_P(options) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STRW(phalcon_forms_exception_ce, "Parameter 'options' must be an array");
		return;
	}
	zephir_update_property_this(this_ptr, SL("_options"), options TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns the options for the element
 *
 * @return array
 */
PHP_METHOD(Phalcon_Forms_Form, getUserOptions) {


	RETURN_MEMBER(this_ptr, "_options");

}

/**
 * Sets the entity related to the model
 *
 * @param object entity
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, setEntity) {

	zval *entity;

	zephir_fetch_params(0, 1, 0, &entity);



	zephir_update_property_this(this_ptr, SL("_entity"), entity TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns the entity related to the model
 *
 * @return object
 */
PHP_METHOD(Phalcon_Forms_Form, getEntity) {


	RETURN_MEMBER(this_ptr, "_entity");

}

/**
 * Returns the form elements added to the form
 *
 * @return Phalcon\Forms\ElementInterface[]
 */
PHP_METHOD(Phalcon_Forms_Form, getElements) {


	RETURN_MEMBER(this_ptr, "_elements");

}

/**
 * Binds data to the entity
 *
 * @param array data
 * @param object entity
 * @param array whitelist
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, bind) {

	zend_function *_3 = NULL, *_6 = NULL, *_7 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *data, *entity, *whitelist = NULL, *elements, *filter = NULL, *key = NULL, *value = NULL, *element = NULL, *filters = NULL, *dependencyInjector = NULL, *filteredValue = NULL, *method = NULL, **_2, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &data, &entity, &whitelist);

	ZEPHIR_SEPARATE_PARAM(entity);
	if (!whitelist) {
		whitelist = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_forms_exception_ce, "The data must be an array");
		return;
	}
	ZEPHIR_OBS_VAR(elements);
	zephir_read_property_this(&elements, this_ptr, SL("_elements"), PH_NOISY_CC);
	if ((Z_TYPE_P(elements) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_forms_exception_ce, "There are no elements in the form");
		return;
	}
	ZEPHIR_INIT_VAR(filter);
	ZVAL_NULL(filter);
	zephir_is_iterable(data, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		if (!(zephir_array_isset(elements, key))) {
			continue;
		}
		if ((Z_TYPE_P(whitelist) == IS_ARRAY)) {
			if (!(zephir_fast_in_array(key, whitelist TSRMLS_CC))) {
				continue;
			}
		}
		ZEPHIR_OBS_NVAR(element);
		zephir_array_fetch(&element, elements, key, PH_NOISY TSRMLS_CC);
		ZEPHIR_INIT_NVAR(filters);
		zephir_call_method(filters, element, "getfilters");
		if (zephir_is_true(filters)) {
			if ((Z_TYPE_P(filter) != IS_OBJECT)) {
				ZEPHIR_INIT_NVAR(dependencyInjector);
				zephir_call_method_cache(dependencyInjector, this_ptr, "getdi", &_3);
				ZEPHIR_INIT_NVAR(_4);
				ZEPHIR_INIT_NVAR(_5);
				ZVAL_STRING(_5, "filter", 1);
				zephir_call_method_p1_cache(_4, dependencyInjector, "getshared", &_6, _5);
				ZEPHIR_CPY_WRT(filter, _4);
			}
			ZEPHIR_INIT_NVAR(filteredValue);
			zephir_call_method_p2_cache(filteredValue, filter, "sanitize", &_7, value, filters);
		} else {
			ZEPHIR_CPY_WRT(filteredValue, value);
		}
		ZEPHIR_INIT_NVAR(method);
		ZEPHIR_CONCAT_SV(method, "set", key);
		if ((zephir_method_exists(entity, method TSRMLS_CC)  == SUCCESS)) {
			zephir_call_method_zval_p1_noret(entity, method, filteredValue);
			continue;
		}
		zephir_update_property_zval_zval(entity, key, filteredValue TSRMLS_CC);
	}
	zephir_update_property_this(this_ptr, SL("_data"), data TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Validates the form
 *
 * @param array data
 * @param object entity
 * @return boolean
 */
PHP_METHOD(Phalcon_Forms_Form, isValid) {

	zend_function *_8 = NULL, *_9 = NULL, *_10 = NULL;
	zval *_7 = NULL;
	HashTable *_2, *_5;
	HashPosition _1, _4;
	zend_bool notFailed;
	zval *data = NULL, *entity = NULL, *elements, *messages, *element = NULL, *validators = NULL, *name = NULL, *preparedValidators = NULL, *filters = NULL, *validator = NULL, *validation = NULL, *elementMessages = NULL, *_0 = NULL, **_3, **_6, *_11 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &data, &entity);

	if (!data) {
		ZEPHIR_CPY_WRT(data, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(data);
	}
	if (!entity) {
		entity = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(elements);
	zephir_read_property_this(&elements, this_ptr, SL("_elements"), PH_NOISY_CC);
	if ((Z_TYPE_P(elements) != IS_ARRAY)) {
		RETURN_MM_BOOL(1);
	}
	if ((Z_TYPE_P(entity) == IS_OBJECT)) {
		zephir_call_method_p2_noret(this_ptr, "bind", data, entity);
	}
	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_OBS_NVAR(data);
		zephir_read_property_this(&data, this_ptr, SL("_data"), PH_NOISY_CC);
	}
	if ((zephir_method_exists_ex(this_ptr, SS("beforevalidation") TSRMLS_CC) == SUCCESS)) {
		ZEPHIR_INIT_VAR(_0);
		zephir_call_method_p2(_0, this_ptr, "beforevalidation", data, entity);
		if (ZEPHIR_IS_FALSE(_0)) {
			RETURN_MM_BOOL(0);
		}
	}
	notFailed = 1;
	ZEPHIR_INIT_VAR(messages);
	array_init(messages);
	zephir_is_iterable(elements, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(element, _3);
		ZEPHIR_INIT_NVAR(validators);
		zephir_call_method(validators, element, "getvalidators");
		if ((Z_TYPE_P(validators) == IS_ARRAY)) {
			if (zephir_fast_count_int(validators TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(name);
				zephir_call_method(name, element, "getname");
				ZEPHIR_INIT_NVAR(preparedValidators);
				array_init(preparedValidators);
				zephir_is_iterable(validators, &_5, &_4, 0, 0);
				for (
					; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
					; zend_hash_move_forward_ex(_5, &_4)
				) {
					ZEPHIR_GET_HVALUE(validator, _6);
					ZEPHIR_INIT_NVAR(_7);
					array_init_size(_7, 3);
					zephir_array_fast_append(_7, name);
					zephir_array_fast_append(_7, validator);
					zephir_array_append(&preparedValidators, _7, PH_SEPARATE);
				}
				ZEPHIR_INIT_NVAR(validation);
				object_init_ex(validation, phalcon_validation_ce);
				zephir_call_method_p1_cache_noret(validation, "__construct", &_8, preparedValidators);
				ZEPHIR_INIT_NVAR(filters);
				zephir_call_method(filters, element, "getfilters");
				if ((Z_TYPE_P(filters) == IS_ARRAY)) {
					ZEPHIR_INIT_NVAR(_0);
					zephir_call_method(_0, element, "getname");
					zephir_call_method_p2_cache_noret(validation, "setfilters", &_9, _0, filters);
				}
				ZEPHIR_INIT_NVAR(elementMessages);
				zephir_call_method_p2_cache(elementMessages, validation, "validate", &_10, data, entity);
				if (zephir_fast_count_int(elementMessages TSRMLS_CC)) {
					ZEPHIR_INIT_NVAR(_11);
					zephir_call_method(_11, element, "getname");
					zephir_array_update_zval(&messages, _11, &elementMessages, PH_COPY | PH_SEPARATE);
					notFailed = 0;
				}
			}
		}
	}
	if (!(notFailed)) {
		zephir_update_property_this(this_ptr, SL("_messages"), messages TSRMLS_CC);
	}
	if ((zephir_method_exists_ex(this_ptr, SS("aftervalidation") TSRMLS_CC) == SUCCESS)) {
		zephir_call_method_p1_noret(this_ptr, "aftervalidation", messages);
	}
	RETURN_MM_BOOL(notFailed);

}

/**
 * Returns the messages generated in the validation
 *
 * @param boolean byItemName
 * @return array
 */
PHP_METHOD(Phalcon_Forms_Form, getMessages) {

	zend_function *_3 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *byItemName_param = NULL, *messages, *group, *element = NULL, *elementMessages = NULL, **_2;
	zend_bool byItemName;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &byItemName_param);

	if (!byItemName_param) {
		byItemName = 0;
	} else {
		byItemName = zephir_get_boolval(byItemName_param);
	}


	ZEPHIR_OBS_VAR(messages);
	zephir_read_property_this(&messages, this_ptr, SL("_messages"), PH_NOISY_CC);
	if (byItemName) {
		if ((Z_TYPE_P(messages) != IS_ARRAY)) {
			object_init_ex(return_value, phalcon_validation_message_group_ce);
			zephir_call_method_noret(return_value, "__construct");
			RETURN_MM();
		}
		RETURN_CCTOR(messages);
	}
	ZEPHIR_INIT_VAR(group);
	object_init_ex(group, phalcon_validation_message_group_ce);
	zephir_call_method_noret(group, "__construct");
	zephir_is_iterable(messages, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(element, _1, _0);
		ZEPHIR_GET_HVALUE(elementMessages, _2);
		zephir_call_method_p1_cache_noret(group, "appendmessages", &_3, elementMessages);
	}
	RETURN_CCTOR(group);

}

/**
 * Returns the messages generated for a specific element
 *
 * @return Phalcon\Validation\Message\Group
 */
PHP_METHOD(Phalcon_Forms_Form, getMessagesFor) {

	zval *name, *messages, *elementMessages, *group;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name);



	messages = zephir_fetch_nproperty_this(this_ptr, SL("_messages"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&elementMessages, messages, name, 1 TSRMLS_CC)) {
		RETURN_CTOR(elementMessages);
	}
	ZEPHIR_INIT_VAR(group);
	object_init_ex(group, phalcon_validation_message_group_ce);
	zephir_call_method_noret(group, "__construct");
	zephir_update_property_array(this_ptr, SL("_messages"), name, group TSRMLS_CC);
	RETURN_CCTOR(group);

}

/**
 * Check if messages were generated for a specific element
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Forms_Form, hasMessagesFor) {

	zval *name, *_0;

	zephir_fetch_params(0, 1, 0, &name);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_messages"), PH_NOISY_CC);
	RETURN_BOOL(zephir_array_isset(_0, name));

}

/**
 * Adds an element to the form
 *
 * @param Phalcon\Forms\ElementInterface element
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, add) {

	zval *element, *name;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &element);



	if ((Z_TYPE_P(element) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_forms_exception_ce, "The element is not valid");
		return;
	}
	ZEPHIR_INIT_VAR(name);
	zephir_call_method(name, element, "getname");
	zephir_call_method_p1_noret(element, "setform", this_ptr);
	zephir_update_property_array(this_ptr, SL("_elements"), name, element TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Renders a specific item in the form
 *
 * @param string name
 * @param array attributes
 * @return string
 */
PHP_METHOD(Phalcon_Forms_Form, render) {

	zval *name_param = NULL, *attributes = NULL, *element, *_0, *_1;
	zval *name = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &name_param, &attributes);

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
	if (!attributes) {
		attributes = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(element);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	if (!(zephir_array_isset_fetch(&element, _0, name, 0 TSRMLS_CC))) {
		ZEPHIR_INIT_VAR(_1);
		object_init_ex(_1, phalcon_forms_exception_ce);
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_SVS(_2, "Element with ID=", name, " is not part of the form");
		zephir_call_method_p1_noret(_1, "__construct", _2);
		zephir_throw_exception(_1 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_call_method_p1(return_value, element, "render", attributes);
	RETURN_MM();

}

/**
 * Returns an element added to the form by its name
 *
 * @param string name
 * @return Phalcon\Forms\ElementInterface
 */
PHP_METHOD(Phalcon_Forms_Form, get) {

	zval *name_param = NULL, *element, *_0, *_1;
	zval *name = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	ZEPHIR_OBS_VAR(element);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&element, _0, name, 0 TSRMLS_CC)) {
		RETURN_CCTOR(element);
	}
	ZEPHIR_INIT_VAR(_1);
	object_init_ex(_1, phalcon_forms_exception_ce);
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_CONCAT_SVS(_2, "Element with ID=", name, " is not part of the form");
	zephir_call_method_p1_noret(_1, "__construct", _2);
	zephir_throw_exception(_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

/**
 * Generate the label of a element added to the form including HTML
 *
 * @param string name
 * @return string
 */
PHP_METHOD(Phalcon_Forms_Form, label) {

	zval *name_param = NULL, *element, *_0, *_1;
	zval *name = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	ZEPHIR_OBS_VAR(element);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&element, _0, name, 0 TSRMLS_CC)) {
		zephir_call_method(return_value, element, "label");
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(_1);
	object_init_ex(_1, phalcon_forms_exception_ce);
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_CONCAT_SVS(_2, "Element with ID=", name, " is not part of the form");
	zephir_call_method_p1_noret(_1, "__construct", _2);
	zephir_throw_exception(_1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

/**
 * Returns a label for an element
 *
 * @param string name
 * @return string
 */
PHP_METHOD(Phalcon_Forms_Form, getLabel) {

	zval *name_param = NULL, *element, *label, *_0, *_1;
	zval *name = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	ZEPHIR_OBS_VAR(element);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	if (!(zephir_array_isset_fetch(&element, _0, name, 0 TSRMLS_CC))) {
		ZEPHIR_INIT_VAR(_1);
		object_init_ex(_1, phalcon_forms_exception_ce);
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_SVS(_2, "Element with ID=", name, " is not part of the form");
		zephir_call_method_p1_noret(_1, "__construct", _2);
		zephir_throw_exception(_1 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(label);
	zephir_call_method(label, element, "getlabel");
	if (!(zephir_is_true(label))) {
		RETURN_CTOR(name);
	}
	RETURN_CCTOR(label);

}

/**
 * Gets a value from the internal related entity or from the default value
 *
 * @param string name
 * @return mixed
 */
PHP_METHOD(Phalcon_Forms_Form, getValue) {

	zval *name_param = NULL, *entity, *method = NULL, *value = NULL, *data;
	zval *name = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	ZEPHIR_OBS_VAR(entity);
	zephir_read_property_this(&entity, this_ptr, SL("_entity"), PH_NOISY_CC);
	if ((Z_TYPE_P(entity) == IS_OBJECT)) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_CONCAT_SV(_0, "get", name);
		ZEPHIR_CPY_WRT(method, _0);
		if ((zephir_method_exists(entity, method TSRMLS_CC)  == SUCCESS)) {
			zephir_call_method_zval(return_value, entity, method);
			RETURN_MM();
		}
		ZEPHIR_OBS_VAR(value);
		if (zephir_read_property_zval(&value, entity, name, PH_SILENT_CC)) {
			RETURN_CCTOR(value);
		}
	}
	data = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
	if ((Z_TYPE_P(data) == IS_ARRAY)) {
		ZEPHIR_OBS_NVAR(value);
		if (zephir_array_isset_fetch(&value, data, name, 0 TSRMLS_CC)) {
			RETURN_CCTOR(value);
		}
	}
	RETURN_MM_NULL();

}

/**
 * Check if the form contains an element
 *
 * @param string name
 * @return boolean
 */
PHP_METHOD(Phalcon_Forms_Form, has) {

	zval *name_param = NULL, *_0;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_array_isset(_0, name));

}

/**
 * Removes an element from the form
 *
 * @param string name
 * @return boolean
 */
PHP_METHOD(Phalcon_Forms_Form, remove) {

	zval *name_param = NULL, *elements, *_0, *_1, *_2;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

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


	elements = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	if (zephir_array_isset(elements, name)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
		zephir_array_fetch(&_2, _1, name, PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_unset(&_0, _2, PH_SEPARATE);
		RETURN_MM_BOOL(1);
	}
	zephir_update_property_this(this_ptr, SL("_elementsIndexed"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	RETURN_MM_BOOL(0);

}

/**
 * Clears every element in the form to its default value
 *
 * @param array fields
 * @return Phalcon\Forms\Form
 */
PHP_METHOD(Phalcon_Forms_Form, clear) {

	HashTable *_1;
	HashPosition _0;
	zval *fields = NULL, *elements, *element = NULL, **_2, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &fields);

	if (!fields) {
		fields = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(elements);
	zephir_read_property_this(&elements, this_ptr, SL("_elements"), PH_NOISY_CC);
	if ((Z_TYPE_P(elements) == IS_ARRAY)) {
		zephir_is_iterable(elements, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(element, _2);
			if ((Z_TYPE_P(fields) != IS_ARRAY)) {
				zephir_call_method_noret(element, "clear");
			} else {
				ZEPHIR_INIT_NVAR(_3);
				zephir_call_method(_3, element, "getname");
				if (zephir_fast_in_array(_3, fields TSRMLS_CC)) {
					zephir_call_method_noret(element, "clear");
				}
			}
		}
	}
	RETURN_THIS();

}

/**
 * Returns the number of elements in the form
 *
 * @return int
 */
PHP_METHOD(Phalcon_Forms_Form, count) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	RETURN_LONG(zephir_fast_count_int(_0 TSRMLS_CC));

}

/**
 * Rewinds the internal iterator
 */
PHP_METHOD(Phalcon_Forms_Form, rewind) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("_position"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elements"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "array_values", _0);
	zephir_update_property_this(this_ptr, SL("_elementsIndexed"), _1 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the current element in the iterator
 *
 * @return Phalcon\Forms\ElementInterface
 */
PHP_METHOD(Phalcon_Forms_Form, current) {

	zval *element, *_0, *_1;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elementsIndexed"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_position"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&element, _0, _1, 1 TSRMLS_CC)) {
		RETURN_CTORW(element);
	}
	RETURN_BOOL(0);

}

/**
 * Returns the current position/key in the iterator
 *
 * @return int
 */
PHP_METHOD(Phalcon_Forms_Form, key) {


	RETURN_MEMBER(this_ptr, "_position");

}

/**
 * Moves the internal iteration pointer to the next position
 *
 */
PHP_METHOD(Phalcon_Forms_Form, next) {


	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("_position") TSRMLS_CC));

}

/**
 * Check if the current element in the iterator is valid
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Forms_Form, valid) {

	zval *_0, *_1;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_elementsIndexed"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_position"), PH_NOISY_CC);
	RETURN_BOOL(zephir_array_isset(_0, _1));

}

