
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/string.h"
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
 * Phalcon\Db\Adapter\Pdo
 *
 * Phalcon\Db\Adapter\Pdo is the Phalcon\Db that internally uses PDO to connect to a database
 *
 *<code>
 *	$connection = new Phalcon\Db\Adapter\Pdo\Mysql(array(
 *		'host' => '192.168.0.11',
 *		'username' => 'sigma',
 *		'password' => 'secret',
 *		'dbname' => 'blog',
 *		'port' => '3306'
 *	));
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Db_Adapter_Pdo) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Db\\Adapter, Pdo, phalcon, db_adapter_pdo, phalcon_db_adapter_ce, phalcon_db_adapter_pdo_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

/**
	 * PDO Handler
	 */
	zend_declare_property_null(phalcon_db_adapter_pdo_ce, SL("_pdo"), ZEND_ACC_PROTECTED TSRMLS_CC);
/**
	 * Last affected rows
	 */
	zend_declare_property_null(phalcon_db_adapter_pdo_ce, SL("_affectedRows"), ZEND_ACC_PROTECTED TSRMLS_CC);
/**
	 * Transaction level
	 */
	zend_declare_property_long(phalcon_db_adapter_pdo_ce, SL("_transactionLevel"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

/**
 * Constructor for Phalcon\Db\Adapter\Pdo
 *
 * @param array descriptor
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, __construct) {

	zval *descriptor;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &descriptor);



	if ((Z_TYPE_P(descriptor) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "The descriptor must be an array");
		return;
	}
	zephir_call_method_p1_noret(this_ptr, "connect", descriptor);
	zephir_call_parent_p1_noret(this_ptr, phalcon_db_adapter_pdo_ce, "__construct", descriptor);
	ZEPHIR_MM_RESTORE();

}

/**
 * This method is automatically called in Phalcon\Db\Adapter\Pdo constructor.
 * Call it when you need to restore a database connection
 *
 *<code>
 * //Make a connection
 * $connection = new Phalcon\Db\Adapter\Pdo\Mysql(array(
 *  'host' => '192.168.0.11',
 *  'username' => 'sigma',
 *  'password' => 'secret',
 *  'dbname' => 'blog',
 * ));
 *
 * //Reconnect
 * $connection->connect();
 * </code>
 *
 * @param 	array descriptor
 * @return 	boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, connect) {

	HashTable *_2;
	HashPosition _1;
	zval *descriptor = NULL, *username, *password, *dsnParts, *dsnAttributes, *persistent, *options, *key = NULL, *value = NULL, *_0, **_3, *_4 = NULL, *_5, *_6, *_7;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &descriptor);

	if (!descriptor) {
		ZEPHIR_CPY_WRT(descriptor, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(descriptor);
	}


	if ((Z_TYPE_P(descriptor) == IS_NULL)) {
		ZEPHIR_OBS_NVAR(descriptor);
		zephir_read_property_this(&descriptor, this_ptr, SL("_descriptor"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(username);
	if (zephir_array_isset_string_fetch(&username, descriptor, SS("username"), 0 TSRMLS_CC)) {
		zephir_array_fetch_string(&_0, descriptor, SL("username"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_unset(&descriptor, _0, PH_SEPARATE);
	} else {
		ZEPHIR_INIT_BNVAR(username);
		ZVAL_NULL(username);
	}
	ZEPHIR_OBS_VAR(password);
	if (zephir_array_isset_string_fetch(&password, descriptor, SS("password"), 0 TSRMLS_CC)) {
		zephir_array_fetch_string(&_0, descriptor, SL("password"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_unset(&descriptor, _0, PH_SEPARATE);
	} else {
		ZEPHIR_INIT_BNVAR(password);
		ZVAL_NULL(password);
	}
	ZEPHIR_OBS_VAR(options);
	if (zephir_array_isset_string_fetch(&options, descriptor, SS("options"), 0 TSRMLS_CC)) {
		zephir_array_fetch_string(&_0, descriptor, SL("options"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_unset(&descriptor, _0, PH_SEPARATE);
	} else {
		ZEPHIR_INIT_BNVAR(options);
		array_init(options);
	}
	ZEPHIR_OBS_VAR(dsnAttributes);
	if (!(zephir_array_isset_string_fetch(&dsnAttributes, descriptor, SS("dsn"), 0 TSRMLS_CC))) {
		ZEPHIR_INIT_VAR(dsnParts);
		array_init(dsnParts);
		zephir_is_iterable(descriptor, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HMKEY(key, _2, _1);
			ZEPHIR_GET_HVALUE(value, _3);
			ZEPHIR_INIT_LNVAR(_4);
			ZEPHIR_CONCAT_VSV(_4, key, "=", value);
			zephir_array_append(&dsnParts, _4, PH_SEPARATE);
		}
		ZEPHIR_INIT_BNVAR(dsnAttributes);
		zephir_fast_join_str(dsnAttributes, SL(";"), dsnParts TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(_5);
	ZVAL_LONG(_5, 2);
	zephir_array_update_long(&options, 3, &_5, PH_COPY | PH_SEPARATE, "phalcon/db/adapter/pdo.zep", 139);
	if (zephir_array_isset_string_fetch(&persistent, descriptor, SS("persistent"), 1 TSRMLS_CC)) {
		if (zephir_is_true(persistent)) {
			zephir_array_update_long(&options, 12, &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE, "phalcon/db/adapter/pdo.zep", 146);
		}
	}
	ZEPHIR_INIT_LNVAR(_4);
	object_init_ex(_4, phalcon_db_adapter_pdo_ce);
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("_type"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_7);
	ZEPHIR_CONCAT_VSV(_7, _6, ":", dsnAttributes);
	zephir_call_method_p4_noret(_4, "__construct", _7, username, password, options);
	zephir_update_property_this(this_ptr, SL("_pdo"), _4 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns a PDO prepared statement to be executed with 'executePrepared'
 *
 *<code>
 * $statement = $db->prepare('SELECT * FROM robots WHERE name = :name');
 * $result = $connection->executePrepared($statement, array('name' => 'Voltron'));
 *</code>
 *
 * @param string sqlStatement
 * @return \PDOStatement
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, prepare) {

	zval *sqlStatement_param = NULL, *pdo;
	zval *sqlStatement = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &sqlStatement_param);

	if (Z_TYPE_P(sqlStatement_param) != IS_STRING && Z_TYPE_P(sqlStatement_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'sqlStatement' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(sqlStatement_param) == IS_STRING) {
		sqlStatement = sqlStatement_param;
	} else {
		ZEPHIR_INIT_VAR(sqlStatement);
		ZVAL_EMPTY_STRING(sqlStatement);
	}


	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, pdo, "prepare", sqlStatement);
	RETURN_MM();

}

/**
 * Executes a prepared statement binding. This function uses integer indexes starting from zero
 *
 *<code>
 * $statement = $db->prepare('SELECT * FROM robots WHERE name = :name');
 * $result = $connection->executePrepared($statement, array('name' => 'Voltron'));
 *</code>
 *
 * @param \PDOStatement statement
 * @param array placeholders
 * @param array dataTypes
 * @return \PDOStatement
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, executePrepared) {

	zend_function *_3 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *statement, *placeholders, *dataTypes, *wildcard = NULL, *value = NULL, *type = NULL, *castValue = NULL, *parameter = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &statement, &placeholders, &dataTypes);



	if ((Z_TYPE_P(placeholders) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Placeholders must be an array");
		return;
	}
	zephir_is_iterable(placeholders, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(wildcard, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		if ((Z_TYPE_P(wildcard) == IS_LONG)) {
			ZEPHIR_INIT_NVAR(parameter);
			ZVAL_LONG(parameter, (zephir_get_numberval(wildcard) + 1));
		} else {
			if ((Z_TYPE_P(wildcard) == IS_STRING)) {
				ZEPHIR_CPY_WRT(parameter, wildcard);
			} else {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Invalid bind parameter");
				return;
			}
		}
		if ((Z_TYPE_P(dataTypes) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(type);
			if (!(zephir_array_isset_fetch(&type, dataTypes, wildcard, 0 TSRMLS_CC))) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "Invalid bind type parameter");
				return;
			}
			if (ZEPHIR_IS_LONG(type, 32)) {
				ZEPHIR_INIT_NVAR(castValue);
				ZEPHIR_CALL_INTERNAL_FUNCTION(castValue, &castValue, "doubleval", &_3, 1, value);
				ZEPHIR_INIT_NVAR(type);
				ZVAL_LONG(type, 1024);
			} else {
				ZEPHIR_CPY_WRT(castValue, value);
			}
			if (ZEPHIR_IS_LONG(type, 1024)) {
				Z_SET_ISREF_P(castValue);
				zephir_call_method_p2_cache_noret(statement, "bindparam", &_4, parameter, castValue);
				Z_UNSET_ISREF_P(castValue);
			} else {
				Z_SET_ISREF_P(castValue);
				zephir_call_method_p3_cache_noret(statement, "bindparam", &_5, parameter, castValue, type);
				Z_UNSET_ISREF_P(castValue);
			}
		} else {
			Z_SET_ISREF_P(value);
			zephir_call_method_p2_cache_noret(statement, "bindparam", &_6, parameter, value);
			Z_UNSET_ISREF_P(value);
		}
	}
	zephir_call_method_noret(statement, "execute");
	RETURN_CCTOR(statement);

}

/**
 * Sends SQL statements to the database server returning the success state.
 * Use this method only when the SQL statement sent to the server is returning rows
 *
 *<code>
 *	//Querying data
 *	$resultset = $connection->query("SELECT * FROM robots WHERE type='mechanical'");
 *	$resultset = $connection->query("SELECT * FROM robots WHERE type=?", array("mechanical"));
 *</code>
 *
 * @param  string sqlStatement
 * @param  array bindParams
 * @param  array bindTypes
 * @return Phalcon\Db\ResultInterface|bool
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, query) {

	zval *sqlStatement_param = NULL, *bindParams = NULL, *bindTypes = NULL, *eventsManager = NULL, *pdo = NULL, *statement = NULL, *_0, *_1 = NULL, *_2;
	zval *sqlStatement = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &sqlStatement_param, &bindParams, &bindTypes);

	if (Z_TYPE_P(sqlStatement_param) != IS_STRING && Z_TYPE_P(sqlStatement_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'sqlStatement' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(sqlStatement_param) == IS_STRING) {
		sqlStatement = sqlStatement_param;
	} else {
		ZEPHIR_INIT_VAR(sqlStatement);
		ZVAL_EMPTY_STRING(sqlStatement);
	}
	if (!bindParams) {
		bindParams = ZEPHIR_GLOBAL(global_null);
	}
	if (!bindTypes) {
		bindTypes = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(eventsManager, _0);
	if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
		zephir_update_property_this(this_ptr, SL("_sqlStatement"), sqlStatement TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_sqlVariables"), bindParams TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_sqlBindTypes"), bindTypes TSRMLS_CC);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "db:beforeQuery", 1);
		zephir_call_method_p3(_1, eventsManager, "fire", _2, this_ptr, bindParams);
		if (ZEPHIR_IS_FALSE(_1)) {
			RETURN_MM_BOOL(0);
		}
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_pdo"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(pdo, _0);
	ZEPHIR_INIT_VAR(statement);
	if ((Z_TYPE_P(bindParams) == IS_ARRAY)) {
		zephir_call_method_p1(statement, pdo, "prepare", sqlStatement);
		if ((Z_TYPE_P(statement) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(_1);
			zephir_call_method_p3(_1, this_ptr, "executeprepared", statement, bindParams, bindTypes);
			ZEPHIR_CPY_WRT(statement, _1);
		}
	} else {
		zephir_call_method_p1(statement, pdo, "query", sqlStatement);
	}
	if ((Z_TYPE_P(statement) == IS_OBJECT)) {
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "db:afterQuery", 1);
			zephir_call_method_p3_noret(eventsManager, "fire", _1, this_ptr, bindParams);
		}
		object_init_ex(return_value, phalcon_db_result_pdo_ce);
		zephir_call_method_p5_noret(return_value, "__construct", this_ptr, statement, sqlStatement, bindParams, bindTypes);
		RETURN_MM();
	}
	RETURN_CCTOR(statement);

}

/**
 * Sends SQL statements to the database server returning the success state.
 * Use this method only when the SQL statement sent to the server doesn't return any row
 *
 *<code>
 *	//Inserting data
 *	$success = $connection->execute("INSERT INTO robots VALUES (1, 'Astro Boy')");
 *	$success = $connection->execute("INSERT INTO robots VALUES (?, ?)", array(1, 'Astro Boy'));
 *</code>
 *
 * @param  string sqlStatement
 * @param  array bindParams
 * @param  array bindTypes
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, execute) {

	zval *sqlStatement_param = NULL, *bindParams = NULL, *bindTypes = NULL, *eventsManager = NULL, *affectedRows, *pdo, *newStatement, *statement, *_0, *_1 = NULL, *_2;
	zval *sqlStatement = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &sqlStatement_param, &bindParams, &bindTypes);

	if (Z_TYPE_P(sqlStatement_param) != IS_STRING && Z_TYPE_P(sqlStatement_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'sqlStatement' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(sqlStatement_param) == IS_STRING) {
		sqlStatement = sqlStatement_param;
	} else {
		ZEPHIR_INIT_VAR(sqlStatement);
		ZVAL_EMPTY_STRING(sqlStatement);
	}
	if (!bindParams) {
		bindParams = ZEPHIR_GLOBAL(global_null);
	}
	if (!bindTypes) {
		bindTypes = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(eventsManager, _0);
	if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
		zephir_update_property_this(this_ptr, SL("_sqlStatement"), sqlStatement TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_sqlVariables"), bindParams TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_sqlBindTypes"), bindTypes TSRMLS_CC);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "db:beforeQuery", 1);
		zephir_call_method_p3(_1, eventsManager, "fire", _2, this_ptr, bindParams);
		if (ZEPHIR_IS_FALSE(_1)) {
			RETURN_MM_BOOL(0);
		}
	}
	ZEPHIR_INIT_VAR(affectedRows);
	ZVAL_LONG(affectedRows, 0);
	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(bindParams) == IS_ARRAY)) {
		ZEPHIR_INIT_VAR(statement);
		zephir_call_method_p1(statement, pdo, "prepare", sqlStatement);
		if ((Z_TYPE_P(statement) == IS_OBJECT)) {
			ZEPHIR_INIT_VAR(newStatement);
			zephir_call_method_p3(newStatement, this_ptr, "executeprepared", statement, bindParams, bindTypes);
			ZEPHIR_INIT_BNVAR(affectedRows);
			zephir_call_method(affectedRows, newStatement, "rowcount");
		}
	} else {
		ZEPHIR_INIT_BNVAR(affectedRows);
		zephir_call_method_p1(affectedRows, pdo, "exec", sqlStatement);
	}
	if ((Z_TYPE_P(affectedRows) == IS_LONG)) {
		zephir_update_property_this(this_ptr, SL("_affectedRows"), affectedRows TSRMLS_CC);
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "db:afterQuery", 1);
			zephir_call_method_p3_noret(eventsManager, "fire", _1, this_ptr, bindParams);
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Returns the number of affected rows by the lastest INSERT/UPDATE/DELETE executed in the database system
 *
 *<code>
 *	$connection->execute("DELETE FROM robots");
 *	echo $connection->affectedRows(), ' were deleted';
 *</code>
 *
 * @return int
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, affectedRows) {


	RETURN_MEMBER(this_ptr, "_affectedRows");

}

/**
 * Closes the active connection returning success. Phalcon automatically closes and destroys
 * active connections when the request ends
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, close) {

	zval *pdo;


	pdo = zephir_fetch_nproperty_this(this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) == IS_OBJECT)) {
		zephir_update_property_this(this_ptr, SL("_pdo"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
		RETURN_BOOL(1);
	}
	RETURN_BOOL(1);

}

/**
 * Escapes a column/table/schema name
 *
 *<code>
 *	$escapedTable = $connection->escapeIdentifier('robots');
 *	$escapedTable = $connection->escapeIdentifier(array('store', 'robots'));
 *</code>
 *
 * @param string identifier
 * @return string
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, escapeIdentifier) {

	zval *identifier, *_0, *_1;

	zephir_fetch_params(0, 1, 0, &identifier);



	if ((Z_TYPE_P(identifier) == IS_ARRAY)) {
		zephir_array_fetch_long(&_0, identifier, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_fetch_long(&_1, identifier, 1, PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_CONCAT_SVSVS(return_value, "\"", _0, "\".\"", _1, "\"");
		return;
	}
	ZEPHIR_CONCAT_SVS(return_value, "\"", identifier, "\"");
	return;

}

/**
 * Escapes a value to avoid SQL injections according to the active charset in the connection
 *
 *<code>
 *	$escapedStr = $connection->escapeString('some dangerous value');
 *</code>
 *
 * @param string str
 * @return string
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, escapeString) {

	zval *str_param = NULL, *_0;
	zval *str = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str_param);

	if (Z_TYPE_P(str_param) != IS_STRING && Z_TYPE_P(str_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'str' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(str_param) == IS_STRING) {
		str = str_param;
	} else {
		ZEPHIR_INIT_VAR(str);
		ZVAL_EMPTY_STRING(str);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_pdo"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "quote", str);
	RETURN_MM();

}

/**
 * Returns the insert id for the auto_increment/serial column inserted in the lastest executed SQL statement
 *
 *<code>
 * //Inserting a new robot
 * $success = $connection->insert(
 *     "robots",
 *     array("Astro Boy", 1952),
 *     array("name", "year")
 * );
 *
 * //Getting the generated id
 * $id = $connection->lastInsertId();
 *</code>
 *
 * @param string sequenceName
 * @return int|boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, lastInsertId) {

	zval *sequenceName = NULL, *pdo;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &sequenceName);

	if (!sequenceName) {
		sequenceName = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) != IS_OBJECT)) {
		RETURN_MM_BOOL(0);
	}
	zephir_call_method_p1(return_value, pdo, "lastinsertid", sequenceName);
	RETURN_MM();

}

/**
 * Starts a transaction in the connection
 *
 * @param boolean nesting
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, begin) {

	zval *nesting_param = NULL, *pdo, *transactionLevel, *eventsManager = NULL, *savepointName, *_0, *_1 = NULL, *_2;
	zend_bool nesting;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &nesting_param);

	if (!nesting_param) {
		nesting = 1;
	} else {
		nesting = zephir_get_boolval(nesting_param);
	}


	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) != IS_OBJECT)) {
		RETURN_MM_BOOL(0);
	}
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
	ZEPHIR_OBS_VAR(transactionLevel);
	zephir_read_property_this(&transactionLevel, this_ptr, SL("_transactionLevel"), PH_NOISY_CC);
	if (ZEPHIR_IS_LONG(transactionLevel, 1)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(eventsManager, _0);
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "db:beginTransaction", 1);
			zephir_call_method_p2_noret(eventsManager, "fire", _1, this_ptr);
		}
		zephir_call_method(return_value, pdo, "begintransaction");
		RETURN_MM();
	} else {
		if (zephir_is_true(transactionLevel)) {
			if (nesting) {
				ZEPHIR_INIT_NVAR(_1);
				zephir_call_method(_1, this_ptr, "isnestedtransactionswithsavepoints");
				if (zephir_is_true(_1)) {
					_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
					ZEPHIR_CPY_WRT(eventsManager, _0);
					ZEPHIR_INIT_VAR(savepointName);
					zephir_call_method(savepointName, this_ptr, "getnestedtransactionsavepointname");
					if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
						ZEPHIR_INIT_VAR(_2);
						ZVAL_STRING(_2, "db:createSavepoint", 1);
						zephir_call_method_p3_noret(eventsManager, "fire", _2, this_ptr, savepointName);
					}
					zephir_call_method_p1(return_value, this_ptr, "createsavepoint", savepointName);
					RETURN_MM();
				}
			}
		}
	}
	RETURN_MM_BOOL(0);

}

/**
 * Rollbacks the active transaction in the connection
 *
 * @param boolean nesting
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, rollback) {

	zval *nesting_param = NULL, *pdo, *transactionLevel, *eventsManager = NULL, *savepointName, *_0, *_1 = NULL, *_2;
	zend_bool nesting;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &nesting_param);

	if (!nesting_param) {
		nesting = 1;
	} else {
		nesting = zephir_get_boolval(nesting_param);
	}


	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) != IS_OBJECT)) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_OBS_VAR(transactionLevel);
	zephir_read_property_this(&transactionLevel, this_ptr, SL("_transactionLevel"), PH_NOISY_CC);
	if (!(zephir_is_true(transactionLevel))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "There is no active transaction");
		return;
	}
	if (ZEPHIR_IS_LONG(transactionLevel, 1)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(eventsManager, _0);
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "db:rollbackTransaction", 1);
			zephir_call_method_p2_noret(eventsManager, "fire", _1, this_ptr);
		}
		RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
		zephir_call_method(return_value, pdo, "rollback");
		RETURN_MM();
	} else {
		if (zephir_is_true(transactionLevel)) {
			if (nesting) {
				ZEPHIR_INIT_NVAR(_1);
				zephir_call_method(_1, this_ptr, "isnestedtransactionswithsavepoints");
				if (zephir_is_true(_1)) {
					ZEPHIR_INIT_VAR(savepointName);
					zephir_call_method(savepointName, this_ptr, "getnestedtransactionsavepointname");
					_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
					ZEPHIR_CPY_WRT(eventsManager, _0);
					if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
						ZEPHIR_INIT_VAR(_2);
						ZVAL_STRING(_2, "db:rollbackSavepoint", 1);
						zephir_call_method_p3_noret(eventsManager, "fire", _2, this_ptr, savepointName);
					}
					RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
					zephir_call_method_p1(return_value, this_ptr, "rollbacksavepoint", savepointName);
					RETURN_MM();
				}
			}
		}
	}
	if (ZEPHIR_GT_LONG(transactionLevel, 0)) {
		RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
	}
	RETURN_MM_BOOL(0);

}

/**
 * Commits the active transaction in the connection
 *
 * @param boolean nesting
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, commit) {

	zval *nesting_param = NULL, *pdo, *transactionLevel, *eventsManager = NULL, *savepointName, *_0, *_1 = NULL, *_2;
	zend_bool nesting;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &nesting_param);

	if (!nesting_param) {
		nesting = 1;
	} else {
		nesting = zephir_get_boolval(nesting_param);
	}


	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) != IS_OBJECT)) {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_OBS_VAR(transactionLevel);
	zephir_read_property_this(&transactionLevel, this_ptr, SL("_transactionLevel"), PH_NOISY_CC);
	if (!(zephir_is_true(transactionLevel))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_db_exception_ce, "There is no active transaction");
		return;
	}
	if (ZEPHIR_IS_LONG(transactionLevel, 1)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(eventsManager, _0);
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "db:commitTransaction", 1);
			zephir_call_method_p2_noret(eventsManager, "fire", _1, this_ptr);
		}
		RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
		zephir_call_method(return_value, pdo, "commit");
		RETURN_MM();
	} else {
		if (zephir_is_true(transactionLevel)) {
			if (nesting) {
				ZEPHIR_INIT_NVAR(_1);
				zephir_call_method(_1, this_ptr, "isnestedtransactionswithsavepoints");
				if (zephir_is_true(_1)) {
					_0 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
					ZEPHIR_CPY_WRT(eventsManager, _0);
					ZEPHIR_INIT_VAR(savepointName);
					zephir_call_method(savepointName, this_ptr, "getnestedtransactionsavepointname");
					if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
						ZEPHIR_INIT_VAR(_2);
						ZVAL_STRING(_2, "db:releaseSavepoint", 1);
						zephir_call_method_p3_noret(eventsManager, "fire", _2, this_ptr, savepointName);
					}
					RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
					zephir_call_method_p1(return_value, this_ptr, "releasesavepoint", savepointName);
					RETURN_MM();
				}
			}
		}
	}
	if (ZEPHIR_GT_LONG(transactionLevel, 0)) {
		RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("_transactionLevel") TSRMLS_CC));
	}
	RETURN_MM_BOOL(0);

}

/**
 * Returns the current transaction nesting level
 *
 * @return int
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, getTransactionLevel) {


	RETURN_MEMBER(this_ptr, "_transactionLevel");

}

/**
 * Checks whether the connection is under a transaction
 *
 *<code>
 *	$connection->begin();
 *	var_dump($connection->isUnderTransaction()); //true
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, isUnderTransaction) {

	zval *pdo;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(pdo);
	zephir_read_property_this(&pdo, this_ptr, SL("_pdo"), PH_NOISY_CC);
	if ((Z_TYPE_P(pdo) == IS_OBJECT)) {
		zephir_call_method(return_value, pdo, "intransaction");
		RETURN_MM();
	}
	RETURN_MM_BOOL(0);

}

/**
 * Return internal PDO handler
 *
 * @return \Pdo
 */
PHP_METHOD(Phalcon_Db_Adapter_Pdo, getInternalHandler) {


	RETURN_MEMBER(this_ptr, "_pdo");

}

