
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
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


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
 * Phalcon\Db\Index
 *
 * Allows to define indexes to be used on tables. Indexes are a common way
 * to enhance database performance. An index allows the database server to find
 * and retrieve specific rows much faster than it could do without an index
 */
ZEPHIR_INIT_CLASS(Phalcon_Db_Index) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Db, Index, phalcon, db_index, phalcon_db_index_method_entry, 0);

/**
	 * Index name
	 *
	 * @var string
	 */
	zend_declare_property_null(phalcon_db_index_ce, SL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
/**
	 * Index columns
	 *
	 * @var array
	 */
	zend_declare_property_null(phalcon_db_index_ce, SL("_columns"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_db_index_ce TSRMLS_CC, 1, phalcon_db_indexinterface_ce);

	return SUCCESS;

}

/**
 * Index name
 *
 * @var string
 */
PHP_METHOD(Phalcon_Db_Index, getName) {


	RETURN_MEMBER(this_ptr, "_name");

}

/**
 * Index columns
 *
 * @var array
 */
PHP_METHOD(Phalcon_Db_Index, getColumns) {


	RETURN_MEMBER(this_ptr, "_columns");

}

/**
 * Phalcon\Db\Index constructor
 *
 * @param string name
 * @param array columns
 */
PHP_METHOD(Phalcon_Db_Index, __construct) {

	zval *name_param = NULL, *columns;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &name_param, &columns);

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


	zephir_update_property_this(this_ptr, SL("_name"), name TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_columns"), columns TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Restore a Phalcon\Db\Index object from export
 *
 * @param array data
 */
PHP_METHOD(Phalcon_Db_Index, __set_state) {

	zval *data, *indexName, *columns;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &data);



	ZEPHIR_OBS_VAR(indexName);
	if (!(zephir_array_isset_string_fetch(&indexName, data, SS("_indexName"), 0 TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "_indexName parameter is required");
		return;
	}
	ZEPHIR_OBS_VAR(columns);
	if (!(zephir_array_isset_string_fetch(&columns, data, SS("_columns"), 0 TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "_columns parameter is required");
		return;
	}
	object_init_ex(return_value, phalcon_db_index_ce);
	zephir_call_method_p2_noret(return_value, "__construct", indexName, columns);
	RETURN_MM();

}

