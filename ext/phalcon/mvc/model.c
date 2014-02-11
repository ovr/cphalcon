
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/string.h"


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
 * Phalcon\Mvc\Model
 *
 * <p>Phalcon\Mvc\Model connects business objects and database tables to create
 * a persistable domain model where logic and data are presented in one wrapping.
 * It‘s an implementation of the object-relational mapping (ORM).</p>
 *
 * <p>A model represents the information (data) of the application and the rules to manipulate that data.
 * Models are primarily used for managing the rules of interaction with a corresponding database table.
 * In most cases, each table in your database will correspond to one model in your application.
 * The bulk of your application’s business logic will be concentrated in the models.</p>
 *
 * <p>Phalcon\Mvc\Model is the first ORM written in C-language for PHP, giving to developers high performance
 * when interacting with databases while is also easy to use.</p>
 *
 * <code>
 *
 * $robot = new Robots();
 * $robot->type = 'mechanical';
 * $robot->name = 'Astro Boy';
 * $robot->year = 1952;
 * if ($robot->save() == false) {
 *  echo "Umh, We can store robots: ";
 *  foreach ($robot->getMessages() as $message) {
 *    echo message;
 *  }
 * } else {
 *  echo "Great, a new robot was saved successfully!";
 * }
 * </code>
 *
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_Model) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Mvc, Model, phalcon, mvc_model, phalcon_mvc_model_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(phalcon_mvc_model_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_modelsManager"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_modelsMetaData"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_errorMessages"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_mvc_model_ce, SL("_operationMade"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_mvc_model_ce, SL("_dirtyState"), 1, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_transaction"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_uniqueKey"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_uniqueParams"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_uniqueTypes"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_skipped"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_related"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_ce, SL("_snapshot"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("OP_NONE"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("OP_CREATE"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("OP_UPDATE"), 2 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("OP_DELETE"), 3 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("DIRTY_STATE_PERSISTENT"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("DIRTY_STATE_TRANSIENT"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_ce, SL("DIRTY_STATE_DETACHED"), 2 TSRMLS_CC);

	zend_class_implements(phalcon_mvc_model_ce TSRMLS_CC, 1, phalcon_mvc_modelinterface_ce);
	zend_class_implements(phalcon_mvc_model_ce TSRMLS_CC, 1, phalcon_mvc_model_resultinterface_ce);
	zend_class_implements(phalcon_mvc_model_ce TSRMLS_CC, 1, phalcon_di_injectionawareinterface_ce);
	zend_class_implements(phalcon_mvc_model_ce TSRMLS_CC, 1, zend_ce_serializable);

	return SUCCESS;

}

/**
 * Phalcon\Mvc\Model constructor
 *
 * @param Phalcon\DiInterface dependencyInjector
 * @param Phalcon\Mvc\Model\ManagerInterface modelsManager
 */
PHP_METHOD(Phalcon_Mvc_Model, __construct) {

	zval *dependencyInjector = NULL, *modelsManager = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &dependencyInjector, &modelsManager);

	if (!dependencyInjector) {
		ZEPHIR_CPY_WRT(dependencyInjector, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(dependencyInjector);
	}
	if (!modelsManager) {
		ZEPHIR_CPY_WRT(modelsManager, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(modelsManager);
	}


	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(dependencyInjector);
		zephir_call_static(dependencyInjector, "Phalcon\\Di", "getdefault");
	}
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
		return;
	}
	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);
	if ((Z_TYPE_P(modelsManager) != IS_OBJECT)) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "modelsManager", 1);
		zephir_call_method_p1(_0, dependencyInjector, "getshared", _1);
		ZEPHIR_CPY_WRT(modelsManager, _0);
		if ((Z_TYPE_P(modelsManager) != IS_OBJECT)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The injected service 'modelsManager' is not valid");
			return;
		}
	}
	zephir_update_property_this(this_ptr, SL("_modelsManager"), modelsManager TSRMLS_CC);
	zephir_call_method_p1_noret(modelsManager, "initialize", this_ptr);
	if ((zephir_method_exists_ex(this_ptr, SS("onconstruct") TSRMLS_CC) == SUCCESS)) {
		zephir_call_method_noret(this_ptr, "onconstruct");
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the dependency injection container
 *
 * @param Phalcon\DiInterface dependencyInjector
 */
PHP_METHOD(Phalcon_Mvc_Model, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the dependency injection container
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Sets a custom events manager
 *
 * @param Phalcon\Events\ManagerInterface eventsManager
 */
PHP_METHOD(Phalcon_Mvc_Model, setEventsManager) {

	zval *eventsManager, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &eventsManager);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(_0, "setcustomeventsmanager", this_ptr, eventsManager);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the custom events manager
 *
 * @return Phalcon\Events\ManagerInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getEventsManager) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getcustomeventsmanager", this_ptr);
	RETURN_MM();

}

/**
 * Returns the models meta-data service related to the entity instance
 *
 * @return Phalcon\Mvc\Model\MetaDataInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getModelsMetaData) {

	zval *metaData = NULL, *dependencyInjector = NULL, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(metaData);
	zephir_read_property_this(&metaData, this_ptr, SL("_modelsMetaData"), PH_NOISY_CC);
	if ((Z_TYPE_P(metaData) != IS_OBJECT)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(dependencyInjector, _0);
		if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
			return;
		}
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "modelsMetadata", 1);
		zephir_call_method_p1(_1, dependencyInjector, "getshared", _2);
		ZEPHIR_CPY_WRT(metaData, _1);
		if ((Z_TYPE_P(metaData) != IS_OBJECT)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The injected service 'modelsMetadata' is not valid");
			return;
		}
		zephir_update_property_this(this_ptr, SL("_modelsMetaData"), metaData TSRMLS_CC);
	}
	RETURN_CCTOR(metaData);

}

/**
 * Returns the models manager related to the entity instance
 *
 * @return Phalcon\Mvc\Model\ManagerInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getModelsManager) {


	RETURN_MEMBER(this_ptr, "_modelsManager");

}

/**
 * Sets a transaction related to the Model instance
 *
 *<code>
 *use Phalcon\Mvc\Model\Transaction\Manager as TxManager;
 *use Phalcon\Mvc\Model\Transaction\Failed as TxFailed;
 *
 *try {
 *
 *  $txManager = new TxManager();
 *
 *  $transaction = $txManager->get();
 *
 *  $robot = new Robots();
 *  $robot->setTransaction($transaction);
 *  $robot->name = 'WALL·E';
 *  $robot->created_at = date('Y-m-d');
 *  if ($robot->save() == false) {
 *    $transaction->rollback("Can't save robot");
 *  }
 *
 *  $robotPart = new RobotParts();
 *  $robotPart->setTransaction($transaction);
 *  $robotPart->type = 'head';
 *  if ($robotPart->save() == false) {
 *    $transaction->rollback("Robot part cannot be saved");
 *  }
 *
 *  $transaction->commit();
 *
 *} catch (TxFailed $e) {
 *  echo 'Failed, reason: ', $e->getMessage();
 *}
 *
 *</code>
 *
 * @param Phalcon\Mvc\Model\TransactionInterface $transaction
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setTransaction) {

	zval *transaction;

	zephir_fetch_params(0, 1, 0, &transaction);



	if ((Z_TYPE_P(transaction) == IS_OBJECT)) {
		zephir_update_property_this(this_ptr, SL("_transaction"), transaction TSRMLS_CC);
		RETURN_THISW();
	}
	ZEPHIR_THROW_EXCEPTION_STRW(phalcon_mvc_model_exception_ce, "Transaction should be an object");
	return;

}

/**
 * Sets table name which model should be mapped
 *
 * @param string source
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setSource) {

	zval *source_param = NULL, *_0;
	zval *source = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &source_param);

	if (Z_TYPE_P(source_param) != IS_STRING && Z_TYPE_P(source_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'source' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(source_param) == IS_STRING) {
		source = source_param;
	} else {
		ZEPHIR_INIT_VAR(source);
		ZVAL_EMPTY_STRING(source);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(_0, "setmodelsource", this_ptr, source);
	RETURN_THIS();

}

/**
 * Returns table name mapped in the model
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model, getSource) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getmodelsource", this_ptr);
	RETURN_MM();

}

/**
 * Sets schema name where table mapped is located
 *
 * @param string schema
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setSchema) {

	zval *schema_param = NULL, *_0;
	zval *schema = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &schema_param);

	if (Z_TYPE_P(schema_param) != IS_STRING && Z_TYPE_P(schema_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'schema' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(schema_param) == IS_STRING) {
		schema = schema_param;
	} else {
		ZEPHIR_INIT_VAR(schema);
		ZVAL_EMPTY_STRING(schema);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2(return_value, _0, "setmodelschema", this_ptr, schema);
	RETURN_MM();

}

/**
 * Returns schema name where table mapped is located
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model, getSchema) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getmodelschema", this_ptr);
	RETURN_MM();

}

/**
 * Sets the DependencyInjection connection service name
 *
 * @param string connectionService
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setConnectionService) {

	zval *connectionService_param = NULL, *_0;
	zval *connectionService = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &connectionService_param);

	if (Z_TYPE_P(connectionService_param) != IS_STRING && Z_TYPE_P(connectionService_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'connectionService' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(connectionService_param) == IS_STRING) {
		connectionService = connectionService_param;
	} else {
		ZEPHIR_INIT_VAR(connectionService);
		ZVAL_EMPTY_STRING(connectionService);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(_0, "setconnectionservice", this_ptr, connectionService);
	RETURN_THIS();

}

/**
 * Sets the DependencyInjection connection service name used to read data
 *
 * @param string connectionService
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setReadConnectionService) {

	zval *connectionService_param = NULL, *_0;
	zval *connectionService = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &connectionService_param);

	if (Z_TYPE_P(connectionService_param) != IS_STRING && Z_TYPE_P(connectionService_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'connectionService' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(connectionService_param) == IS_STRING) {
		connectionService = connectionService_param;
	} else {
		ZEPHIR_INIT_VAR(connectionService);
		ZVAL_EMPTY_STRING(connectionService);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(_0, "setreadconnectionservice", this_ptr, connectionService);
	RETURN_THIS();

}

/**
 * Sets the DependencyInjection connection service name used to write data
 *
 * @param string connectionService
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setWriteConnectionService) {

	zval *connectionService_param = NULL, *_0;
	zval *connectionService = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &connectionService_param);

	if (Z_TYPE_P(connectionService_param) != IS_STRING && Z_TYPE_P(connectionService_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'connectionService' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(connectionService_param) == IS_STRING) {
		connectionService = connectionService_param;
	} else {
		ZEPHIR_INIT_VAR(connectionService);
		ZVAL_EMPTY_STRING(connectionService);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2(return_value, _0, "setwriteconnectionservice", this_ptr, connectionService);
	RETURN_MM();

}

/**
 * Returns the DependencyInjection connection service name used to read data related the model
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model, getReadConnectionService) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getreadconnectionservice", this_ptr);
	RETURN_MM();

}

/**
 * Returns the DependencyInjection connection service name used to write data related to the model
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model, getWriteConnectionService) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getwriteconnectionservice", this_ptr);
	RETURN_MM();

}

/**
 * Sets the dirty state of the object using one of the DIRTY_STATE_* constants
 *
 * @param int dirtyState
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, setDirtyState) {

	zval *dirtyState_param = NULL, *_0;
	int dirtyState;

	zephir_fetch_params(0, 1, 0, &dirtyState_param);

		dirtyState = zephir_get_intval(dirtyState_param);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, dirtyState);
	zephir_update_property_zval(this_ptr, SL("_dirtyState"), _0 TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns one of the DIRTY_STATE_* constants telling if the record exists in the database or not
 *
 * @return int
 */
PHP_METHOD(Phalcon_Mvc_Model, getDirtyState) {


	RETURN_MEMBER(this_ptr, "_dirtyState");

}

/**
 * Gets the connection used to read data for the model
 *
 * @return Phalcon\Db\AdapterInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getReadConnection) {

	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getreadconnection", this_ptr);
	RETURN_MM();

}

/**
 * Gets the connection used to write data to the model
 *
 * @return Phalcon\Db\AdapterInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getWriteConnection) {

	zval *transaction = NULL, *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_transaction"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(transaction, _0);
	if ((Z_TYPE_P(transaction) == IS_OBJECT)) {
		zephir_call_method(return_value, transaction, "getconnection");
		RETURN_MM();
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p1(return_value, _0, "getwriteconnection", this_ptr);
	RETURN_MM();

}

/**
 * Assigns values to a model from an array
 *
 *<code>
 *$robot->assign(array(
 *  'type' => 'mechanical',
 *  'name' => 'Astro Boy',
 *  'year' => 1952
 *));
 *</code>
 *
 * @param array data
 * @param array columnMap
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, assign) {

	zend_function *_5 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *data, *columnMap = NULL, *key = NULL, *value = NULL, *attribute = NULL, **_2, *_3 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &data, &columnMap);

	if (!columnMap) {
		columnMap = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data to dump in the object must be an Array");
		return;
	}
	zephir_is_iterable(data, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(attribute);
			if (zephir_array_isset_fetch(&attribute, columnMap, key, 0 TSRMLS_CC)) {
				zephir_update_property_zval_zval(this_ptr, attribute, value TSRMLS_CC);
			} else {
				ZEPHIR_INIT_LNVAR(_3);
				object_init_ex(_3, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_4);
				ZEPHIR_CONCAT_SVS(_4, "Column '", key, "' doesn\'t make part of the column map");
				zephir_call_method_p1_cache_noret(_3, "__construct", &_5, _4);
				zephir_throw_exception(_3 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			zephir_update_property_zval_zval(this_ptr, key, value TSRMLS_CC);
		}
	}
	RETURN_THIS();

}

/**
 * Assigns values to a model from an array returning a new model.
 *
 *<code>
 *$robot = \Phalcon\Mvc\Model::cloneResultMap(new Robots(), array(
 *  'type' => 'mechanical',
 *  'name' => 'Astro Boy',
 *  'year' => 1952
 *));
 *</code>
 *
 * @param Phalcon\Mvc\Model base
 * @param array data
 * @param array columnMap
 * @param int dirtyState
 * @param boolean keepSnapshots
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, cloneResultMap) {

	zend_function *_6 = NULL;
	HashTable *_2;
	HashPosition _1;
	zend_bool keepSnapshots;
	int dirtyState;
	zval *base, *data, *columnMap, *dirtyState_param = NULL, *keepSnapshots_param = NULL, *object, *attribute = NULL, *key = NULL, *value = NULL, *_0, **_3, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 2, &base, &data, &columnMap, &dirtyState_param, &keepSnapshots_param);

	if (!dirtyState_param) {
		dirtyState = 0;
	} else {
		dirtyState = zephir_get_intval(dirtyState_param);
	}
	if (!keepSnapshots_param) {
		keepSnapshots = 0;
	} else {
		keepSnapshots = zephir_get_boolval(keepSnapshots_param);
	}


	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data to dump in the object must be an Array");
		return;
	}
	ZEPHIR_INIT_VAR(object);
	if (zephir_clone(object, base TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, dirtyState);
	zephir_call_method_p1_noret(object, "setdirtystate", _0);
	zephir_is_iterable(data, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(key, _2, _1);
		ZEPHIR_GET_HVALUE(value, _3);
		if ((Z_TYPE_P(key) == IS_STRING)) {
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attribute);
				if (zephir_array_isset_fetch(&attribute, columnMap, key, 0 TSRMLS_CC)) {
					zephir_update_property_zval_zval(object, attribute, value TSRMLS_CC);
				} else {
					ZEPHIR_INIT_LNVAR(_4);
					object_init_ex(_4, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_5);
					ZEPHIR_CONCAT_SVS(_5, "Column '", key, "' doesn't make part of the column map");
					zephir_call_method_p1_cache_noret(_4, "__construct", &_6, _5);
					zephir_throw_exception(_4 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				zephir_update_property_zval_zval(object, key, value TSRMLS_CC);
			}
		}
	}
	if (keepSnapshots) {
		zephir_call_method_p2_noret(object, "setsnapshotdata", data, columnMap);
	}
	if ((zephir_method_exists_ex(object, SS("afterfetch") TSRMLS_CC) == SUCCESS)) {
		zephir_call_method_noret(object, "afterfetch");
	}
	RETURN_CCTOR(object);

}

/**
 * Returns an hydrated result based on the data and the column map
 *
 * @param array data
 * @param array columnMap
 * @param int hydrationMode
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, cloneResultMapHydrate) {

	zend_function *_5 = NULL;
	HashTable *_1;
	HashPosition _0;
	int hydrationMode;
	zval *data, *columnMap, *hydrationMode_param = NULL, *hydrate, *key = NULL, *value = NULL, *attribute = NULL, **_2, *_3 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &data, &columnMap, &hydrationMode_param);

		hydrationMode = zephir_get_intval(hydrationMode_param);


	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data to hidrate must be an Array");
		return;
	}
	if ((Z_TYPE_P(columnMap) != IS_ARRAY)) {
		if ((hydrationMode == 1)) {
			RETURN_CCTOR(data);
		}
	}
	ZEPHIR_INIT_VAR(hydrate);
	if ((hydrationMode == 1)) {
		array_init(hydrate);
	} else {
		object_init(hydrate);
	}
	zephir_is_iterable(data, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		if ((Z_TYPE_P(key) == IS_STRING)) {
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attribute);
				if (!(zephir_array_isset_fetch(&attribute, columnMap, key, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_3);
					object_init_ex(_3, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_4);
					ZEPHIR_CONCAT_SVS(_4, "Column '", key, "' doesn't make part of the column map");
					zephir_call_method_p1_cache_noret(_3, "__construct", &_5, _4);
					zephir_throw_exception(_3 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
				if ((hydrationMode == 1)) {
					zephir_array_update_zval(&hydrate, attribute, &value, PH_COPY | PH_SEPARATE);
				} else {
					zephir_update_property_zval_zval(hydrate, attribute, value TSRMLS_CC);
				}
			} else {
				if ((hydrationMode == 1)) {
					zephir_array_update_zval(&hydrate, key, &value, PH_COPY | PH_SEPARATE);
				} else {
					zephir_update_property_zval_zval(hydrate, key, value TSRMLS_CC);
				}
			}
		}
	}
	RETURN_CCTOR(hydrate);

}

/**
 * Assigns values to a model from an array returning a new model
 *
 *<code>
 *$robot = Phalcon\Mvc\Model::cloneResult(new Robots(), array(
 *  'type' => 'mechanical',
 *  'name' => 'Astro Boy',
 *  'year' => 1952
 *));
 *</code>
 *
 * @param Phalcon\Mvc\Model $base
 * @param array data
 * @param int dirtyState
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, cloneResult) {

	HashTable *_2;
	HashPosition _1;
	int dirtyState;
	zval *base, *data, *dirtyState_param = NULL, *object, *key = NULL, *value = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &base, &data, &dirtyState_param);

	if (!dirtyState_param) {
		dirtyState = 0;
	} else {
		dirtyState = zephir_get_intval(dirtyState_param);
	}


	if ((Z_TYPE_P(data) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data to dump in the object must be an Array");
		return;
	}
	ZEPHIR_INIT_VAR(object);
	if (zephir_clone(object, base TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, dirtyState);
	zephir_call_method_p1_noret(object, "setdirtystate", _0);
	zephir_is_iterable(data, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(key, _2, _1);
		ZEPHIR_GET_HVALUE(value, _3);
		if ((Z_TYPE_P(key) != IS_STRING)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Invalid key in array data provided to dumpResult()");
			return;
		}
		zephir_update_property_zval_zval(object, key, value TSRMLS_CC);
	}
	if ((zephir_method_exists_ex(object, SS("afterfetch") TSRMLS_CC) == SUCCESS)) {
		zephir_call_method_noret(object, "afterfetch");
	}
	RETURN_CCTOR(object);

}

/**
 * Allows to query a set of records that match the specified conditions
 *
 * <code>
 *
 * //How many robots are there?
 * $robots = Robots::find();
 * echo "There are ", count($robots), "\n";
 *
 * //How many mechanical robots are there?
 * $robots = Robots::find("type='mechanical'");
 * echo "There are ", count($robots), "\n";
 *
 * //Get and print virtual robots ordered by name
 * $robots = Robots::find(array("type='virtual'", "order" => "name"));
 * foreach ($robots as $robot) {
 *	   echo $robot->name, "\n";
 * }
 *
 * //Get first 100 virtual robots ordered by name
 * $robots = Robots::find(array("type='virtual'", "order" => "name", "limit" => 100));
 * foreach ($robots as $robot) {
 *	   echo $robot->name, "\n";
 * }
 * </code>
 *
 * @param 	array parameters
 * @return  Phalcon\Mvc\Model\ResultsetInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, find) {

	zval *parameters = NULL, *params = NULL, *builder, *query, *bindParams = NULL, *bindTypes = NULL, *cache, *resultset, *hydration, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(parameters) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(params);
		array_init(params);
		if ((Z_TYPE_P(parameters) != IS_NULL)) {
			zephir_array_append(&params, parameters, PH_SEPARATE);
		}
	} else {
		ZEPHIR_CPY_WRT(params, parameters);
	}
	ZEPHIR_INIT_VAR(builder);
	object_init_ex(builder, phalcon_mvc_model_query_builder_ce);
	zephir_call_method_p1_noret(builder, "__construct", params);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "get_called_class");
	zephir_call_method_p1_noret(builder, "from", _0);
	ZEPHIR_INIT_VAR(query);
	zephir_call_method(query, builder, "getquery");
	ZEPHIR_INIT_VAR(bindParams);
	ZVAL_NULL(bindParams);
	ZEPHIR_INIT_VAR(bindTypes);
	ZVAL_NULL(bindTypes);
	ZEPHIR_OBS_NVAR(bindParams);
	if (zephir_array_isset_string_fetch(&bindParams, params, SS("bind"), 0 TSRMLS_CC)) {
		ZEPHIR_OBS_NVAR(bindTypes);
	}
	ZEPHIR_OBS_VAR(cache);
	if (zephir_array_isset_string_fetch(&cache, params, SS("cache"), 0 TSRMLS_CC)) {
		zephir_call_method_p1_noret(query, "cache", cache);
	}
	ZEPHIR_INIT_VAR(resultset);
	zephir_call_method_p2(resultset, query, "execute", bindParams, bindTypes);
	if ((Z_TYPE_P(resultset) == IS_OBJECT)) {
		ZEPHIR_OBS_VAR(hydration);
		if (zephir_array_isset_string_fetch(&hydration, params, SS("hydration"), 0 TSRMLS_CC)) {
			zephir_call_method_p1_noret(resultset, "sethydratemode", hydration);
		}
	}
	RETURN_CCTOR(resultset);

}

/**
 * Allows to query the first record that match the specified conditions
 *
 * <code>
 *
 * //What's the first robot in robots table?
 * $robot = Robots::findFirst();
 * echo "The robot name is ", $robot->name;
 *
 * //What's the first mechanical robot in robots table?
 * $robot = Robots::findFirst("type='mechanical'");
 * echo "The first mechanical robot name is ", $robot->name;
 *
 * //Get first virtual robot ordered by name
 * $robot = Robots::findFirst(array("type='virtual'", "order" => "name"));
 * echo "The first virtual robot name is ", $robot->name;
 *
 * </code>
 *
 * @param array parameters
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, findFirst) {

	zval *parameters = NULL, *params = NULL, *builder, *query, *bindParams = NULL, *bindTypes = NULL, *cache, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(parameters) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(params);
		array_init(params);
		if ((Z_TYPE_P(parameters) != IS_NULL)) {
			zephir_array_append(&params, parameters, PH_SEPARATE);
		}
	} else {
		ZEPHIR_CPY_WRT(params, parameters);
	}
	ZEPHIR_INIT_VAR(builder);
	object_init_ex(builder, phalcon_mvc_model_query_builder_ce);
	zephir_call_method_p1_noret(builder, "__construct", params);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "get_called_class");
	zephir_call_method_p1_noret(builder, "from", _0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_call_method_p1_noret(builder, "limit", _1);
	ZEPHIR_INIT_VAR(query);
	zephir_call_method(query, builder, "getquery");
	ZEPHIR_INIT_VAR(bindParams);
	ZVAL_NULL(bindParams);
	ZEPHIR_INIT_VAR(bindTypes);
	ZVAL_NULL(bindTypes);
	ZEPHIR_OBS_NVAR(bindParams);
	if (zephir_array_isset_string_fetch(&bindParams, params, SS("bind"), 0 TSRMLS_CC)) {
		ZEPHIR_OBS_NVAR(bindTypes);
	}
	ZEPHIR_OBS_VAR(cache);
	if (zephir_array_isset_string_fetch(&cache, params, SS("cache"), 0 TSRMLS_CC)) {
		zephir_call_method_p1_noret(query, "cache", cache);
	}
	zephir_call_method_p1_noret(query, "setuniquerow", ZEPHIR_GLOBAL(global_true));
	zephir_call_method_p2(return_value, query, "execute", bindParams, bindTypes);
	RETURN_MM();

}

/**
 * Create a criteria for a specific model
 *
 * @param Phalcon\DiInterface dependencyInjector
 * @return Phalcon\Mvc\Model\Criteria
 */
PHP_METHOD(Phalcon_Mvc_Model, query) {

	zval *dependencyInjector = NULL, *criteria, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &dependencyInjector);

	if (!dependencyInjector) {
		ZEPHIR_CPY_WRT(dependencyInjector, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(dependencyInjector);
	}


	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(dependencyInjector);
		zephir_call_static(dependencyInjector, "Phalcon\\Di", "getdefault");
	}
	ZEPHIR_INIT_VAR(criteria);
	object_init_ex(criteria, phalcon_mvc_model_criteria_ce);
	if (zephir_has_constructor(criteria TSRMLS_CC)) {
		zephir_call_method_noret(criteria, "__construct");
	}
	zephir_call_method_p1_noret(criteria, "setdi", dependencyInjector);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "get_called_class");
	zephir_call_method_p1_noret(criteria, "setmodelname", _0);
	RETURN_CCTOR(criteria);

}

/**
 * Checks if the current record already exists or not
 *
 * @param Phalcon\Mvc\Model\MetadataInterface metaData
 * @param Phalcon\Db\AdapterInterface connection
 * @param string|array table
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _exists) {

	zend_function *_5 = NULL, *_6 = NULL, *_8 = NULL;
	HashTable *_1;
	HashPosition _0;
	int numberEmpty, numberPrimary;
	zval *metaData, *connection, *table = NULL, *uniqueParams = NULL, *uniqueTypes = NULL, *uniqueKey = NULL, *columnMap, *primaryKeys, *wherePk, *field = NULL, *attributeField = NULL, *value = NULL, *bindDataTypes, *joinWhere, *num, *type = NULL, *schema, *source, **_2, *_3 = NULL, *_4 = NULL, *_7 = NULL, *_9, *_10, *_11, *_12;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &metaData, &connection, &table);

	if (!table) {
		ZEPHIR_CPY_WRT(table, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(table);
	}


	ZEPHIR_INIT_VAR(uniqueParams);
	ZVAL_NULL(uniqueParams);
	ZEPHIR_INIT_VAR(uniqueTypes);
	ZVAL_NULL(uniqueTypes);
	ZEPHIR_OBS_VAR(uniqueKey);
	zephir_read_property_this(&uniqueKey, this_ptr, SL("_uniqueKey"), PH_NOISY_CC);
	if ((Z_TYPE_P(uniqueKey) == IS_NULL)) {
		ZEPHIR_INIT_VAR(primaryKeys);
		zephir_call_method_p1(primaryKeys, metaData, "getprimarykeyattributes", this_ptr);
		ZEPHIR_INIT_VAR(bindDataTypes);
		zephir_call_method_p1(bindDataTypes, metaData, "getbindtypes", this_ptr);
		numberPrimary = zephir_fast_count_int(primaryKeys TSRMLS_CC);
		if (!(numberPrimary)) {
			RETURN_MM_BOOL(0);
		}
		ZEPHIR_INIT_VAR(columnMap);
		if (ZEPHIR_GLOBAL(orm).column_renaming) {
			zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
		} else {
			ZVAL_NULL(columnMap);
		}
		numberEmpty = 0;
		ZEPHIR_INIT_VAR(wherePk);
		array_init(wherePk);
		ZEPHIR_INIT_BNVAR(uniqueParams);
		array_init(uniqueParams);
		ZEPHIR_INIT_BNVAR(uniqueTypes);
		array_init(uniqueTypes);
		zephir_is_iterable(primaryKeys, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(field, _2);
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, field, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_3);
					object_init_ex(_3, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_4);
					ZEPHIR_CONCAT_SVS(_4, "Column '", field, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_3, "__construct", &_5, _4);
					zephir_throw_exception(_3 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, field);
			}
			ZEPHIR_INIT_NVAR(value);
			ZVAL_NULL(value);
			if (1) {
				if (ZEPHIR_IS_EMPTY(value)) {
					numberEmpty++;
				}
				zephir_array_append(&uniqueParams, value, PH_SEPARATE);
			} else {
				zephir_array_append(&uniqueParams, ZEPHIR_GLOBAL(global_null), PH_SEPARATE);
				numberEmpty++;
			}
			ZEPHIR_OBS_NVAR(type);
			if (!(zephir_array_isset_fetch(&type, bindDataTypes, field, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_3);
				object_init_ex(_3, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_4);
				ZEPHIR_CONCAT_SVS(_4, "Column '", field, "' isn't part of the table columns");
				zephir_call_method_p1_cache_noret(_3, "__construct", &_6, _4);
				zephir_throw_exception(_3 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_array_append(&uniqueTypes, type, PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_7);
			zephir_call_method_p1_cache(_7, connection, "escapeidentifier", &_8, field);
			ZEPHIR_INIT_LNVAR(_3);
			ZEPHIR_CONCAT_VS(_3, _7, " = ?");
			zephir_array_append(&wherePk, _3, PH_SEPARATE);
		}
		if ((numberPrimary == numberEmpty)) {
			RETURN_MM_BOOL(0);
		}
		ZEPHIR_INIT_VAR(joinWhere);
		zephir_fast_join_str(joinWhere, SL(" AND "), wherePk TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_uniqueKey"), joinWhere TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_uniqueParams"), uniqueParams TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_uniqueTypes"), uniqueTypes TSRMLS_CC);
		ZEPHIR_CPY_WRT(uniqueKey, joinWhere);
	}
	_9 = zephir_fetch_nproperty_this(this_ptr, SL("_dirtyState"), PH_NOISY_CC);
	if (!(zephir_is_true(_9))) {
		RETURN_MM_BOOL(1);
	}
	if ((Z_TYPE_P(uniqueKey) == IS_NULL)) {
		ZEPHIR_OBS_NVAR(uniqueKey);
		zephir_read_property_this(&uniqueKey, this_ptr, SL("_uniqueKey"), PH_NOISY_CC);
	}
	if ((Z_TYPE_P(uniqueParams) == IS_NULL)) {
		ZEPHIR_OBS_NVAR(uniqueParams);
		zephir_read_property_this(&uniqueParams, this_ptr, SL("_uniqueParams"), PH_NOISY_CC);
	}
	if ((Z_TYPE_P(uniqueTypes) == IS_NULL)) {
		ZEPHIR_OBS_NVAR(uniqueTypes);
		zephir_read_property_this(&uniqueTypes, this_ptr, SL("_uniqueTypes"), PH_NOISY_CC);
	}
	ZEPHIR_INIT_VAR(schema);
	zephir_call_method(schema, this_ptr, "getschema");
	ZEPHIR_INIT_VAR(source);
	zephir_call_method(source, this_ptr, "getsource");
	if (zephir_is_true(schema)) {
		ZEPHIR_INIT_NVAR(table);
		array_init_size(table, 3);
		zephir_array_fast_append(table, schema);
		zephir_array_fast_append(table, source);
	} else {
		ZEPHIR_CPY_WRT(table, source);
	}
	ZEPHIR_INIT_NVAR(_7);
	zephir_call_method_p1(_7, connection, "escapeidentifier", table);
	ZEPHIR_INIT_VAR(_10);
	ZEPHIR_CONCAT_SVSV(_10, "SELECT COUNT(*) \"rowcount\" FROM ", _7, " WHERE ", uniqueKey);
	ZEPHIR_INIT_VAR(num);
	zephir_call_method_p4(num, connection, "fetchone", _10, ZEPHIR_GLOBAL(global_null), uniqueParams, uniqueTypes);
	zephir_array_fetch_string(&_11, num, SL("rowcount"), PH_NOISY | PH_READONLY TSRMLS_CC);
	if (zephir_is_true(_11)) {
		ZEPHIR_INIT_ZVAL_NREF(_12);
		ZVAL_LONG(_12, 0);
		zephir_update_property_this(this_ptr, SL("_dirtyState"), _12 TSRMLS_CC);
		RETURN_MM_BOOL(1);
	} else {
		ZEPHIR_INIT_ZVAL_NREF(_12);
		ZVAL_LONG(_12, 1);
		zephir_update_property_this(this_ptr, SL("_dirtyState"), _12 TSRMLS_CC);
	}
	RETURN_MM_BOOL(0);

}

/**
 * Generate a PHQL SELECT statement for an aggregate
 *
 * @param string function
 * @param string alias
 * @param array parameters
 * @return Phalcon\Mvc\Model\ResultsetInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, _groupResult) {

	zval *functionName_param = NULL, *alias_param = NULL, *parameters, *params = NULL, *distinctColumn, *groupColumn, *columns = NULL, *bindParams = NULL, *bindTypes = NULL, *resultset, *cache, *firstRow, *groupColumns, *builder, *query, *_0, *_1;
	zval *functionName = NULL, *alias = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &functionName_param, &alias_param, &parameters);

	if (Z_TYPE_P(functionName_param) != IS_STRING && Z_TYPE_P(functionName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'functionName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(functionName_param) == IS_STRING) {
		functionName = functionName_param;
	} else {
		ZEPHIR_INIT_VAR(functionName);
		ZVAL_EMPTY_STRING(functionName);
	}
	if (Z_TYPE_P(alias_param) != IS_STRING && Z_TYPE_P(alias_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'alias' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(alias_param) == IS_STRING) {
		alias = alias_param;
	} else {
		ZEPHIR_INIT_VAR(alias);
		ZVAL_EMPTY_STRING(alias);
	}


	if ((Z_TYPE_P(parameters) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(params);
		array_init(params);
		if ((Z_TYPE_P(parameters) != IS_NULL)) {
			zephir_array_append(&params, parameters, PH_SEPARATE);
		}
	} else {
		ZEPHIR_CPY_WRT(params, parameters);
	}
	ZEPHIR_OBS_VAR(groupColumn);
	if (!(zephir_array_isset_string_fetch(&groupColumn, params, SS("column"), 0 TSRMLS_CC))) {
		ZEPHIR_INIT_BNVAR(groupColumn);
		ZVAL_LONG(groupColumn, '*');
	}
	ZEPHIR_OBS_VAR(distinctColumn);
	if (zephir_array_isset_string_fetch(&distinctColumn, params, SS("distinct"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_VAR(columns);
		ZEPHIR_CONCAT_VSVSV(columns, functionName, "(DISTINCT ", distinctColumn, ") AS ", alias);
	} else {
		ZEPHIR_INIT_NVAR(columns);
		ZEPHIR_OBS_VAR(groupColumns);
		if (zephir_array_isset_string_fetch(&groupColumns, params, SS("group"), 0 TSRMLS_CC)) {
			ZEPHIR_CONCAT_VSVSVSV(columns, groupColumns, ", ", functionName, "(", groupColumn, ") AS ", alias);
		} else {
			ZEPHIR_CONCAT_VSVSV(columns, functionName, "(", groupColumn, ") AS ", alias);
		}
	}
	ZEPHIR_INIT_VAR(builder);
	object_init_ex(builder, phalcon_mvc_model_query_builder_ce);
	zephir_call_method_p1_noret(builder, "__construct", params);
	zephir_call_method_p1_noret(builder, "columns", columns);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "get_called_class");
	zephir_call_method_p1_noret(builder, "from", _0);
	ZEPHIR_INIT_VAR(query);
	zephir_call_method(query, builder, "getquery");
	ZEPHIR_INIT_VAR(bindParams);
	ZVAL_NULL(bindParams);
	ZEPHIR_INIT_VAR(bindTypes);
	ZVAL_NULL(bindTypes);
	ZEPHIR_OBS_NVAR(bindParams);
	if (zephir_array_isset_string_fetch(&bindParams, params, SS("bind"), 0 TSRMLS_CC)) {
		ZEPHIR_OBS_NVAR(bindTypes);
	}
	ZEPHIR_INIT_VAR(resultset);
	zephir_call_method_p2(resultset, query, "execute", bindParams, bindTypes);
	ZEPHIR_OBS_VAR(cache);
	if (zephir_array_isset_string_fetch(&cache, params, SS("cache"), 0 TSRMLS_CC)) {
		zephir_call_method_p1_noret(query, "cache", cache);
	}
	if (zephir_array_isset_string(params, SS("group"))) {
		RETURN_CCTOR(resultset);
	}
	ZEPHIR_INIT_VAR(firstRow);
	zephir_call_method(firstRow, resultset, "getfirst");
	ZEPHIR_OBS_VAR(_1);
	zephir_read_property_zval(&_1, firstRow, alias, PH_NOISY_CC);
	RETURN_CCTOR(_1);

}

/**
 * Allows to count how many records match the specified conditions
 *
 * <code>
 *
 * //How many robots are there?
 * $number = Robots::count();
 * echo "There are ", $number, "\n";
 *
 * //How many mechanical robots are there?
 * $number = Robots::count("type='mechanical'");
 * echo "There are ", $number, " mechanical robots\n";
 *
 * </code>
 *
 * @param array parameters
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, count) {

	zval *parameters = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "COUNT", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "rowcount", 1);
	zephir_call_self_p3(return_value, this_ptr, "_groupresult", _0, _1, parameters);
	RETURN_MM();

}

/**
 * Allows to calculate a summatory on a column that match the specified conditions
 *
 * <code>
 *
 * //How much are all robots?
 * $sum = Robots::sum(array('column' => 'price'));
 * echo "The total price of robots is ", $sum, "\n";
 *
 * //How much are mechanical robots?
 * $sum = Robots::sum(array("type='mechanical'", 'column' => 'price'));
 * echo "The total price of mechanical robots is  ", $sum, "\n";
 *
 * </code>
 *
 * @param array parameters
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, sum) {

	zval *parameters = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "SUM", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "sumatory", 1);
	zephir_call_self_p3(return_value, this_ptr, "_groupresult", _0, _1, parameters);
	RETURN_MM();

}

/**
 * Allows to get the maximum value of a column that match the specified conditions
 *
 * <code>
 *
 * //What is the maximum robot id?
 * $id = Robots::maximum(array('column' => 'id'));
 * echo "The maximum robot id is: ", $id, "\n";
 *
 * //What is the maximum id of mechanical robots?
 * $sum = Robots::maximum(array("type='mechanical'", 'column' => 'id'));
 * echo "The maximum robot id of mechanical robots is ", $id, "\n";
 *
 * </code>
 *
 * @param array parameters
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, maximum) {

	zval *parameters = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "MAX", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "maximum", 1);
	zephir_call_self_p3(return_value, this_ptr, "_groupresult", _0, _1, parameters);
	RETURN_MM();

}

/**
 * Allows to get the minimum value of a column that match the specified conditions
 *
 * <code>
 *
 * //What is the minimum robot id?
 * $id = Robots::minimum(array('column' => 'id'));
 * echo "The minimum robot id is: ", $id;
 *
 * //What is the minimum id of mechanical robots?
 * $sum = Robots::minimum(array("type='mechanical'", 'column' => 'id'));
 * echo "The minimum robot id of mechanical robots is ", $id;
 *
 * </code>
 *
 * @param array parameters
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, minimum) {

	zval *parameters = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "MIN", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "minimum", 1);
	zephir_call_self_p3(return_value, this_ptr, "_groupresult", _0, _1, parameters);
	RETURN_MM();

}

/**
 * Allows to calculate the average value on a column matching the specified conditions
 *
 * <code>
 *
 * //What's the average price of robots?
 * $average = Robots::average(array('column' => 'price'));
 * echo "The average price is ", $average, "\n";
 *
 * //What's the average price of mechanical robots?
 * $average = Robots::average(array("type='mechanical'", 'column' => 'price'));
 * echo "The average price of mechanical robots is ", $average, "\n";
 *
 * </code>
 *
 * @param array parameters
 * @return double
 */
PHP_METHOD(Phalcon_Mvc_Model, average) {

	zval *parameters = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &parameters);

	if (!parameters) {
		parameters = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "AVG", 1);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "average", 1);
	zephir_call_self_p3(return_value, this_ptr, "_groupresult", _0, _1, parameters);
	RETURN_MM();

}

/**
 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
 *
 * @param string eventName
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, fireEvent) {

	zval *eventName_param = NULL, *modelsManager = NULL, *_0;
	zval *eventName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &eventName_param);

	if (Z_TYPE_P(eventName_param) != IS_STRING && Z_TYPE_P(eventName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'eventName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(eventName_param) == IS_STRING) {
		eventName = eventName_param;
	} else {
		ZEPHIR_INIT_VAR(eventName);
		ZVAL_EMPTY_STRING(eventName);
	}


	if ((zephir_method_exists(this_ptr, eventName TSRMLS_CC)  == SUCCESS)) {
		zephir_call_method_zval_noret(this_ptr, eventName);
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(modelsManager, _0);
	zephir_call_method_p2(return_value, modelsManager, "notifyevent", eventName, this_ptr);
	RETURN_MM();

}

/**
 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
 * This method stops if one of the callbacks/listeners returns boolean false
 *
 * @param string eventName
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, fireEventCancel) {

	zval *eventName_param = NULL, *modelsManager = NULL, *_0 = NULL, *_1;
	zval *eventName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &eventName_param);

	if (Z_TYPE_P(eventName_param) != IS_STRING && Z_TYPE_P(eventName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'eventName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(eventName_param) == IS_STRING) {
		eventName = eventName_param;
	} else {
		ZEPHIR_INIT_VAR(eventName);
		ZVAL_EMPTY_STRING(eventName);
	}


	if ((zephir_method_exists(this_ptr, eventName TSRMLS_CC)  == SUCCESS)) {
		ZEPHIR_INIT_VAR(_0);
		zephir_call_method_zval(_0, this_ptr, eventName);
		if (ZEPHIR_IS_FALSE(_0)) {
			RETURN_MM_BOOL(0);
		}
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(modelsManager, _1);
	ZEPHIR_INIT_NVAR(_0);
	zephir_call_method_p2(_0, modelsManager, "notifyevent", eventName, this_ptr);
	if (ZEPHIR_IS_FALSE(_0)) {
		RETURN_MM_BOOL(0);
	}
	RETURN_MM_BOOL(1);

}

/**
 * Cancel the current operation
 *
 */
PHP_METHOD(Phalcon_Mvc_Model, _cancelOperation) {

	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_operationMade"), PH_NOISY_CC);
	if (ZEPHIR_IS_LONG(_0, 3)) {
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "notDeleted", 1);
		zephir_call_method_p1_noret(this_ptr, "fireevent", _1);
	} else {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_STRING(_1, "notSaved", 1);
		zephir_call_method_p1_noret(this_ptr, "fireevent", _1);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Appends a customized message on the validation process
 *
 * <code>
 * use \Phalcon\Mvc\Model\Message as Message;
 *
 * class Robots extends Phalcon\Mvc\Model
 * {
 *
 *   public function beforeSave()
 *   {
 *     if ($this->name == 'Peter') {
 *        $message = new Message("Sorry, but a robot cannot be named Peter");
 *        $this->appendMessage($message);
 *     }
 *   }
 * }
 * </code>
 *
 * @param Phalcon\Mvc\Model\MessageInterface message
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, appendMessage) {

	zval *message;

	zephir_fetch_params(0, 1, 0, &message);



	zephir_update_property_array_append(this_ptr, SL("_errorMessages"), message TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Executes validators on every validation call
 *
 *<code>
 *use Phalcon\Mvc\Model\Validator\ExclusionIn as ExclusionIn;
 *
 *class Subscriptors extends Phalcon\Mvc\Model
 *{
 *
 *	public function validation()
 *  {
 * 		$this->validate(new ExclusionIn(array(
 *			'field' => 'status',
 *			'domain' => array('A', 'I')
 *		)));
 *		if ($this->validationHasFailed() == true) {
 *			return false;
 *		}
 *	}
 *
 *}
 *</code>
 *
 * @param object validator
 * @return Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, validate) {

	HashTable *_3;
	HashPosition _2;
	zval *validator, *message = NULL, *_0, *_1, **_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &validator);



	if ((Z_TYPE_P(validator) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Validator must be an Object");
		return;
	}
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p1(_0, validator, "validate", this_ptr);
	if (ZEPHIR_IS_FALSE(_0)) {
		ZEPHIR_INIT_VAR(_1);
		zephir_call_method(_1, validator, "getmessages");
		zephir_is_iterable(_1, &_3, &_2, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
			; zend_hash_move_forward_ex(_3, &_2)
		) {
			ZEPHIR_GET_HVALUE(message, _4);
			zephir_update_property_array_append(this_ptr, SL("_errorMessages"), message TSRMLS_CC);
		}
	}
	RETURN_THIS();

}

/**
 * Check whether validation process has generated any messages
 *
 *<code>
 *use Phalcon\Mvc\Model\Validator\ExclusionIn as ExclusionIn;
 *
 *class Subscriptors extends Phalcon\Mvc\Model
 *{
 *
 *	public function validation()
 *  {
 * 		$this->validate(new ExclusionIn(array(
 *			'field' => 'status',
 *			'domain' => array('A', 'I')
 *		)));
 *		if ($this->validationHasFailed() == true) {
 *			return false;
 *		}
 *	}
 *
 *}
 *</code>
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, validationHasFailed) {

	zval *errorMessages;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(errorMessages);
	zephir_read_property_this(&errorMessages, this_ptr, SL("_errorMessages"), PH_NOISY_CC);
	if ((Z_TYPE_P(errorMessages) == IS_ARRAY)) {
		if (zephir_fast_count_int(errorMessages TSRMLS_CC)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}

/**
 * Returns all the validation messages
 *
 *<code>
 *	$robot = new Robots();
 *	$robot->type = 'mechanical';
 *	$robot->name = 'Astro Boy';
 *	$robot->year = 1952;
 *	if ($robot->save() == false) {
 *  	echo "Umh, We can't store robots right now ";
 *  	foreach ($robot->getMessages() as $message) {
 *			echo $message;
 *		}
 *	} else {
 *  	echo "Great, a new robot was saved successfully!";
 *	}
 * </code>
 *
 * @return Phalcon\Mvc\Model\MessageInterface[]
 */
PHP_METHOD(Phalcon_Mvc_Model, getMessages) {


	RETURN_MEMBER(this_ptr, "_errorMessages");

}

/**
 * Reads "belongs to" relations and check the virtual foreign keys when inserting or updating records
 * to verify that inserted/updated values are present in the related entity
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _checkForeignKeysRestrict) {

	zval *_12 = NULL;
	zend_function *_5 = NULL, *_15 = NULL, *_16 = NULL;
	HashTable *_2, *_7;
	HashPosition _1, _6;
	zend_bool error;
	zval *manager = NULL, *belongsTo, *foreignKey = NULL, *relation = NULL, *conditions = NULL, *position = NULL, *bindParams = NULL, *extraConditions = NULL, *message = NULL, *fields = NULL, *referencedFields = NULL, *field = NULL, *action = NULL, *referencedModel = NULL, *value = NULL, *_0, **_3, *_4 = NULL, **_8, *_9, *_10 = NULL, *_11 = NULL, *_13 = NULL, *_14 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(manager, _0);
	ZEPHIR_INIT_VAR(belongsTo);
	zephir_call_method_p1(belongsTo, manager, "getbelongsto", this_ptr);
	if (zephir_fast_count_int(belongsTo TSRMLS_CC)) {
		error = 0;
		zephir_is_iterable(belongsTo, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HVALUE(relation, _3);
			ZEPHIR_INIT_NVAR(foreignKey);
			zephir_call_method(foreignKey, relation, "getforeignkey");
			if (!ZEPHIR_IS_FALSE(foreignKey)) {
				ZEPHIR_INIT_NVAR(action);
				ZVAL_LONG(action, 1);
				if ((Z_TYPE_P(foreignKey) == IS_ARRAY)) {
					ZEPHIR_OBS_NVAR(action);
				}
				if (ZEPHIR_IS_LONG(action, 1)) {
					ZEPHIR_INIT_NVAR(_4);
					zephir_call_method(_4, relation, "getreferencedmodel");
					ZEPHIR_INIT_NVAR(referencedModel);
					zephir_call_method_p1_cache(referencedModel, manager, "load", &_5, _4);
					ZEPHIR_INIT_NVAR(conditions);
					array_init(conditions);
					ZEPHIR_INIT_NVAR(bindParams);
					array_init(bindParams);
					ZEPHIR_INIT_NVAR(fields);
					zephir_call_method(fields, relation, "getfields");
					ZEPHIR_INIT_NVAR(referencedFields);
					zephir_call_method(referencedFields, relation, "getreferencedfields");
					if ((Z_TYPE_P(fields) == IS_ARRAY)) {
						zephir_is_iterable(fields, &_7, &_6, 0, 0);
						for (
							; zend_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
							; zend_hash_move_forward_ex(_7, &_6)
						) {
							ZEPHIR_GET_HMKEY(position, _7, _6);
							ZEPHIR_GET_HVALUE(field, _8);
							ZEPHIR_OBS_NVAR(value);
							zephir_array_fetch(&_9, referencedFields, position, PH_NOISY | PH_READONLY TSRMLS_CC);
							ZEPHIR_INIT_LNVAR(_10);
							ZEPHIR_CONCAT_SVSV(_10, "[", _9, "] = ?", position);
							zephir_array_append(&conditions, _10, PH_SEPARATE);
							zephir_array_append(&bindParams, value, PH_SEPARATE);
						}
					} else {
						ZEPHIR_OBS_NVAR(value);
						ZEPHIR_INIT_LNVAR(_10);
						ZEPHIR_CONCAT_SVS(_10, "[", referencedFields, "] = ?0");
						zephir_array_append(&conditions, _10, PH_SEPARATE);
						zephir_array_append(&bindParams, value, PH_SEPARATE);
					}
					ZEPHIR_OBS_NVAR(extraConditions);
					if (zephir_array_isset_string_fetch(&extraConditions, foreignKey, SS("conditions"), 0 TSRMLS_CC)) {
						zephir_array_append(&conditions, extraConditions, PH_SEPARATE);
					}
					ZEPHIR_INIT_NVAR(_11);
					ZEPHIR_INIT_NVAR(_12);
					array_init_size(_12, 3);
					ZEPHIR_INIT_NVAR(_13);
					zephir_fast_join_str(_13, SL(" AND "), conditions TSRMLS_CC);
					zephir_array_fast_append(_12, _13);
					zephir_array_update_string(&_12, SL("bind"), &bindParams, PH_COPY | PH_SEPARATE);
					zephir_call_method_p1(_11, referencedModel, "count", _12);
					if (!(zephir_is_true(_11))) {
						ZEPHIR_OBS_NVAR(message);
						if (!(zephir_array_isset_string_fetch(&message, foreignKey, SS("message"), 0 TSRMLS_CC))) {
							ZEPHIR_INIT_NVAR(message);
							if ((Z_TYPE_P(fields) == IS_ARRAY)) {
								ZEPHIR_INIT_NVAR(_13);
								zephir_fast_join_str(_13, SL(", "), fields TSRMLS_CC);
								ZEPHIR_CONCAT_SVS(message, "Value of fields '", _13, "' does not exist on referenced table");
							} else {
								ZEPHIR_CONCAT_SVS(message, "Value of field '", fields, "' does not exist on referenced table");
							}
						}
						ZEPHIR_INIT_LNVAR(_10);
						object_init_ex(_10, phalcon_mvc_model_message_ce);
						ZEPHIR_INIT_NVAR(_14);
						ZVAL_STRING(_14, "ConstraintViolation", 1);
						zephir_call_method_p3_cache_noret(_10, "__construct", &_15, message, fields, _14);
						zephir_call_method_p1_cache_noret(this_ptr, "appendmessage", &_16, _10);
						error = 1;
						break;
					}
				}
			}
		}
		if ((error == 1)) {
			if (ZEPHIR_GLOBAL(orm).events) {
				ZEPHIR_INIT_NVAR(_4);
				ZVAL_STRING(_4, "onValidationFails", 1);
				zephir_call_method_p1_noret(this_ptr, "fireevent", _4);
				zephir_call_method_noret(this_ptr, "_canceloperation");
			}
			RETURN_MM_BOOL(0);
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Reads both "hasMany" and "hasOne" relations and checks the virtual foreign keys (cascade) when deleting records
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _checkForeignKeysReverseCascade) {

	zval *_11 = NULL;
	zend_function *_5 = NULL;
	HashTable *_2, *_7;
	HashPosition _1, _6;
	zval *manager = NULL, *relations, *relation = NULL, *foreignKey = NULL, *resulset = NULL, *conditions = NULL, *bindParams = NULL, *referencedModel = NULL, *referencedFields = NULL, *action = NULL, *fields = NULL, *field = NULL, *position = NULL, *value = NULL, *extraConditions = NULL, *_0, **_3, *_4 = NULL, **_8, *_9, *_10 = NULL, *_12 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(manager, _0);
	ZEPHIR_INIT_VAR(relations);
	zephir_call_method_p1(relations, manager, "gethasoneandhasmany", this_ptr);
	if (zephir_fast_count_int(relations TSRMLS_CC)) {
		zephir_is_iterable(relations, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HVALUE(relation, _3);
			ZEPHIR_INIT_NVAR(foreignKey);
			zephir_call_method(foreignKey, relation, "getforeignkey");
			if (!ZEPHIR_IS_FALSE(foreignKey)) {
				ZEPHIR_INIT_NVAR(action);
				ZVAL_LONG(action, 0);
				if ((Z_TYPE_P(foreignKey) == IS_ARRAY)) {
					ZEPHIR_OBS_NVAR(action);
				}
				if (ZEPHIR_IS_LONG(action, 2)) {
					ZEPHIR_INIT_NVAR(_4);
					zephir_call_method(_4, relation, "getreferencedmodel");
					ZEPHIR_INIT_NVAR(referencedModel);
					zephir_call_method_p1_cache(referencedModel, manager, "load", &_5, _4);
					ZEPHIR_INIT_NVAR(fields);
					zephir_call_method(fields, relation, "getfields");
					ZEPHIR_INIT_NVAR(referencedFields);
					zephir_call_method(referencedFields, relation, "getreferencedfields");
					ZEPHIR_INIT_NVAR(conditions);
					array_init(conditions);
					ZEPHIR_INIT_NVAR(bindParams);
					array_init(bindParams);
					if ((Z_TYPE_P(fields) == IS_ARRAY)) {
						zephir_is_iterable(fields, &_7, &_6, 0, 0);
						for (
							; zend_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
							; zend_hash_move_forward_ex(_7, &_6)
						) {
							ZEPHIR_GET_HMKEY(position, _7, _6);
							ZEPHIR_GET_HVALUE(field, _8);
							ZEPHIR_OBS_NVAR(value);
							zephir_array_fetch(&_9, referencedFields, position, PH_NOISY | PH_READONLY TSRMLS_CC);
							ZEPHIR_INIT_LNVAR(_10);
							ZEPHIR_CONCAT_SVSV(_10, "[", _9, "] = ?", position);
							zephir_array_append(&conditions, _10, PH_SEPARATE);
							zephir_array_append(&bindParams, value, PH_SEPARATE);
						}
					} else {
						ZEPHIR_OBS_NVAR(value);
						ZEPHIR_INIT_LNVAR(_10);
						ZEPHIR_CONCAT_SVS(_10, "[", referencedFields, "] = ?0");
						zephir_array_append(&conditions, _10, PH_SEPARATE);
						zephir_array_append(&bindParams, value, PH_SEPARATE);
					}
					ZEPHIR_OBS_NVAR(extraConditions);
					if (zephir_array_isset_string_fetch(&extraConditions, foreignKey, SS("conditions"), 0 TSRMLS_CC)) {
						zephir_array_append(&conditions, extraConditions, PH_SEPARATE);
					}
					ZEPHIR_INIT_NVAR(_11);
					array_init_size(_11, 3);
					ZEPHIR_INIT_NVAR(_12);
					zephir_fast_join_str(_12, SL(" AND "), conditions TSRMLS_CC);
					zephir_array_fast_append(_11, _12);
					zephir_array_update_string(&_11, SL("bind"), &bindParams, PH_COPY | PH_SEPARATE);
					ZEPHIR_INIT_NVAR(resulset);
					zephir_call_method_p1(resulset, referencedModel, "find", _11);
					ZEPHIR_INIT_NVAR(_12);
					zephir_call_method(_12, resulset, "delete");
					if (ZEPHIR_IS_FALSE(_12)) {
						RETURN_MM_BOOL(0);
					}
				}
			}
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Reads both "hasMany" and "hasOne" relations and checks the virtual foreign keys (restrict) when deleting records
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _checkForeignKeysReverseRestrict) {

	zend_function *_11 = NULL, *_12 = NULL;
	zval *_9 = NULL;
	HashTable *_1, *_4;
	HashPosition _0, _3;
	zval *manager, *relations, *foreignKey = NULL, *action = NULL, *relation = NULL, *relationClass = NULL, *referencedModel = NULL, *fields = NULL, *referencedFields = NULL, *conditions = NULL, *bindParams = NULL, *position = NULL, *field = NULL, *value = NULL, *extraConditions = NULL, *message = NULL, **_2, **_5, *_6, *_7 = NULL, *_8 = NULL, *_10 = NULL;
	zend_bool error;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(relations);
	zephir_call_method_p1(relations, manager, "gethasoneandhasmany", this_ptr);
	if (zephir_fast_count_int(relations TSRMLS_CC)) {
		error = 0;
		zephir_is_iterable(relations, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(relation, _2);
			ZEPHIR_INIT_NVAR(foreignKey);
			zephir_call_method(foreignKey, relation, "getforeignkey");
			if (!ZEPHIR_IS_FALSE(foreignKey)) {
				ZEPHIR_INIT_NVAR(action);
				ZVAL_LONG(action, 1);
				if ((Z_TYPE_P(foreignKey) == IS_ARRAY)) {
					ZEPHIR_OBS_NVAR(action);
				}
				if (ZEPHIR_IS_LONG(action, 1)) {
					ZEPHIR_INIT_NVAR(relationClass);
					zephir_call_method(relationClass, relation, "getreferencedmodel");
					ZEPHIR_INIT_NVAR(referencedModel);
					zephir_call_method_p1(referencedModel, manager, "load", relationClass);
					ZEPHIR_INIT_NVAR(fields);
					zephir_call_method(fields, relation, "getfields");
					ZEPHIR_INIT_NVAR(referencedFields);
					zephir_call_method(referencedFields, relation, "getreferencedfields");
					ZEPHIR_INIT_NVAR(conditions);
					array_init(conditions);
					ZEPHIR_INIT_NVAR(bindParams);
					array_init(bindParams);
					if ((Z_TYPE_P(fields) == IS_ARRAY)) {
						zephir_is_iterable(fields, &_4, &_3, 0, 0);
						for (
							; zend_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
							; zend_hash_move_forward_ex(_4, &_3)
						) {
							ZEPHIR_GET_HMKEY(position, _4, _3);
							ZEPHIR_GET_HVALUE(field, _5);
							ZEPHIR_OBS_NVAR(value);
							zephir_array_fetch(&_6, referencedFields, position, PH_NOISY | PH_READONLY TSRMLS_CC);
							ZEPHIR_INIT_LNVAR(_7);
							ZEPHIR_CONCAT_SVSV(_7, "[", _6, "] = ?", position);
							zephir_array_append(&conditions, _7, PH_SEPARATE);
							zephir_array_append(&bindParams, value, PH_SEPARATE);
						}
					} else {
						ZEPHIR_OBS_NVAR(value);
						ZEPHIR_INIT_LNVAR(_7);
						ZEPHIR_CONCAT_SVS(_7, "[", referencedFields, "] = ?0");
						zephir_array_append(&conditions, _7, PH_SEPARATE);
						zephir_array_append(&bindParams, value, PH_SEPARATE);
					}
					ZEPHIR_OBS_NVAR(extraConditions);
					if (zephir_array_isset_string_fetch(&extraConditions, foreignKey, SS("conditions"), 0 TSRMLS_CC)) {
						zephir_array_append(&conditions, extraConditions, PH_SEPARATE);
					}
					ZEPHIR_INIT_NVAR(_8);
					ZEPHIR_INIT_NVAR(_9);
					array_init_size(_9, 3);
					ZEPHIR_INIT_NVAR(_10);
					zephir_fast_join_str(_10, SL(" AND "), conditions TSRMLS_CC);
					zephir_array_fast_append(_9, _10);
					zephir_array_update_string(&_9, SL("bind"), &bindParams, PH_COPY | PH_SEPARATE);
					zephir_call_method_p1(_8, referencedModel, "count", _9);
					if (zephir_is_true(_8)) {
						ZEPHIR_OBS_NVAR(message);
						if (!(zephir_array_isset_string_fetch(&message, foreignKey, SS("message"), 0 TSRMLS_CC))) {
							ZEPHIR_INIT_NVAR(message);
							ZEPHIR_CONCAT_SV(message, "Record is referenced by model ", relationClass);
						}
						ZEPHIR_INIT_LNVAR(_7);
						object_init_ex(_7, phalcon_mvc_model_message_ce);
						ZEPHIR_INIT_NVAR(_10);
						ZVAL_STRING(_10, "ConstraintViolation", 1);
						zephir_call_method_p3_cache_noret(_7, "__construct", &_11, message, fields, _10);
						zephir_call_method_p1_cache_noret(this_ptr, "appendmessage", &_12, _7);
						error = 1;
						break;
					}
				}
			}
		}
		if ((error == 1)) {
			if (ZEPHIR_GLOBAL(orm).events) {
				ZEPHIR_INIT_NVAR(_8);
				ZVAL_STRING(_8, "onValidationFails", 1);
				zephir_call_method_p1_noret(this_ptr, "fireevent", _8);
				zephir_call_method_noret(this_ptr, "_canceloperation");
			}
			RETURN_MM_BOOL(0);
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Executes internal hooks before save a record
 *
 * @param Phalcon\Mvc\Model\MetadataInterface metaData
 * @param boolean exists
 * @param string identityField
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _preSave) {

	zend_function *_9 = NULL, *_10 = NULL;
	HashTable *_5;
	HashPosition _4;
	zend_bool exists, error, isNull;
	zval *metaData, *exists_param = NULL, *identityField, *notNull, *columnMap, *dataTypeNumeric, *automaticAttributes, *field = NULL, *attributeField = NULL, *value = NULL, *_0 = NULL, *_1 = NULL, *_2 = NULL, *_3 = NULL, **_6, *_7 = NULL, *_8 = NULL, *_11;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &metaData, &exists_param, &identityField);

		exists = zephir_get_boolval(exists_param);


	if (ZEPHIR_GLOBAL(orm).events) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "beforeValidation", 1);
		zephir_call_method_p1(_0, this_ptr, "fireeventcancel", _1);
		if (ZEPHIR_IS_FALSE(_0)) {
			RETURN_MM_BOOL(0);
		}
		if (exists) {
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_VAR(_2);
			ZVAL_STRING(_2, "beforeValidationOnCreate", 1);
			zephir_call_method_p1(_1, this_ptr, "fireeventcancel", _2);
			if (ZEPHIR_IS_FALSE(_1)) {
				RETURN_MM_BOOL(0);
			}
		} else {
			ZEPHIR_INIT_NVAR(_2);
			ZEPHIR_INIT_VAR(_3);
			ZVAL_STRING(_3, "beforeValidationOnUpdate", 1);
			zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _3);
			if (ZEPHIR_IS_FALSE(_2)) {
				RETURN_MM_BOOL(0);
			}
		}
	}
	if (ZEPHIR_GLOBAL(orm).virtual_foreign_keys) {
		ZEPHIR_INIT_NVAR(_0);
		zephir_call_method(_0, this_ptr, "_checkforeignkeysrestrict");
		if (ZEPHIR_IS_FALSE(_0)) {
			RETURN_MM_BOOL(0);
		}
	}
	if (ZEPHIR_GLOBAL(orm).not_null_validations) {
		ZEPHIR_INIT_VAR(notNull);
		zephir_call_method_p1(notNull, metaData, "getnotnullattributes", this_ptr);
		if ((Z_TYPE_P(notNull) == IS_ARRAY)) {
			ZEPHIR_INIT_VAR(dataTypeNumeric);
			zephir_call_method_p1(dataTypeNumeric, metaData, "getdatatypesnumeric", this_ptr);
			ZEPHIR_INIT_VAR(columnMap);
			if (ZEPHIR_GLOBAL(orm).column_renaming) {
				zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
			} else {
				ZVAL_NULL(columnMap);
			}
			ZEPHIR_INIT_VAR(automaticAttributes);
			if (exists) {
				zephir_call_method_p1(automaticAttributes, metaData, "getautomaticupdateattributes", this_ptr);
			} else {
				zephir_call_method_p1(automaticAttributes, metaData, "getautomaticcreateattributes", this_ptr);
			}
			error = 0;
			zephir_is_iterable(notNull, &_5, &_4, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
				; zend_hash_move_forward_ex(_5, &_4)
			) {
				ZEPHIR_GET_HVALUE(field, _6);
				if (!(zephir_array_isset(automaticAttributes, field))) {
					isNull = 0;
					if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
						ZEPHIR_OBS_NVAR(attributeField);
						if (!(zephir_array_isset_fetch(&attributeField, columnMap, field, 0 TSRMLS_CC))) {
							ZEPHIR_INIT_LNVAR(_7);
							object_init_ex(_7, phalcon_mvc_model_exception_ce);
							ZEPHIR_INIT_LNVAR(_8);
							ZEPHIR_CONCAT_SVS(_8, "Column '", field, "' isn't part of the column map");
							zephir_call_method_p1_cache_noret(_7, "__construct", &_9, _8);
							zephir_throw_exception(_7 TSRMLS_CC);
							ZEPHIR_MM_RESTORE();
							return;
						}
					} else {
						ZEPHIR_CPY_WRT(attributeField, field);
					}
					ZEPHIR_OBS_NVAR(value);
					if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
						if ((Z_TYPE_P(value) != IS_OBJECT)) {
							if (!(zephir_array_isset(dataTypeNumeric, field))) {
								if (ZEPHIR_IS_EMPTY(value)) {
									isNull = 1;
								}
							} else {
								if (!(zephir_is_numeric(value))) {
									isNull = 1;
								}
							}
						}
					} else {
						isNull = 1;
					}
					if ((isNull == 1)) {
						if (!(exists)) {
							if (ZEPHIR_IS_EQUAL(field, identityField)) {
								continue;
							}
						}
						ZEPHIR_INIT_LNVAR(_7);
						object_init_ex(_7, phalcon_mvc_model_message_ce);
						ZEPHIR_INIT_LNVAR(_8);
						ZEPHIR_CONCAT_VS(_8, attributeField, " is required");
						ZEPHIR_INIT_NVAR(_0);
						ZVAL_STRING(_0, "PresenceOf", 1);
						zephir_call_method_p3_cache_noret(_7, "__construct", &_10, _8, attributeField, _0);
						zephir_update_property_array_append(this_ptr, SL("_errorMessages"), _7 TSRMLS_CC);
						error = 1;
					}
				}
			}
			if ((error == 1)) {
				if (ZEPHIR_GLOBAL(orm).events) {
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_STRING(_0, "onValidationFails", 1);
					zephir_call_method_p1_noret(this_ptr, "fireevent", _0);
					zephir_call_method_noret(this_ptr, "_canceloperation");
				}
				RETURN_MM_BOOL(0);
			}
		}
	}
	ZEPHIR_INIT_NVAR(_0);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "validation", 1);
	zephir_call_method_p1(_0, this_ptr, "fireeventcancel", _1);
	if (ZEPHIR_IS_FALSE(_0)) {
		if (ZEPHIR_GLOBAL(orm).events) {
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "onValidationFails", 1);
			zephir_call_method_p1_noret(this_ptr, "fireevent", _1);
		}
		RETURN_MM_BOOL(0);
	}
	if (ZEPHIR_GLOBAL(orm).events) {
		if (!(exists)) {
			ZEPHIR_INIT_NVAR(_0);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "afterValidationOnCreate", 1);
			zephir_call_method_p1(_0, this_ptr, "fireeventcancel", _1);
			if (ZEPHIR_IS_FALSE(_0)) {
				RETURN_MM_BOOL(0);
			}
		} else {
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_STRING(_2, "afterValidationOnUpdate", 1);
			zephir_call_method_p1(_1, this_ptr, "fireeventcancel", _2);
			if (ZEPHIR_IS_FALSE(_1)) {
				RETURN_MM_BOOL(0);
			}
		}
		ZEPHIR_INIT_NVAR(_2);
		ZEPHIR_INIT_NVAR(_3);
		ZVAL_STRING(_3, "afterValidation", 1);
		zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _3);
		if (ZEPHIR_IS_FALSE(_2)) {
			RETURN_MM_BOOL(0);
		}
		ZEPHIR_INIT_NVAR(_2);
		ZEPHIR_INIT_NVAR(_3);
		ZVAL_STRING(_3, "beforeSave", 1);
		zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _3);
		if (ZEPHIR_IS_FALSE(_2)) {
			RETURN_MM_BOOL(0);
		}
		zephir_update_property_this(this_ptr, SL("_skipped"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
		if (exists) {
			ZEPHIR_INIT_NVAR(_2);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_STRING(_3, "beforeUpdate", 1);
			zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _3);
			if (ZEPHIR_IS_FALSE(_2)) {
				RETURN_MM_BOOL(0);
			}
		} else {
			ZEPHIR_INIT_NVAR(_2);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_STRING(_3, "beforeCreate", 1);
			zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _3);
			if (ZEPHIR_IS_FALSE(_2)) {
				RETURN_MM_BOOL(0);
			}
		}
		_11 = zephir_fetch_nproperty_this(this_ptr, SL("_skipped"), PH_NOISY_CC);
		if (ZEPHIR_IS_TRUE(_11)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Executes internal events after save a record
 *
 * @param boolean success
 * @param boolean exists
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _postSave) {

	zval *success_param = NULL, *exists_param = NULL, *_0 = NULL;
	zend_bool success, exists;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &success_param, &exists_param);

		success = zephir_get_boolval(success_param);
		exists = zephir_get_boolval(exists_param);


	if ((success == 1)) {
		if (exists) {
			ZEPHIR_INIT_VAR(_0);
			ZVAL_STRING(_0, "afterUpdate", 1);
			zephir_call_method_p1_noret(this_ptr, "fireevent", _0);
		} else {
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_STRING(_0, "afterCreate", 1);
			zephir_call_method_p1_noret(this_ptr, "fireevent", _0);
		}
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "afterSave", 1);
		zephir_call_method_p1_noret(this_ptr, "fireevent", _0);
		RETURN_MM_BOOL(success);
	}
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "notSave", 1);
	zephir_call_method_p1_noret(this_ptr, "fireevent", _0);
	zephir_call_method_noret(this_ptr, "_canceloperation");
	RETURN_MM_BOOL(0);

}

/**
 * Sends a pre-build INSERT SQL statement to the relational database system
 *
 * @param Phalcon\Mvc\Model\MetadataInterface metaData
 * @param Phalcon\Db\AdapterInterface connection
 * @param string|array table
 * @param boolean|string identityField
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _doLowInsert) {

	zend_function *_5 = NULL, *_6 = NULL;
	HashTable *_1;
	HashPosition _0;
	zend_bool useExplicitIdentity, _8;
	zval *metaData, *connection, *table, *identityField, *bindSkip, *fields, *values, *bindTypes, *attributes, *bindDataTypes, *automaticAttributes, *field = NULL, *columnMap, *value = NULL, *attributeField = NULL, *success, *bindType = NULL, *defaultValue, *sequenceName = NULL, **_2, *_3 = NULL, *_4 = NULL, *_7 = NULL, *_9 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &metaData, &connection, &table, &identityField);



	ZEPHIR_INIT_VAR(bindSkip);
	ZVAL_LONG(bindSkip, 1024);
	ZEPHIR_INIT_VAR(fields);
	array_init(fields);
	ZEPHIR_INIT_VAR(values);
	array_init(values);
	ZEPHIR_INIT_VAR(bindTypes);
	array_init(bindTypes);
	ZEPHIR_INIT_VAR(attributes);
	zephir_call_method_p1(attributes, metaData, "getattributes", this_ptr);
	ZEPHIR_INIT_VAR(bindDataTypes);
	zephir_call_method_p1(bindDataTypes, metaData, "getbindtypes", this_ptr);
	ZEPHIR_INIT_VAR(automaticAttributes);
	zephir_call_method_p1(automaticAttributes, metaData, "getautomaticcreateattributes", this_ptr);
	ZEPHIR_INIT_VAR(columnMap);
	if (ZEPHIR_GLOBAL(orm).column_renaming) {
		zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
	} else {
		ZVAL_NULL(columnMap);
	}
	zephir_is_iterable(attributes, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(field, _2);
		if (!(zephir_array_isset(automaticAttributes, field))) {
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, field, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_3);
					object_init_ex(_3, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_4);
					ZEPHIR_CONCAT_SVS(_4, "Column '", field, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_3, "__construct", &_5, _4);
					zephir_throw_exception(_3 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, field);
			}
			if (!ZEPHIR_IS_EQUAL(field, identityField)) {
				zephir_array_append(&fields, field, PH_SEPARATE);
				ZEPHIR_OBS_NVAR(value);
				if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
					ZEPHIR_OBS_NVAR(bindType);
					if (!(zephir_array_isset_fetch(&bindType, bindDataTypes, field, 0 TSRMLS_CC))) {
						ZEPHIR_INIT_LNVAR(_3);
						object_init_ex(_3, phalcon_mvc_model_exception_ce);
						ZEPHIR_INIT_LNVAR(_4);
						ZEPHIR_CONCAT_SVS(_4, "Column '", field, "' have not defined a bind data type");
						zephir_call_method_p1_cache_noret(_3, "__construct", &_6, _4);
						zephir_throw_exception(_3 TSRMLS_CC);
						ZEPHIR_MM_RESTORE();
						return;
					}
					zephir_array_append(&values, value, PH_SEPARATE);
					zephir_array_append(&bindTypes, bindType, PH_SEPARATE);
				} else {
					zephir_array_append(&values, ZEPHIR_GLOBAL(global_null), PH_SEPARATE);
					zephir_array_append(&bindTypes, bindSkip, PH_SEPARATE);
				}
			}
		}
	}
	if (!ZEPHIR_IS_FALSE(identityField)) {
		ZEPHIR_INIT_VAR(defaultValue);
		zephir_call_method(defaultValue, connection, "getdefaultidvalue");
		ZEPHIR_INIT_VAR(_7);
		zephir_call_method(_7, connection, "useexplicitidvalue");
		useExplicitIdentity = zephir_get_boolval(_7);
		if (useExplicitIdentity) {
			zephir_array_append(&fields, identityField, PH_SEPARATE);
		}
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(attributeField);
			if (!(zephir_array_isset_fetch(&attributeField, columnMap, identityField, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_3);
				object_init_ex(_3, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_4);
				ZEPHIR_CONCAT_SVS(_4, "Identity column '", identityField, "' isn't part of the column map");
				zephir_call_method_p1_noret(_3, "__construct", _4);
				zephir_throw_exception(_3 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			ZEPHIR_CPY_WRT(attributeField, identityField);
		}
		ZEPHIR_OBS_NVAR(value);
		if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
			if (ZEPHIR_IS_EMPTY(value)) {
				if (useExplicitIdentity) {
					zephir_array_append(&values, defaultValue, PH_SEPARATE);
					zephir_array_append(&bindTypes, bindSkip, PH_SEPARATE);
				}
			} else {
				if (!(useExplicitIdentity)) {
					zephir_array_append(&fields, identityField, PH_SEPARATE);
				}
				ZEPHIR_OBS_NVAR(bindType);
				if (!(zephir_array_isset_fetch(&bindType, bindDataTypes, identityField, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_3);
					object_init_ex(_3, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_4);
					ZEPHIR_CONCAT_SVS(_4, "Identity column '", identityField, "' isn\'t part of the table columns");
					zephir_call_method_p1_noret(_3, "__construct", _4);
					zephir_throw_exception(_3 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
				zephir_array_append(&values, value, PH_SEPARATE);
				zephir_array_append(&bindTypes, bindType, PH_SEPARATE);
			}
		} else {
			if (useExplicitIdentity) {
				zephir_array_append(&values, defaultValue, PH_SEPARATE);
				zephir_array_append(&bindTypes, bindSkip, PH_SEPARATE);
			}
		}
	}
	ZEPHIR_INIT_VAR(success);
	zephir_call_method_p4(success, connection, "insert", table, values, fields, bindTypes);
	_8 = zephir_is_true(success);
	if (_8) {
		_8 = !ZEPHIR_IS_FALSE(identityField);
	}
	if (_8) {
		ZEPHIR_INIT_VAR(sequenceName);
		ZVAL_NULL(sequenceName);
		ZEPHIR_INIT_NVAR(_7);
		zephir_call_method(_7, connection, "supportsequences");
		if (ZEPHIR_IS_TRUE(_7)) {
			ZEPHIR_INIT_NVAR(sequenceName);
			if ((zephir_method_exists_ex(this_ptr, SS("getsequencename") TSRMLS_CC) == SUCCESS)) {
				zephir_call_method(sequenceName, this_ptr, "getsequencename");
			} else {
				ZEPHIR_INIT_VAR(_9);
				zephir_call_method(_9, this_ptr, "getsource");
				ZEPHIR_CONCAT_VSVS(sequenceName, _9, "_", identityField, "_seq");
			}
		}
		ZEPHIR_INIT_NVAR(_9);
		zephir_call_method_p1(_9, connection, "lastinsertid", sequenceName);
		zephir_update_property_zval_zval(this_ptr, attributeField, _9 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("_uniqueParams"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	RETURN_CCTOR(success);

}

/**
 * Sends a pre-build UPDATE SQL statement to the relational database system
 *
 * @param Phalcon\Mvc\Model\MetaDataInterface metaData
 * @param Phalcon\Db\AdapterInterface connection
 * @param string|array table
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _doLowUpdate) {

	zval *_12;
	zend_function *_6 = NULL, *_7 = NULL, *_11 = NULL;
	HashTable *_2, *_9;
	HashPosition _1, _8;
	zend_bool useDynamicUpdate, changed;
	zval *metaData, *connection, *table, *bindSkip, *fields, *values, *bindTypes, *manager, *bindDataTypes, *field = NULL, *automaticAttributes, *snapshotValue = NULL, *uniqueKey, *uniqueParams, *uniqueTypes, *snapshot, *nonPrimary, *columnMap, *attributeField = NULL, *value = NULL, *primaryKeys, *bindType = NULL, *_0, **_3, *_4 = NULL, *_5 = NULL, **_10;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &metaData, &connection, &table);



	ZEPHIR_INIT_VAR(bindSkip);
	ZVAL_LONG(bindSkip, 1024);
	ZEPHIR_INIT_VAR(fields);
	array_init(fields);
	ZEPHIR_INIT_VAR(values);
	array_init(values);
	ZEPHIR_INIT_VAR(bindTypes);
	array_init(bindTypes);
	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p1(_0, manager, "isusingdynamicupdate", this_ptr);
	useDynamicUpdate = zephir_get_boolval(_0);
	if (useDynamicUpdate) {
		ZEPHIR_OBS_VAR(snapshot);
		zephir_read_property_this(&snapshot, this_ptr, SL("_snapshot"), PH_NOISY_CC);
		if ((Z_TYPE_P(snapshot) != IS_ARRAY)) {
			useDynamicUpdate = 0;
		}
	}
	ZEPHIR_INIT_VAR(bindDataTypes);
	zephir_call_method_p1(bindDataTypes, metaData, "getbindtypes", this_ptr);
	ZEPHIR_INIT_VAR(nonPrimary);
	zephir_call_method_p1(nonPrimary, metaData, "getnonprimarykeyattributes", this_ptr);
	ZEPHIR_INIT_VAR(automaticAttributes);
	zephir_call_method_p1(automaticAttributes, metaData, "getautomaticupdateattributes", this_ptr);
	ZEPHIR_INIT_VAR(columnMap);
	if (ZEPHIR_GLOBAL(orm).column_renaming) {
		zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
	} else {
		ZVAL_NULL(columnMap);
	}
	zephir_is_iterable(nonPrimary, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(field, _3);
		if (!(zephir_array_isset(automaticAttributes, field))) {
			ZEPHIR_OBS_NVAR(bindType);
			if (!(zephir_array_isset_fetch(&bindType, bindDataTypes, field, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_4);
				object_init_ex(_4, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_5);
				ZEPHIR_CONCAT_SVS(_5, "Column '", field, "' have not defined a bind data type");
				zephir_call_method_p1_cache_noret(_4, "__construct", &_6, _5);
				zephir_throw_exception(_4 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, field, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_4);
					object_init_ex(_4, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_5);
					ZEPHIR_CONCAT_SVS(_5, "Column '", field, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_4, "__construct", &_7, _5);
					zephir_throw_exception(_4 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, field);
			}
			ZEPHIR_OBS_NVAR(value);
			if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
				if (!(useDynamicUpdate)) {
					zephir_array_append(&fields, field, PH_SEPARATE);
					zephir_array_append(&values, value, PH_SEPARATE);
					zephir_array_append(&bindTypes, bindType, PH_SEPARATE);
				} else {
					ZEPHIR_OBS_NVAR(snapshotValue);
					if (!(zephir_array_isset_fetch(&snapshotValue, snapshot, attributeField, 0 TSRMLS_CC))) {
						changed = 1;
					} else {
						changed = !ZEPHIR_IS_EQUAL(value, snapshotValue);
					}
					if (changed) {
						zephir_array_append(&fields, field, PH_SEPARATE);
						zephir_array_append(&values, value, PH_SEPARATE);
						zephir_array_append(&bindTypes, bindType, PH_SEPARATE);
					}
				}
			} else {
				zephir_array_append(&fields, field, PH_SEPARATE);
				zephir_array_append(&values, ZEPHIR_GLOBAL(global_null), PH_SEPARATE);
				zephir_array_append(&bindTypes, bindSkip, PH_SEPARATE);
			}
		}
	}
	if (!(zephir_fast_count_int(fields TSRMLS_CC))) {
		RETURN_MM_BOOL(1);
	}
	ZEPHIR_OBS_VAR(uniqueKey);
	zephir_read_property_this(&uniqueKey, this_ptr, SL("_uniqueKey"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(uniqueParams);
	zephir_read_property_this(&uniqueParams, this_ptr, SL("_uniqueParams"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(uniqueTypes);
	zephir_read_property_this(&uniqueTypes, this_ptr, SL("_uniqueTypes"), PH_NOISY_CC);
	if ((Z_TYPE_P(uniqueParams) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(primaryKeys);
		zephir_call_method_p1(primaryKeys, metaData, "getprimarykeyattributes", this_ptr);
		if (!(zephir_fast_count_int(primaryKeys TSRMLS_CC))) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "A primary key must be defined in the model in order to perform the operation");
			return;
		}
		ZEPHIR_INIT_BNVAR(uniqueParams);
		array_init(uniqueParams);
		zephir_is_iterable(primaryKeys, &_9, &_8, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_9, (void**) &_10, &_8) == SUCCESS
			; zend_hash_move_forward_ex(_9, &_8)
		) {
			ZEPHIR_GET_HVALUE(field, _10);
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, field, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_4);
					object_init_ex(_4, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_5);
					ZEPHIR_CONCAT_SVS(_5, "Column '", field, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_4, "__construct", &_11, _5);
					zephir_throw_exception(_4 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, field);
			}
			ZEPHIR_OBS_NVAR(value);
			if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
				zephir_array_append(&uniqueParams, value, PH_SEPARATE);
			} else {
				zephir_array_append(&uniqueParams, ZEPHIR_GLOBAL(global_null), PH_SEPARATE);
			}
		}
	}
	ZEPHIR_INIT_VAR(_12);
	array_init_size(_12, 5);
	zephir_array_update_string(&_12, SL("conditions"), &uniqueKey, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_12, SL("bind"), &uniqueParams, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_12, SL("bindTypes"), &uniqueTypes, PH_COPY | PH_SEPARATE);
	zephir_call_method_p5(return_value, connection, "update", table, fields, values, _12, bindTypes);
	RETURN_MM();

}

/**
 * Saves related records that must be stored prior to save the master record
 *
 * @param Phalcon\Db\AdapterInterface connection
 * @param Phalcon\Mvc\ModelInterface[] related
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _preSaveRelatedRecords) {

	zend_function *_3 = NULL, *_4 = NULL, *_5 = NULL, *_11 = NULL, *_12 = NULL;
	HashTable *_1, *_9;
	HashPosition _0, _8;
	zend_bool nesting;
	zval *connection, *related, *className, *manager, *type = NULL, *relation = NULL, *columns = NULL, *referencedFields = NULL, *referencedModel = NULL, *message = NULL, *name = NULL, *record = NULL, **_2, *_6 = NULL, *_7 = NULL, **_10;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &connection, &related);



	nesting = 0;
	zephir_call_method_p1_noret(connection, "begin", (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(manager);
	zephir_call_method(manager, this_ptr, "getmodelsmanager");
	zephir_is_iterable(related, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(name, _1, _0);
		ZEPHIR_GET_HVALUE(record, _2);
		ZEPHIR_INIT_NVAR(relation);
		zephir_call_method_p2_cache(relation, manager, "getrelationbyalias", &_3, className, name);
		if ((Z_TYPE_P(relation) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(type);
			zephir_call_method(type, relation, "gettype");
			if (ZEPHIR_IS_LONG(type, 0)) {
				if ((Z_TYPE_P(record) != IS_OBJECT)) {
					zephir_call_method_p1_cache_noret(connection, "rollback", &_4, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
					ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Only objects can be stored as part of belongs-to relations");
					return;
				}
				ZEPHIR_INIT_NVAR(columns);
				zephir_call_method(columns, relation, "getfields");
				ZEPHIR_INIT_NVAR(referencedModel);
				zephir_call_method(referencedModel, relation, "getreferencedmodel");
				ZEPHIR_INIT_NVAR(referencedFields);
				zephir_call_method(referencedFields, relation, "getreferencedfields");
				if ((Z_TYPE_P(columns) == IS_ARRAY)) {
					zephir_call_method_p1_cache_noret(connection, "rollback", &_5, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
					ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Not implemented");
					return;
				}
				ZEPHIR_INIT_NVAR(_6);
				zephir_call_method(_6, record, "save");
				if (!(zephir_is_true(_6))) {
					ZEPHIR_INIT_NVAR(_7);
					zephir_call_method(_7, record, "getmessages");
					zephir_is_iterable(_7, &_9, &_8, 0, 0);
					for (
						; zend_hash_get_current_data_ex(_9, (void**) &_10, &_8) == SUCCESS
						; zend_hash_move_forward_ex(_9, &_8)
					) {
						ZEPHIR_GET_HVALUE(message, _10);
						if ((Z_TYPE_P(record) == IS_OBJECT)) {
							zephir_call_method_p1_noret(message, "setmodel", record);
						}
						zephir_call_method_p1_cache_noret(this_ptr, "appendmessage", &_11, message);
					}
					zephir_call_method_p1_cache_noret(connection, "rollback", &_12, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
					RETURN_MM_BOOL(0);
				}
				ZEPHIR_INIT_NVAR(_7);
				zephir_call_method_p1(_7, record, "readattribute", referencedFields);
				zephir_update_property_zval_zval(this_ptr, columns, _7 TSRMLS_CC);
			}
		}
	}
	RETURN_MM_BOOL(1);

}

/**
 * Save the related records assigned in the has-one/has-many relations
 *
 * @param Phalcon\Db\AdapterInterface connection
 * @param Phalcon\Mvc\ModelInterface[] related
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, _postSaveRelatedRecords) {

	zend_function *_3 = NULL, *_5 = NULL, *_6 = NULL, *_7 = NULL, *_10 = NULL, *_15 = NULL, *_20 = NULL, *_21 = NULL, *_26 = NULL, *_27 = NULL, *_28 = NULL, *_29 = NULL;
	HashTable *_1, *_13, *_18, *_24;
	HashPosition _0, _12, _17, _23;
	zend_bool nesting, isThrough;
	zval *connection, *related, *className, *manager, *relation = NULL, *name = NULL, *record = NULL, *message = NULL, *columns = NULL, *referencedModel = NULL, *referencedFields = NULL, *relatedRecords = NULL, *value = NULL, *recordAfter = NULL, *intermediateModel = NULL, *intermediateFields = NULL, *intermediateValue = NULL, *intermediateModelName = NULL, *intermediateReferencedFields = NULL, **_2, *_4 = NULL, *_8 = NULL, *_9 = NULL, *_11 = NULL, **_14, *_16 = NULL, **_19, *_22 = NULL, **_25;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &connection, &related);



	nesting = 0;
	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(manager);
	zephir_call_method(manager, this_ptr, "getmodelsmanager");
	zephir_is_iterable(related, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(name, _1, _0);
		ZEPHIR_GET_HVALUE(record, _2);
		ZEPHIR_INIT_NVAR(relation);
		zephir_call_method_p2_cache(relation, manager, "getrelationbyalias", &_3, className, name);
		if ((Z_TYPE_P(relation) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(_4);
			zephir_call_method(_4, relation, "gettype");
			if (ZEPHIR_IS_LONG(_4, 0)) {
				continue;
			}
			if ((Z_TYPE_P(record) != IS_OBJECT)) {
				if ((Z_TYPE_P(record) != IS_ARRAY)) {
					zephir_call_method_p1_cache_noret(connection, "rollback", &_5, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
					ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Only objects/arrays can be stored as part of has-many/has-one/has-many-to-many relations");
					return;
				}
			}
			ZEPHIR_INIT_NVAR(columns);
			zephir_call_method(columns, relation, "getfields");
			ZEPHIR_INIT_NVAR(referencedModel);
			zephir_call_method(referencedModel, relation, "getreferencedmodel");
			ZEPHIR_INIT_NVAR(referencedFields);
			zephir_call_method(referencedFields, relation, "getreferencedfields");
			if ((Z_TYPE_P(columns) == IS_ARRAY)) {
				zephir_call_method_p1_cache_noret(connection, "rollback", &_6, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Not implemented");
				return;
			}
			if ((Z_TYPE_P(record) == IS_OBJECT)) {
				ZEPHIR_INIT_NVAR(relatedRecords);
				array_init_size(relatedRecords, 2);
				zephir_array_fast_append(relatedRecords, record);
			} else {
				ZEPHIR_CPY_WRT(relatedRecords, record);
			}
			ZEPHIR_OBS_NVAR(value);
			if (!(zephir_read_property_zval(&value, this_ptr, columns, PH_SILENT_CC))) {
				zephir_call_method_p1_cache_noret(connection, "rollback", &_7, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
				ZEPHIR_INIT_LNVAR(_8);
				object_init_ex(_8, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_9);
				ZEPHIR_CONCAT_SVS(_9, "The column '", columns, "' needs to be present in the model");
				zephir_call_method_p1_cache_noret(_8, "__construct", &_10, _9);
				zephir_throw_exception(_8 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			ZEPHIR_INIT_NVAR(_11);
			zephir_call_method(_11, relation, "isthrough");
			isThrough = zephir_get_boolval(_11);
			if (isThrough) {
				ZEPHIR_INIT_NVAR(intermediateModelName);
				zephir_call_method(intermediateModelName, relation, "getintermediatemodel");
				ZEPHIR_INIT_NVAR(intermediateFields);
				zephir_call_method(intermediateFields, relation, "getintermediatefields");
				ZEPHIR_INIT_NVAR(intermediateReferencedFields);
				zephir_call_method(intermediateReferencedFields, relation, "getintermediatereferencedfields");
			}
			zephir_is_iterable(relatedRecords, &_13, &_12, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_13, (void**) &_14, &_12) == SUCCESS
				; zend_hash_move_forward_ex(_13, &_12)
			) {
				ZEPHIR_GET_HVALUE(recordAfter, _14);
				if (!(isThrough)) {
					zephir_call_method_p2_noret(recordAfter, "writeattribute", referencedFields, value);
				} else {
					ZEPHIR_INIT_NVAR(intermediateModel);
					zephir_call_method_p2_cache(intermediateModel, manager, "load", &_15, intermediateModelName, ZEPHIR_GLOBAL(global_true));
					zephir_call_method_p2_noret(intermediateModel, "writeattribute", intermediateFields, value);
					ZEPHIR_OBS_NVAR(intermediateValue);
					zephir_read_property_zval(&intermediateValue, recordAfter, referencedFields, PH_NOISY_CC);
					zephir_call_method_p2_noret(intermediateModel, "writeattribute", intermediateReferencedFields, intermediateValue);
					ZEPHIR_INIT_NVAR(_11);
					zephir_call_method(_11, intermediateModel, "save");
					if (!(zephir_is_true(_11))) {
						ZEPHIR_INIT_NVAR(_16);
						zephir_call_method(_16, intermediateModel, "getmessages");
						zephir_is_iterable(_16, &_18, &_17, 0, 0);
						for (
							; zend_hash_get_current_data_ex(_18, (void**) &_19, &_17) == SUCCESS
							; zend_hash_move_forward_ex(_18, &_17)
						) {
							ZEPHIR_GET_HVALUE(message, _19);
							if ((Z_TYPE_P(message) != IS_OBJECT)) {
								zephir_call_method_p1_noret(message, "setmodel", record);
							}
							zephir_call_method_p1_cache_noret(this_ptr, "appendmessage", &_20, message);
						}
						zephir_call_method_p1_cache_noret(connection, "rollback", &_21, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
						RETURN_MM_BOOL(0);
					}
				}
				ZEPHIR_INIT_NVAR(_16);
				zephir_call_method(_16, recordAfter, "save");
				if (!(zephir_is_true(_16))) {
					ZEPHIR_INIT_NVAR(_22);
					zephir_call_method(_22, recordAfter, "getmessages");
					zephir_is_iterable(_22, &_24, &_23, 0, 0);
					for (
						; zend_hash_get_current_data_ex(_24, (void**) &_25, &_23) == SUCCESS
						; zend_hash_move_forward_ex(_24, &_23)
					) {
						ZEPHIR_GET_HVALUE(message, _25);
						if ((Z_TYPE_P(message) == IS_OBJECT)) {
							zephir_call_method_p1_noret(message, "setmodel", record);
						}
						zephir_call_method_p1_cache_noret(this_ptr, "appendmessage", &_26, message);
					}
					zephir_call_method_p1_cache_noret(connection, "rollback", &_27, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
					RETURN_MM_BOOL(0);
				}
			}
		} else {
			if ((Z_TYPE_P(record) != IS_ARRAY)) {
				zephir_call_method_p1_cache_noret(connection, "rollback", &_28, (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
				ZEPHIR_INIT_LNVAR(_8);
				object_init_ex(_8, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_9);
				ZEPHIR_CONCAT_SVSVS(_9, "There are no defined relations for the model '", className, "' using alias '", name, "'");
				zephir_call_method_p1_cache_noret(_8, "__construct", &_29, _9);
				zephir_throw_exception(_8 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		}
	}
	zephir_call_method_p1_noret(connection, "commit", (nesting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
	RETURN_MM_BOOL(1);

}

/**
 * Inserts or updates a model instance. Returning true on success or false otherwise.
 *
 *<code>
 *	//Creating a new robot
 *	$robot = new Robots();
 *	$robot->type = 'mechanical';
 *	$robot->name = 'Astro Boy';
 *	$robot->year = 1952;
 *	$robot->save();
 *
 *	//Updating a robot name
 *	$robot = Robots::findFirst("id=100");
 *	$robot->name = "Biomass";
 *	$robot->save();
 *</code>
 *
 * @param array data
 * @param array whiteList
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, save) {

	HashTable *_1;
	HashPosition _0;
	zval *data = NULL, *whiteList = NULL, *metaData, *attribute = NULL, *attributes = NULL, *related, *schema, *possibleSetter = NULL, *value = NULL, *writeConnection, *readConnection, *source, *table = NULL, *identityField, *exists, *success = NULL, **_2, *_3 = NULL, *_4, *_5, *_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &data, &whiteList);

	if (!data) {
		data = ZEPHIR_GLOBAL(global_null);
	}
	if (!whiteList) {
		whiteList = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	if ((Z_TYPE_P(data) != IS_NULL)) {
		if ((Z_TYPE_P(data) != IS_ARRAY)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data passed to save() must be an array");
			return;
		}
		ZEPHIR_INIT_VAR(attributes);
		zephir_call_method_p1(attributes, metaData, "getcolumnmap", this_ptr);
		if ((Z_TYPE_P(attributes) == IS_ARRAY)) {
			ZEPHIR_INIT_NVAR(attributes);
			zephir_call_method_p1(attributes, metaData, "getattributes", this_ptr);
		}
		zephir_is_iterable(attributes, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(attribute, _2);
			ZEPHIR_OBS_NVAR(value);
			if (zephir_array_isset_fetch(&value, data, attribute, 0 TSRMLS_CC)) {
				if ((Z_TYPE_P(whiteList) == IS_ARRAY)) {
					if (!(zephir_fast_in_array(attribute, whiteList TSRMLS_CC))) {
						continue;
					}
				}
				ZEPHIR_INIT_NVAR(possibleSetter);
				ZEPHIR_CONCAT_SV(possibleSetter, "set", attribute);
				if ((zephir_method_exists(this_ptr, possibleSetter TSRMLS_CC)  == SUCCESS)) {
					zephir_call_method_zval_p1_noret(this_ptr, possibleSetter, value);
				} else {
					zephir_update_property_zval_zval(this_ptr, attribute, value TSRMLS_CC);
				}
			}
		}
	}
	ZEPHIR_INIT_VAR(writeConnection);
	zephir_call_method(writeConnection, this_ptr, "getwriteconnection");
	ZEPHIR_OBS_VAR(related);
	zephir_read_property_this(&related, this_ptr, SL("_related"), PH_NOISY_CC);
	if ((Z_TYPE_P(related) == IS_ARRAY)) {
		ZEPHIR_INIT_VAR(_3);
		zephir_call_method_p2(_3, this_ptr, "_presaverelatedrecords", writeConnection, related);
		if (ZEPHIR_IS_FALSE(_3)) {
			RETURN_MM_BOOL(0);
		}
	}
	ZEPHIR_INIT_VAR(schema);
	zephir_call_method(schema, this_ptr, "getschema");
	ZEPHIR_INIT_VAR(source);
	zephir_call_method(source, this_ptr, "getsource");
	if (zephir_is_true(schema)) {
		ZEPHIR_INIT_VAR(table);
		array_init_size(table, 3);
		zephir_array_fast_append(table, schema);
		zephir_array_fast_append(table, source);
	} else {
		ZEPHIR_CPY_WRT(table, source);
	}
	ZEPHIR_INIT_VAR(readConnection);
	zephir_call_method(readConnection, this_ptr, "getreadconnection");
	ZEPHIR_INIT_VAR(exists);
	zephir_call_method_p3(exists, this_ptr, "_exists", metaData, readConnection, table);
	if (zephir_is_true(exists)) {
		ZEPHIR_INIT_ZVAL_NREF(_4);
		ZVAL_LONG(_4, 2);
		zephir_update_property_this(this_ptr, SL("_operationMade"), _4 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_ZVAL_NREF(_4);
		ZVAL_LONG(_4, 1);
		zephir_update_property_this(this_ptr, SL("_operationMade"), _4 TSRMLS_CC);
	}
	ZEPHIR_INIT_NVAR(_3);
	array_init(_3);
	zephir_update_property_this(this_ptr, SL("_errorMessages"), _3 TSRMLS_CC);
	ZEPHIR_INIT_VAR(identityField);
	zephir_call_method_p1(identityField, metaData, "getidentityfield", this_ptr);
	ZEPHIR_INIT_VAR(_5);
	zephir_call_method_p3(_5, this_ptr, "_presave", metaData, exists, identityField);
	if (ZEPHIR_IS_FALSE(_5)) {
		if ((Z_TYPE_P(related) == IS_ARRAY)) {
			zephir_call_method_p1_noret(writeConnection, "rollback", ZEPHIR_GLOBAL(global_false));
		}
		if (ZEPHIR_GLOBAL(orm).exception_on_failed_save) {
			ZEPHIR_INIT_VAR(_6);
			object_init_ex(_6, phalcon_mvc_model_validationfailed_ce);
			_4 = zephir_fetch_nproperty_this(this_ptr, SL("_errorMessages"), PH_NOISY_CC);
			zephir_call_method_p2_noret(_6, "__construct", this_ptr, _4);
			zephir_throw_exception(_6 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_INIT_VAR(success);
	if (zephir_is_true(exists)) {
		zephir_call_method_p3(success, this_ptr, "_dolowupdate", metaData, writeConnection, table);
	} else {
		zephir_call_method_p4(success, this_ptr, "_dolowinsert", metaData, writeConnection, table, identityField);
	}
	if (zephir_is_true(success)) {
		ZEPHIR_INIT_ZVAL_NREF(_4);
		ZVAL_LONG(_4, 0);
		zephir_update_property_this(this_ptr, SL("_dirtyState"), _4 TSRMLS_CC);
	}
	if (ZEPHIR_GLOBAL(orm).events) {
		ZEPHIR_INIT_NVAR(_3);
		zephir_call_method_p2(_3, this_ptr, "_postsave", success, exists);
		ZEPHIR_CPY_WRT(success, _3);
	}
	if ((Z_TYPE_P(related) == IS_ARRAY)) {
		if (ZEPHIR_IS_FALSE(success)) {
			zephir_call_method_p1_noret(writeConnection, "rollback", ZEPHIR_GLOBAL(global_false));
			RETURN_MM_BOOL(0);
		}
		ZEPHIR_INIT_NVAR(_3);
		zephir_call_method_p2(_3, this_ptr, "_postsaverelatedrecords", writeConnection, related);
		if (ZEPHIR_IS_FALSE(_3)) {
			RETURN_MM_BOOL(0);
		}
	}
	RETURN_CCTOR(success);

}

/**
 * Inserts a model instance. If the instance already exists in the persistance it will throw an exception
 * Returning true on success or false otherwise.
 *
 *<code>
 *	//Creating a new robot
 *	$robot = new Robots();
 *	$robot->type = 'mechanical';
 *	$robot->name = 'Astro Boy';
 *	$robot->year = 1952;
 *	$robot->create();
 *
 *  //Passing an array to create
 *  $robot = new Robots();
 *  $robot->create(array(
 *      'type' => 'mechanical',
 *      'name' => 'Astroy Boy',
 *      'year' => 1952
 *  ));
 *</code>
 *
 * @param array data
 * @param array whiteList
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, create) {

	zval *_8;
	zend_function *_6 = NULL;
	HashTable *_2;
	HashPosition _1;
	zval *data = NULL, *whiteList = NULL, *metaData, *attribute = NULL, *possibleSetter = NULL, *value = NULL, *columnMap, *attributeField = NULL, *_0 = NULL, **_3, *_4 = NULL, *_5 = NULL, *_7, *_9, *_10;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &data, &whiteList);

	if (!data) {
		data = ZEPHIR_GLOBAL(global_null);
	}
	if (!whiteList) {
		whiteList = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	if ((Z_TYPE_P(data) != IS_NULL)) {
		if ((Z_TYPE_P(data) != IS_ARRAY)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data passed to create() must be an array");
			return;
		}
		ZEPHIR_INIT_VAR(columnMap);
		if (ZEPHIR_GLOBAL(orm).column_renaming) {
			zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
		} else {
			ZVAL_NULL(columnMap);
		}
		ZEPHIR_INIT_VAR(_0);
		zephir_call_method_p1(_0, metaData, "getattributes", this_ptr);
		zephir_is_iterable(_0, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HVALUE(attribute, _3);
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, attribute, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_4);
					object_init_ex(_4, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_5);
					ZEPHIR_CONCAT_SVS(_5, "Column '", attribute, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_4, "__construct", &_6, _5);
					zephir_throw_exception(_4 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, attribute);
			}
			ZEPHIR_OBS_NVAR(value);
			if (zephir_array_isset_fetch(&value, data, attributeField, 0 TSRMLS_CC)) {
				if ((Z_TYPE_P(whiteList) == IS_ARRAY)) {
					if (!(zephir_fast_in_array(attributeField, whiteList TSRMLS_CC))) {
						continue;
					}
				}
				ZEPHIR_INIT_NVAR(possibleSetter);
				ZEPHIR_CONCAT_SV(possibleSetter, "set", attributeField);
				if ((zephir_method_exists(this_ptr, possibleSetter TSRMLS_CC)  == SUCCESS)) {
					zephir_call_method_zval_p1_noret(this_ptr, possibleSetter, value);
				} else {
					zephir_update_property_zval_zval(this_ptr, attributeField, value TSRMLS_CC);
				}
			}
		}
	}
	ZEPHIR_INIT_NVAR(_0);
	ZEPHIR_INIT_VAR(_7);
	zephir_call_method(_7, this_ptr, "getreadconnection");
	zephir_call_method_p2(_0, this_ptr, "_exists", metaData, _7);
	if (zephir_is_true(_0)) {
		ZEPHIR_INIT_VAR(_8);
		array_init_size(_8, 2);
		ZEPHIR_INIT_LNVAR(_4);
		object_init_ex(_4, phalcon_mvc_model_message_ce);
		ZEPHIR_INIT_VAR(_9);
		ZVAL_STRING(_9, "Record cannot be created because it already exists", 1);
		ZEPHIR_INIT_VAR(_10);
		ZVAL_STRING(_10, "InvalidCreateAttempt", 1);
		zephir_call_method_p3_noret(_4, "__construct", _9, ZEPHIR_GLOBAL(global_null), _10);
		zephir_array_fast_append(_8, _4);
		zephir_update_property_this(this_ptr, SL("_errorMessages"), _8 TSRMLS_CC);
		RETURN_MM_BOOL(0);
	}
	zephir_call_method(return_value, this_ptr, "save");
	RETURN_MM();

}

/**
 * Updates a model instance. If the instance doesn't exist in the persistance it will throw an exception
 * Returning true on success or false otherwise.
 *
 *<code>
 *	//Updating a robot name
 *	$robot = Robots::findFirst("id=100");
 *	$robot->name = "Biomass";
 *	$robot->update();
 *</code>
 *
 * @param array data
 * @param array whiteList
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, update) {

	zval *_9;
	zend_function *_6 = NULL;
	HashTable *_2;
	HashPosition _1;
	zval *data = NULL, *whiteList = NULL, *metaData, *columnMap, *attribute = NULL, *attributeField = NULL, *possibleSetter = NULL, *value = NULL, *_0 = NULL, **_3, *_4 = NULL, *_5 = NULL, *_7, *_8, *_10, *_11;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &data, &whiteList);

	if (!data) {
		data = ZEPHIR_GLOBAL(global_null);
	}
	if (!whiteList) {
		whiteList = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(metaData);
	ZVAL_NULL(metaData);
	if ((Z_TYPE_P(data) != IS_NULL)) {
		if ((Z_TYPE_P(data) != IS_ARRAY)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Data passed to update() must be an array");
			return;
		}
		ZEPHIR_INIT_BNVAR(metaData);
		zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
		ZEPHIR_INIT_VAR(columnMap);
		if (ZEPHIR_GLOBAL(orm).column_renaming) {
			zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
		} else {
			ZVAL_NULL(columnMap);
		}
		ZEPHIR_INIT_VAR(_0);
		zephir_call_method_p1(_0, metaData, "getattributes", this_ptr);
		zephir_is_iterable(_0, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HVALUE(attribute, _3);
			if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
				ZEPHIR_OBS_NVAR(attributeField);
				if (!(zephir_array_isset_fetch(&attributeField, columnMap, attribute, 0 TSRMLS_CC))) {
					ZEPHIR_INIT_LNVAR(_4);
					object_init_ex(_4, phalcon_mvc_model_exception_ce);
					ZEPHIR_INIT_LNVAR(_5);
					ZEPHIR_CONCAT_SVS(_5, "Column '", attribute, "' isn't part of the column map");
					zephir_call_method_p1_cache_noret(_4, "__construct", &_6, _5);
					zephir_throw_exception(_4 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				ZEPHIR_CPY_WRT(attributeField, attribute);
			}
			ZEPHIR_OBS_NVAR(value);
			if (zephir_array_isset_fetch(&value, data, attributeField, 0 TSRMLS_CC)) {
				if ((Z_TYPE_P(whiteList) == IS_ARRAY)) {
					if (!(zephir_fast_in_array(attributeField, whiteList TSRMLS_CC))) {
						continue;
					}
				}
				ZEPHIR_INIT_NVAR(possibleSetter);
				ZEPHIR_CONCAT_SV(possibleSetter, "set", attributeField);
				if ((zephir_method_exists(this_ptr, possibleSetter TSRMLS_CC)  == SUCCESS)) {
					zephir_call_method_zval_p1_noret(this_ptr, possibleSetter, value);
				} else {
					zephir_update_property_zval_zval(this_ptr, attributeField, value TSRMLS_CC);
				}
			}
		}
	}
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("_dirtyState"), PH_NOISY_CC);
	if (zephir_is_true(_7)) {
		if ((Z_TYPE_P(metaData) == IS_NULL)) {
			ZEPHIR_INIT_BNVAR(metaData);
			zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
		}
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_INIT_VAR(_8);
		zephir_call_method(_8, this_ptr, "getreadconnection");
		zephir_call_method_p2(_0, this_ptr, "_exists", metaData, _8);
		if (zephir_is_true(_0)) {
			ZEPHIR_INIT_VAR(_9);
			array_init_size(_9, 2);
			ZEPHIR_INIT_LNVAR(_4);
			object_init_ex(_4, phalcon_mvc_model_message_ce);
			ZEPHIR_INIT_VAR(_10);
			ZVAL_STRING(_10, "Record cannot be updated because it does not exist", 1);
			ZEPHIR_INIT_VAR(_11);
			ZVAL_STRING(_11, "InvalidUpdateAttempt", 1);
			zephir_call_method_p3_noret(_4, "__construct", _10, ZEPHIR_GLOBAL(global_null), _11);
			zephir_array_fast_append(_9, _4);
			zephir_update_property_this(this_ptr, SL("_errorMessages"), _9 TSRMLS_CC);
			RETURN_MM_BOOL(0);
		}
	}
	zephir_call_method(return_value, this_ptr, "save");
	RETURN_MM();

}

/**
 * Deletes a model instance. Returning true on success or false otherwise.
 *
 * <code>
 *$robot = Robots::findFirst("id=100");
 *$robot->delete();
 *
 *foreach (Robots::find("type = 'mechanical'") as $robot) {
 *   $robot->delete();
 *}
 * </code>
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, delete) {

	zend_function *_8 = NULL, *_9 = NULL, *_10 = NULL, *_11 = NULL;
	HashTable *_4;
	HashPosition _3;
	zval *metaData, *writeConnection, *values, *bindTypes, *primaryKeys, *bindDataTypes, *columnMap, *attributeField = NULL, *conditions, *primaryKey = NULL, *bindType = NULL, *value = NULL, *schema, *source, *table = NULL, *success, *_0, *_1, *_2 = NULL, **_5, *_6 = NULL, *_7 = NULL, *_12 = NULL, *_13;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(writeConnection);
	zephir_call_method(writeConnection, this_ptr, "getwriteconnection");
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 3);
	zephir_update_property_this(this_ptr, SL("_operationMade"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	zephir_update_property_this(this_ptr, SL("_errorMessages"), _1 TSRMLS_CC);
	if (ZEPHIR_GLOBAL(orm).virtual_foreign_keys) {
		ZEPHIR_INIT_VAR(_2);
		zephir_call_method(_2, this_ptr, "_checkforeignkeysreverserestrict");
		if (ZEPHIR_IS_FALSE(_2)) {
			RETURN_MM_BOOL(0);
		}
	}
	ZEPHIR_INIT_VAR(values);
	array_init(values);
	ZEPHIR_INIT_VAR(bindTypes);
	array_init(bindTypes);
	ZEPHIR_INIT_VAR(conditions);
	array_init(conditions);
	ZEPHIR_INIT_VAR(primaryKeys);
	zephir_call_method_p1(primaryKeys, metaData, "getprimarykeyattributes", this_ptr);
	ZEPHIR_INIT_VAR(bindDataTypes);
	zephir_call_method_p1(bindDataTypes, metaData, "getbindtypes", this_ptr);
	ZEPHIR_INIT_VAR(columnMap);
	if (ZEPHIR_GLOBAL(orm).column_renaming) {
		zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
	} else {
		ZVAL_NULL(columnMap);
	}
	if (!(zephir_fast_count_int(primaryKeys TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "A primary key must be defined in the model in order to perform the operation");
		return;
	}
	zephir_is_iterable(primaryKeys, &_4, &_3, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
		; zend_hash_move_forward_ex(_4, &_3)
	) {
		ZEPHIR_GET_HVALUE(primaryKey, _5);
		ZEPHIR_OBS_NVAR(bindType);
		if (!(zephir_array_isset_fetch(&bindType, bindDataTypes, primaryKey, 0 TSRMLS_CC))) {
			ZEPHIR_INIT_LNVAR(_6);
			object_init_ex(_6, phalcon_mvc_model_exception_ce);
			ZEPHIR_INIT_LNVAR(_7);
			ZEPHIR_CONCAT_SVS(_7, "Column '", primaryKey, "' have not defined a bind data type");
			zephir_call_method_p1_cache_noret(_6, "__construct", &_8, _7);
			zephir_throw_exception(_6 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(attributeField);
			if (!(zephir_array_isset_fetch(&attributeField, columnMap, primaryKey, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_6);
				object_init_ex(_6, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_7);
				ZEPHIR_CONCAT_SVS(_7, "Column '", primaryKey, "' isn't part of the column map");
				zephir_call_method_p1_cache_noret(_6, "__construct", &_9, _7);
				zephir_throw_exception(_6 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			ZEPHIR_CPY_WRT(attributeField, primaryKey);
		}
		ZEPHIR_OBS_NVAR(value);
		if (!(zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC))) {
			ZEPHIR_INIT_LNVAR(_6);
			object_init_ex(_6, phalcon_mvc_model_exception_ce);
			ZEPHIR_INIT_LNVAR(_7);
			ZEPHIR_CONCAT_SVS(_7, "Cannot delete the record because the primary key attribute: '", attributeField, "' wasn't set");
			zephir_call_method_p1_cache_noret(_6, "__construct", &_10, _7);
			zephir_throw_exception(_6 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		zephir_array_append(&values, value, PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_2);
		zephir_call_method_p1_cache(_2, writeConnection, "escapeidentifier", &_11, primaryKey);
		ZEPHIR_INIT_LNVAR(_6);
		ZEPHIR_CONCAT_VS(_6, _2, " = ?");
		zephir_array_append(&conditions, _6, PH_SEPARATE);
		zephir_array_append(&bindTypes, bindType, PH_SEPARATE);
	}
	if (ZEPHIR_GLOBAL(orm).events) {
		zephir_update_property_this(this_ptr, SL("_skipped"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_2);
		ZEPHIR_INIT_VAR(_12);
		ZVAL_STRING(_12, "beforeDelete", 1);
		zephir_call_method_p1(_2, this_ptr, "fireeventcancel", _12);
		if (ZEPHIR_IS_FALSE(_2)) {
			RETURN_MM_BOOL(0);
		} else {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("_skipped"), PH_NOISY_CC);
			if (ZEPHIR_IS_TRUE(_0)) {
				RETURN_MM_BOOL(1);
			}
		}
	}
	ZEPHIR_INIT_VAR(schema);
	zephir_call_method(schema, this_ptr, "getschema");
	ZEPHIR_INIT_VAR(source);
	zephir_call_method(source, this_ptr, "getsource");
	if (zephir_is_true(schema)) {
		ZEPHIR_INIT_VAR(table);
		array_init_size(table, 3);
		zephir_array_fast_append(table, schema);
		zephir_array_fast_append(table, source);
	} else {
		ZEPHIR_CPY_WRT(table, source);
	}
	ZEPHIR_INIT_NVAR(_2);
	zephir_fast_join_str(_2, SL(" AND "), conditions TSRMLS_CC);
	ZEPHIR_INIT_VAR(success);
	zephir_call_method_p4(success, writeConnection, "delete", table, _2, values, bindTypes);
	if (ZEPHIR_GLOBAL(orm).virtual_foreign_keys) {
		ZEPHIR_INIT_NVAR(_12);
		zephir_call_method(_12, this_ptr, "_checkforeignkeysreversecascade");
		if (ZEPHIR_IS_FALSE(_12)) {
			RETURN_MM_BOOL(0);
		}
	}
	if (ZEPHIR_GLOBAL(orm).events) {
		if (zephir_is_true(success)) {
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_STRING(_2, "afterDelete", 1);
			zephir_call_method_p1_noret(this_ptr, "fireevent", _2);
		}
	}
	ZEPHIR_INIT_ZVAL_NREF(_13);
	ZVAL_LONG(_13, 2);
	zephir_update_property_this(this_ptr, SL("_dirtyState"), _13 TSRMLS_CC);
	RETURN_CCTOR(success);

}

/**
 * Returns the type of the latest operation performed by the ORM
 * Returns one of the OP_* class constants
 *
 * @return int
 */
PHP_METHOD(Phalcon_Mvc_Model, getOperationMade) {


	RETURN_MEMBER(this_ptr, "_operationMade");

}

/**
 * Refreshes the model attributes re-querying the record from the database
 */
PHP_METHOD(Phalcon_Mvc_Model, refresh) {

	zval *_5 = NULL;
	HashTable *_3;
	HashPosition _2;
	zval *metaData, *readConnection, *schema, *source, *table = NULL, *uniqueKey = NULL, *uniqueParams, *dialect, *row, *fields, *attribute = NULL, *_0, *_1 = NULL, **_4, *_6, *_7;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dirtyState"), PH_NOISY_CC);
	if (!ZEPHIR_IS_LONG(_0, 0)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The record cannot be refreshed because it does not exist or is deleted");
		return;
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(readConnection);
	zephir_call_method(readConnection, this_ptr, "getreadconnection");
	ZEPHIR_INIT_VAR(schema);
	zephir_call_method(schema, this_ptr, "getschema");
	ZEPHIR_INIT_VAR(source);
	zephir_call_method(source, this_ptr, "getsource");
	if (zephir_is_true(schema)) {
		ZEPHIR_INIT_VAR(table);
		array_init_size(table, 3);
		zephir_array_fast_append(table, schema);
		zephir_array_fast_append(table, source);
	} else {
		ZEPHIR_CPY_WRT(table, source);
	}
	ZEPHIR_OBS_VAR(uniqueKey);
	zephir_read_property_this(&uniqueKey, this_ptr, SL("_uniqueKey"), PH_NOISY_CC);
	if (!(zephir_is_true(uniqueKey))) {
		ZEPHIR_INIT_VAR(_1);
		zephir_call_method_p3(_1, this_ptr, "_exists", metaData, readConnection, table);
		if (zephir_is_true(_1)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The record cannot be refreshed because it does not exist or is deleted");
			return;
		}
		ZEPHIR_OBS_NVAR(uniqueKey);
		zephir_read_property_this(&uniqueKey, this_ptr, SL("_uniqueKey"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(uniqueParams);
	zephir_read_property_this(&uniqueParams, this_ptr, SL("_uniqueParams"), PH_NOISY_CC);
	if ((Z_TYPE_P(uniqueParams) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The record cannot be refreshed because it does not exist or is deleted");
		return;
	}
	ZEPHIR_INIT_VAR(fields);
	array_init(fields);
	ZEPHIR_INIT_NVAR(_1);
	zephir_call_method_p1(_1, metaData, "getattributes", this_ptr);
	zephir_is_iterable(_1, &_3, &_2, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
		; zend_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(attribute, _4);
		ZEPHIR_INIT_NVAR(_5);
		array_init_size(_5, 2);
		zephir_array_fast_append(_5, attribute);
		zephir_array_append(&fields, _5, PH_SEPARATE);
	}
	ZEPHIR_INIT_VAR(dialect);
	zephir_call_method(dialect, readConnection, "getdialect");
	ZEPHIR_INIT_NVAR(_1);
	ZEPHIR_INIT_NVAR(_5);
	array_init_size(_5, 5);
	zephir_array_update_string(&_5, SL("columns"), &fields, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(_6);
	zephir_call_method_p1(_6, readConnection, "escapeidentifier", table);
	zephir_array_update_string(&_5, SL("tables"), &_6, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_5, SL("where"), &uniqueKey, PH_COPY | PH_SEPARATE);
	zephir_call_method_p1(_1, dialect, "select", _5);
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("_uniqueTypes"), PH_NOISY_CC);
	ZEPHIR_INIT_BNVAR(_6);
	ZVAL_LONG(_6, 1);
	ZEPHIR_INIT_VAR(row);
	zephir_call_method_p4(row, readConnection, "fetchone", _1, _6, uniqueParams, _7);
	if ((Z_TYPE_P(row) == IS_ARRAY)) {
		ZEPHIR_INIT_BNVAR(_6);
		zephir_call_method_p1(_6, metaData, "getcolumnmap", this_ptr);
		zephir_call_method_p2_noret(this_ptr, "assign", row, _6);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Skips the current operation forcing a success state
 *
 * @param boolean skip
 */
PHP_METHOD(Phalcon_Mvc_Model, skipOperation) {

	zval *skip_param = NULL;
	zend_bool skip;

	zephir_fetch_params(0, 1, 0, &skip_param);

		skip = zephir_get_boolval(skip_param);


	zephir_update_property_this(this_ptr, SL("_skipped"), skip ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);

}

/**
 * Reads an attribute value by its name
 *
 * <code>
 * echo $robot->readAttribute('name');
 * </code>
 *
 * @param string attribute
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, readAttribute) {

	zval *attribute_param = NULL, *_0;
	zval *attribute = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &attribute_param);

	if (Z_TYPE_P(attribute_param) != IS_STRING && Z_TYPE_P(attribute_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'attribute' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(attribute_param) == IS_STRING) {
		attribute = attribute_param;
	} else {
		ZEPHIR_INIT_VAR(attribute);
		ZVAL_EMPTY_STRING(attribute);
	}


	if (zephir_isset_property_zval(this_ptr, attribute TSRMLS_CC)) {
		ZEPHIR_OBS_VAR(_0);
		zephir_read_property_zval(&_0, this_ptr, attribute, PH_NOISY_CC);
		RETURN_CCTOR(_0);
	}
	RETURN_MM_NULL();

}

/**
 * Writes an attribute value by its name
 *
 * <code>
 * 	$robot->writeAttribute('name', 'Rosey');
 * </code>
 *
 * @param string attribute
 * @param mixed value
 */
PHP_METHOD(Phalcon_Mvc_Model, writeAttribute) {

	zval *attribute_param = NULL, *value;
	zval *attribute = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &attribute_param, &value);

	if (Z_TYPE_P(attribute_param) != IS_STRING && Z_TYPE_P(attribute_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'attribute' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(attribute_param) == IS_STRING) {
		attribute = attribute_param;
	} else {
		ZEPHIR_INIT_VAR(attribute);
		ZVAL_EMPTY_STRING(attribute);
	}


	zephir_update_property_zval_zval(this_ptr, attribute, value TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets a list of attributes that must be skipped from the
 * generated INSERT/UPDATE statement
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->skipAttributes(array('price'));
 *   }
 *
 *}
 *</code>
 *
 * @param array attributes
 */
PHP_METHOD(Phalcon_Mvc_Model, skipAttributes) {

	HashTable *_1;
	HashPosition _0;
	zval *attributes, *keysAttributes, *metaData, *attribute = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &attributes);



	if ((Z_TYPE_P(attributes) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Attributes must be an array");
		return;
	}
	ZEPHIR_INIT_VAR(keysAttributes);
	array_init(keysAttributes);
	zephir_is_iterable(attributes, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(attribute, _2);
		zephir_array_update_zval(&keysAttributes, attribute, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	zephir_call_method_p2_noret(metaData, "setautomaticcreateattributes", this_ptr, keysAttributes);
	zephir_call_method_p2_noret(metaData, "setautomaticupdateattributes", this_ptr, keysAttributes);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets a list of attributes that must be skipped from the
 * generated INSERT statement
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->skipAttributesOnCreate(array('created_at'));
 *   }
 *
 *}
 *</code>
 *
 * @param array attributes
 */
PHP_METHOD(Phalcon_Mvc_Model, skipAttributesOnCreate) {

	HashTable *_1;
	HashPosition _0;
	zval *attributes, *keysAttributes, *metaData, *attribute = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &attributes);



	if ((Z_TYPE_P(attributes) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Attributes must be an array");
		return;
	}
	ZEPHIR_INIT_VAR(keysAttributes);
	array_init(keysAttributes);
	zephir_is_iterable(attributes, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(attribute, _2);
		zephir_array_update_zval(&keysAttributes, attribute, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	zephir_call_method_p2_noret(metaData, "setautomaticcreateattributes", this_ptr, keysAttributes);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets a list of attributes that must be skipped from the
 * generated UPDATE statement
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->skipAttributesOnUpdate(array('modified_in'));
 *   }
 *
 *}
 *</code>
 *
 * @param array attributes
 */
PHP_METHOD(Phalcon_Mvc_Model, skipAttributesOnUpdate) {

	HashTable *_1;
	HashPosition _0;
	zval *attributes, *keysAttributes, *metaData, *attribute = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &attributes);



	if ((Z_TYPE_P(attributes) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Attributes must be an array");
		return;
	}
	ZEPHIR_INIT_VAR(keysAttributes);
	array_init(keysAttributes);
	zephir_is_iterable(attributes, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(attribute, _2);
		zephir_array_update_zval(&keysAttributes, attribute, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	zephir_call_method_p2_noret(metaData, "setautomaticupdateattributes", this_ptr, keysAttributes);
	ZEPHIR_MM_RESTORE();

}

/**
 * Setup a 1-1 relation between two models
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->hasOne('id', 'RobotsDescription', 'robots_id');
 *   }
 *
 *}
 *</code>
 *
 * @param	mixed fields
 * @param	string referenceModel
 * @param	mixed referencedFields
 * @param   array options
 * @return  Phalcon\Mvc\Model\Relation
 */
PHP_METHOD(Phalcon_Mvc_Model, hasOne) {

	zval *referenceModel = NULL;
	zval *fields, *referenceModel_param = NULL, *referencedFields, *options = NULL, *manager;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &fields, &referenceModel_param, &referencedFields, &options);

	if (Z_TYPE_P(referenceModel_param) != IS_STRING && Z_TYPE_P(referenceModel_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'referenceModel' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(referenceModel_param) == IS_STRING) {
		referenceModel = referenceModel_param;
	} else {
		ZEPHIR_INIT_VAR(referenceModel);
		ZVAL_EMPTY_STRING(referenceModel);
	}
	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p5(return_value, manager, "addhasone", this_ptr, fields, referenceModel, referencedFields, options);
	RETURN_MM();

}

/**
 * Setup a relation reverse 1-1  between two models
 *
 *<code>
 *<?php
 *
 *class RobotsParts extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->belongsTo('robots_id', 'Robots', 'id');
 *   }
 *
 *}
 *</code>
 *
 * @param	mixed fields
 * @param	string referenceModel
 * @param	mixed referencedFields
 * @param   array options
 * @return  Phalcon\Mvc\Model\Relation
 */
PHP_METHOD(Phalcon_Mvc_Model, belongsTo) {

	zval *referenceModel = NULL;
	zval *fields, *referenceModel_param = NULL, *referencedFields, *options = NULL, *manager;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &fields, &referenceModel_param, &referencedFields, &options);

	if (Z_TYPE_P(referenceModel_param) != IS_STRING && Z_TYPE_P(referenceModel_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'referenceModel' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(referenceModel_param) == IS_STRING) {
		referenceModel = referenceModel_param;
	} else {
		ZEPHIR_INIT_VAR(referenceModel);
		ZVAL_EMPTY_STRING(referenceModel);
	}
	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p5(return_value, manager, "addbelongsto", this_ptr, fields, referenceModel, referencedFields, options);
	RETURN_MM();

}

/**
 * Setup a relation 1-n between two models
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       $this->hasMany('id', 'RobotsParts', 'robots_id');
 *   }
 *
 *}
 *</code>
 *
 * @param	mixed fields
 * @param	string referenceModel
 * @param	mixed referencedFields
 * @param   array options
 * @return  Phalcon\Mvc\Model\Relation
 */
PHP_METHOD(Phalcon_Mvc_Model, hasMany) {

	zval *referenceModel = NULL;
	zval *fields, *referenceModel_param = NULL, *referencedFields, *options = NULL, *manager;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &fields, &referenceModel_param, &referencedFields, &options);

	if (Z_TYPE_P(referenceModel_param) != IS_STRING && Z_TYPE_P(referenceModel_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'referenceModel' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(referenceModel_param) == IS_STRING) {
		referenceModel = referenceModel_param;
	} else {
		ZEPHIR_INIT_VAR(referenceModel);
		ZVAL_EMPTY_STRING(referenceModel);
	}
	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p5(return_value, manager, "addhasmany", this_ptr, fields, referenceModel, referencedFields, options);
	RETURN_MM();

}

/**
 * Setup a relation n-n between two models through an intermediate relation
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *       //Setup a many-to-many relation to Parts through RobotsParts
 *       $this->hasManyToMany(
 *			'id',
 *			'RobotsParts',
 *			'robots_id',
 *			'parts_id',
 *			'Parts',
 *			'id'
 *		);
 *   }
 *
 *}
 *</code>
 *
 * @param	string|array fields
 * @param	string intermediateModel
 * @param	string|array intermediateFields
 * @param	string|array intermediateReferencedFields
 * @param	string referencedModel
 * @param   string|array referencedFields
 * @param   array options
 * @return  Phalcon\Mvc\Model\Relation
 */
PHP_METHOD(Phalcon_Mvc_Model, hasManyToMany) {

	zval *intermediateModel = NULL, *referenceModel = NULL;
	zval *fields, *intermediateModel_param = NULL, *intermediateFields, *intermediateReferencedFields, *referenceModel_param = NULL, *referencedFields, *options = NULL, *manager;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 6, 1, &fields, &intermediateModel_param, &intermediateFields, &intermediateReferencedFields, &referenceModel_param, &referencedFields, &options);

	if (Z_TYPE_P(intermediateModel_param) != IS_STRING && Z_TYPE_P(intermediateModel_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'intermediateModel' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(intermediateModel_param) == IS_STRING) {
		intermediateModel = intermediateModel_param;
	} else {
		ZEPHIR_INIT_VAR(intermediateModel);
		ZVAL_EMPTY_STRING(intermediateModel);
	}
		zephir_get_strval(referenceModel, referenceModel_param);
	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p8(return_value, manager, "addhasmanytomany", this_ptr, fields, intermediateModel, intermediateFields, intermediateReferencedFields, referenceModel, referencedFields, options);
	RETURN_MM();

}

/**
 * Setups a behavior in a model
 *
 *<code>
 *<?php
 *
 *use Phalcon\Mvc\Model\Behavior\Timestampable;
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *		$this->addBehavior(new Timestampable(array(
 *			'onCreate' => array(
 *				'field' => 'created_at',
 *				'format' => 'Y-m-d'
 *			)
 *		)));
 *   }
 *
 *}
 *</code>
 *
 * @param Phalcon\Mvc\Model\BehaviorInterface behavior
 */
PHP_METHOD(Phalcon_Mvc_Model, addBehavior) {

	zval *behavior, *manager;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &behavior);



	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(manager, "addbehavior", this_ptr, behavior);
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets if the model must keep the original record snapshot in memory
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *		$this->keepSnapshots(true);
 *   }
 *
 *}
 *</code>
 *
 * @param boolean keepSnapshots
 */
PHP_METHOD(Phalcon_Mvc_Model, keepSnapshots) {

	zval *keepSnapshot_param = NULL, *manager;
	zend_bool keepSnapshot;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &keepSnapshot_param);

		keepSnapshot = zephir_get_boolval(keepSnapshot_param);


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(manager, "keepsnapshots", this_ptr, (keepSnapshot ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
	ZEPHIR_MM_RESTORE();

}

/**
 * Sets the record's snapshot data.
 * This method is used internally to set snapshot data when the model was set up to keep snapshot data
 *
 * @param array data
 * @param array columnMap
 */
PHP_METHOD(Phalcon_Mvc_Model, setSnapshotData) {

	zend_function *_5 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *data, *columnMap = NULL, *key = NULL, *value = NULL, *snapshot, *attribute = NULL, **_2, *_3 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &data, &columnMap);

	if (!columnMap) {
		columnMap = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(data) == IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The snapshot data must be an array");
		return;
	}
	if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
		ZEPHIR_INIT_VAR(snapshot);
		array_init(snapshot);
		zephir_is_iterable(data, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(key, _1, _0);
			ZEPHIR_GET_HVALUE(value, _2);
			if ((Z_TYPE_P(key) != IS_STRING)) {
				continue;
			}
			ZEPHIR_OBS_NVAR(attribute);
			if (!(zephir_array_isset_fetch(&attribute, columnMap, key, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_3);
				object_init_ex(_3, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_4);
				ZEPHIR_CONCAT_SVS(_4, "Column '", key, "' doesn't make part of the column map");
				zephir_call_method_p1_cache_noret(_3, "__construct", &_5, _4);
				zephir_throw_exception(_3 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_array_update_zval(&snapshot, attribute, &value, PH_COPY | PH_SEPARATE);
		}
		zephir_update_property_this(this_ptr, SL("_snapshot"), snapshot TSRMLS_CC);
		RETURN_MM_NULL();
	}
	zephir_update_property_this(this_ptr, SL("_snapshot"), data TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Checks if the object has internal snapshot data
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model, hasSnapshotData) {

	zval *snapshot;


	snapshot = zephir_fetch_nproperty_this(this_ptr, SL("_snapshot"), PH_NOISY_CC);
	if ((Z_TYPE_P(snapshot) == IS_ARRAY)) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);

}

/**
 * Returns the internal snapshot data
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model, getSnapshotData) {


	RETURN_MEMBER(this_ptr, "_snapshot");

}

/**
 * Check if a specific attribute has changed
 * This only works if the model is keeping data snapshots
 *
 * @param string|array fieldName
 */
PHP_METHOD(Phalcon_Mvc_Model, hasChanged) {

	HashTable *_5;
	HashPosition _4;
	zval *fieldName = NULL, *snapshot, *metaData, *columnMap, *allAttributes = NULL, *value = NULL, *originalValue = NULL, *name = NULL, *type = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, **_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &fieldName);

	if (!fieldName) {
		fieldName = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(snapshot);
	zephir_read_property_this(&snapshot, this_ptr, SL("_snapshot"), PH_NOISY_CC);
	if ((Z_TYPE_P(snapshot) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The record doesn't have a valid data snapshot");
		return;
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dirtyState"), PH_NOISY_CC);
	if (!ZEPHIR_IS_LONG(_0, 0)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Change checking cannot be performed because the object has not been persisted or is deleted");
		return;
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(columnMap);
	zephir_call_method_p1(columnMap, metaData, "getreversecolumnmap", this_ptr);
	if ((Z_TYPE_P(columnMap) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(allAttributes);
		zephir_call_method_p1(allAttributes, metaData, "getdatatypes", this_ptr);
	} else {
		ZEPHIR_CPY_WRT(allAttributes, columnMap);
	}
	if ((Z_TYPE_P(fieldName) == IS_ARRAY)) {
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			if (!(zephir_array_isset(columnMap, fieldName))) {
				ZEPHIR_INIT_VAR(_1);
				object_init_ex(_1, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_VAR(_2);
				ZEPHIR_CONCAT_SVS(_2, "The field '", fieldName, "' is not part of the model");
				zephir_call_method_p1_noret(_1, "__construct", _2);
				zephir_throw_exception(_1 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			if (!(zephir_array_isset(allAttributes, fieldName))) {
				ZEPHIR_INIT_LNVAR(_1);
				object_init_ex(_1, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_2);
				ZEPHIR_CONCAT_SVS(_2, "The field '", fieldName, "' is not part of the model");
				zephir_call_method_p1_noret(_1, "__construct", _2);
				zephir_throw_exception(_1 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		}
		ZEPHIR_OBS_VAR(value);
		if (!(zephir_read_property_zval(&value, this_ptr, fieldName, PH_SILENT_CC))) {
			ZEPHIR_INIT_LNVAR(_1);
			object_init_ex(_1, phalcon_mvc_model_exception_ce);
			ZEPHIR_INIT_LNVAR(_2);
			ZEPHIR_CONCAT_SVS(_2, "The field '", fieldName, "' is not defined on the model");
			zephir_call_method_p1_noret(_1, "__construct", _2);
			zephir_throw_exception(_1 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_OBS_VAR(originalValue);
		if (!(zephir_array_isset_fetch(&originalValue, snapshot, fieldName, 0 TSRMLS_CC))) {
			ZEPHIR_INIT_LNVAR(_1);
			object_init_ex(_1, phalcon_mvc_model_exception_ce);
			ZEPHIR_INIT_VAR(_3);
			ZEPHIR_CONCAT_SVS(_3, "The field '", fieldName, "' was not found in the snapshot");
			zephir_call_method_p1_noret(_1, "__construct", _3);
			zephir_throw_exception(_1 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		RETURN_MM_BOOL(!ZEPHIR_IS_EQUAL(value, originalValue));
	}
	zephir_is_iterable(allAttributes, &_5, &_4, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		; zend_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HMKEY(name, _5, _4);
		ZEPHIR_GET_HVALUE(type, _6);
		ZEPHIR_OBS_NVAR(originalValue);
		if (!(zephir_array_isset_fetch(&originalValue, snapshot, name, 0 TSRMLS_CC))) {
			RETURN_MM_BOOL(1);
		}
		ZEPHIR_OBS_NVAR(value);
		if (!(zephir_read_property_zval(&value, this_ptr, name, PH_SILENT_CC))) {
			RETURN_MM_BOOL(1);
		}
		if (!ZEPHIR_IS_EQUAL(value, originalValue)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}

/**
 * Returns a list of changed values
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model, getChangedFields) {

	HashTable *_2;
	HashPosition _1;
	zval *metaData, *changed, *name = NULL, *type = NULL, *snapshot, *columnMap, *allAttributes = NULL, *value = NULL, *_0, **_3, *_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(snapshot);
	zephir_read_property_this(&snapshot, this_ptr, SL("_snapshot"), PH_NOISY_CC);
	if ((Z_TYPE_P(snapshot) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The record doesn't have a valid data snapshot");
		return;
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dirtyState"), PH_NOISY_CC);
	if (!ZEPHIR_IS_LONG(_0, 0)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Change checking cannot be performed because the object has not been persisted or is deleted");
		return;
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(columnMap);
	zephir_call_method_p1(columnMap, metaData, "getreversecolumnmap", this_ptr);
	if ((Z_TYPE_P(columnMap) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(allAttributes);
		zephir_call_method_p1(allAttributes, metaData, "getdatatypes", this_ptr);
	} else {
		ZEPHIR_CPY_WRT(allAttributes, columnMap);
	}
	ZEPHIR_INIT_VAR(changed);
	array_init(changed);
	zephir_is_iterable(allAttributes, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(name, _2, _1);
		ZEPHIR_GET_HVALUE(type, _3);
		if (!(zephir_array_isset(snapshot, name))) {
			zephir_array_append(&changed, name, PH_SEPARATE);
			continue;
		}
		ZEPHIR_OBS_NVAR(value);
		if (!(zephir_read_property_zval(&value, this_ptr, name, PH_SILENT_CC))) {
			zephir_array_append(&changed, name, PH_SEPARATE);
			continue;
		}
		zephir_array_fetch(&_4, snapshot, name, PH_NOISY | PH_READONLY TSRMLS_CC);
		if (!ZEPHIR_IS_EQUAL(value, _4)) {
			zephir_array_append(&changed, name, PH_SEPARATE);
			continue;
		}
	}
	RETURN_CCTOR(changed);

}

/**
 * Sets if a model must use dynamic update instead of the all-field update
 *
 *<code>
 *<?php
 *
 *class Robots extends \Phalcon\Mvc\Model
 *{
 *
 *   public function initialize()
 *   {
 *		$this->useDynamicUpdate(true);
 *   }
 *
 *}
 *</code>
 *
 * @param boolean dynamicUpdate
 */
PHP_METHOD(Phalcon_Mvc_Model, useDynamicUpdate) {

	zval *dynamicUpdate_param = NULL, *manager;
	zend_bool dynamicUpdate;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &dynamicUpdate_param);

		dynamicUpdate = zephir_get_boolval(dynamicUpdate_param);


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	zephir_call_method_p2_noret(manager, "usedynamicupdate", this_ptr, (dynamicUpdate ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns related records based on defined relations
 *
 * @param string alias
 * @param array arguments
 * @return Phalcon\Mvc\Model\ResultsetInterface
 */
PHP_METHOD(Phalcon_Mvc_Model, getRelated) {

	zval *_2, *_4;
	zval *alias_param = NULL, *arguments = NULL, *manager, *relation, *className, *_0, *_1, *_3;
	zval *alias = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &alias_param, &arguments);

		zephir_get_strval(alias, alias_param);
	if (!arguments) {
		arguments = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(relation);
	zephir_call_method_p2(relation, manager, "getrelationbyalias", className, alias);
	if ((Z_TYPE_P(relation) != IS_OBJECT)) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, phalcon_mvc_model_exception_ce);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SVSVS(_1, "There is no defined relations for the model '", className, "' using alias '", alias, "'");
		zephir_call_method_p1_noret(_0, "__construct", _1);
		zephir_throw_exception(_0 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 3);
	zephir_array_fast_append(_2, manager);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "getRelationRecords", 1);
	zephir_array_fast_append(_2, _3);
	ZEPHIR_INIT_VAR(_4);
	array_init_size(_4, 7);
	zephir_array_fast_append(_4, relation);
	zephir_array_fast_append(_4, ZEPHIR_GLOBAL(global_null));
	zephir_array_fast_append(_4, this_ptr);
	zephir_array_fast_append(_4, arguments);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, _2, _4);
	RETURN_MM();

}

/**
 * Returns related records defined relations depending on the method name
 *
 * @param string modelName
 * @param string method
 * @param array arguments
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, _getRelatedRecords) {

	zval *_2, *_3;
	zval *modelName_param = NULL, *method_param = NULL, *arguments, *manager, *relation, *queryMethod, *extraArgs, *_0 = NULL, *_1;
	zval *modelName = NULL, *method = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &modelName_param, &method_param, &arguments);

		zephir_get_strval(modelName, modelName_param);
		zephir_get_strval(method, method_param);


	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(relation);
	ZVAL_BOOL(relation, 0);
	ZEPHIR_INIT_VAR(queryMethod);
	ZVAL_NULL(queryMethod);
	if (zephir_start_with_str(method, SL("get"))) {
		ZEPHIR_INIT_VAR(_0);
		zephir_substr(_0, method, 3 , 0 );
		ZEPHIR_INIT_BNVAR(relation);
		zephir_call_method_p2(relation, manager, "getrelationbyalias", modelName, _0);
	}
	if ((Z_TYPE_P(relation) != IS_OBJECT)) {
		if (zephir_start_with_str(method, SL("count"))) {
			ZEPHIR_INIT_BNVAR(queryMethod);
			ZVAL_STRING(queryMethod, "count", 1);
			ZEPHIR_INIT_NVAR(_0);
			zephir_substr(_0, method, 5 , 0 );
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "count", 1);
			ZEPHIR_INIT_BNVAR(relation);
			zephir_call_method_p2(relation, manager, "getrelationbyalias", _1, _0);
		}
	}
	if ((Z_TYPE_P(relation) == IS_OBJECT)) {
		ZEPHIR_OBS_VAR(extraArgs);
		ZEPHIR_INIT_VAR(_2);
		array_init_size(_2, 3);
		zephir_array_fast_append(_2, manager);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "getRelationRecords", 1);
		zephir_array_fast_append(_2, _0);
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 7);
		zephir_array_fast_append(_3, relation);
		zephir_array_fast_append(_3, queryMethod);
		zephir_array_fast_append(_3, this_ptr);
		zephir_array_fast_append(_3, extraArgs);
		ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, _2, _3);
		RETURN_MM();
	}
	RETURN_MM_NULL();

}

/**
 * Handles method calls when a method is not implemented
 *
 * @param	string method
 * @param	array arguments
 * @return	mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, __call) {

	zval *method_param = NULL, *arguments = NULL, *modelName, *modelsManager, *status, *records, *_0, *_1;
	zval *method = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &method_param, &arguments);

		zephir_get_strval(method, method_param);
	if (!arguments) {
		arguments = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(modelName);
	zephir_get_class(modelName, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(records);
	zephir_call_method_p3(records, this_ptr, "_getrelatedrecords", modelName, method, arguments);
	if ((Z_TYPE_P(records) != IS_NULL)) {
		RETURN_CCTOR(records);
	}
	ZEPHIR_OBS_VAR(modelsManager);
	zephir_read_property_this(&modelsManager, this_ptr, SL("_modelsManager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(status);
	zephir_call_method_p3(status, modelsManager, "missingmethod", this_ptr, method, arguments);
	if ((Z_TYPE_P(status) != IS_NULL)) {
		RETURN_CCTOR(status);
	}
	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, phalcon_mvc_model_exception_ce);
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_CONCAT_SVSVS(_1, "The method '", method, "' doesn't exist on model '", modelName, "'");
	zephir_call_method_p1_noret(_0, "__construct", _1);
	zephir_throw_exception(_0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

/**
 * Handles method calls when a static method is not implemented
 *
 * @param	string method
 * @param	array arguments
 * @return	mixed
 */
PHP_METHOD(Phalcon_Mvc_Model, __callStatic) {

	zend_class_entry *_3;
	zval *method_param = NULL, *arguments = NULL, *extraMethod, *type = NULL, *modelName, *value, *model, *attributes, *field = NULL, *extraMethodFirst, *metaData, *_0 = NULL, *_1 = NULL;
	zval *method = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &method_param, &arguments);

		zephir_get_strval(method, method_param);
	if (!arguments) {
		arguments = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(extraMethod);
	ZVAL_NULL(extraMethod);
	if (zephir_start_with_str(method, SL("findFirstBy"))) {
		ZEPHIR_INIT_VAR(type);
		ZVAL_STRING(type, "findFirst", 1);
		ZEPHIR_INIT_BNVAR(extraMethod);
		zephir_substr(extraMethod, method, 11 , 0 );
	}
	if ((Z_TYPE_P(extraMethod) == IS_NULL)) {
		if (zephir_start_with_str(method, SL("findBy"))) {
			ZEPHIR_INIT_NVAR(type);
			ZVAL_STRING(type, "find", 1);
			ZEPHIR_INIT_BNVAR(extraMethod);
			zephir_substr(extraMethod, method, 6 , 0 );
		}
	}
	if ((Z_TYPE_P(extraMethod) == IS_NULL)) {
		if (zephir_start_with_str(method, SL("countBy"))) {
			ZEPHIR_INIT_NVAR(type);
			ZVAL_STRING(type, "count", 1);
			ZEPHIR_INIT_BNVAR(extraMethod);
			zephir_substr(extraMethod, method, 7 , 0 );
		}
	}
	ZEPHIR_INIT_VAR(modelName);
	zephir_call_func(modelName, "get_called_class");
	if (zephir_is_true(extraMethod)) {
		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, phalcon_mvc_model_exception_ce);
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_CONCAT_SVSVS(_1, "The static method '", method, "' doesn't exist on model '", modelName, "'");
		zephir_call_method_p1_noret(_0, "__construct", _1);
		zephir_throw_exception(_0 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_OBS_VAR(value);
	if (!(zephir_array_isset_long_fetch(&value, arguments, 0, 0 TSRMLS_CC))) {
		ZEPHIR_INIT_LNVAR(_0);
		object_init_ex(_0, phalcon_mvc_model_exception_ce);
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_SVS(_2, "The static method '", method, "' requires one argument");
		zephir_call_method_p1_noret(_0, "__construct", _2);
		zephir_throw_exception(_0 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(model);
	_3 = zend_fetch_class(Z_STRVAL_P(modelName), Z_STRLEN_P(modelName), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(model, _3);
	if (zephir_has_constructor(model TSRMLS_CC)) {
		zephir_call_method_noret(model, "__construct");
	}
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, model, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(attributes);
	zephir_call_method_p1(attributes, metaData, "getreversecolumnmap", model);
	if ((Z_TYPE_P(attributes) != IS_ARRAY)) {
		ZEPHIR_INIT_BNVAR(attributes);
		zephir_call_method_p1(attributes, metaData, "getdatatypes", model);
	}
	if (zephir_array_isset(attributes, extraMethod)) {
		ZEPHIR_CPY_WRT(field, extraMethod);
	} else {
		ZEPHIR_INIT_VAR(extraMethodFirst);
		zephir_call_func_p1(extraMethodFirst, "lcfirst", extraMethod);
		if (zephir_array_isset(attributes, extraMethodFirst)) {
			ZEPHIR_CPY_WRT(field, extraMethodFirst);
		} else {
			ZEPHIR_INIT_VAR(field);
			zephir_uncamelize(field, extraMethod);
			if (!(zephir_array_isset(attributes, field))) {
				ZEPHIR_INIT_LNVAR(_0);
				object_init_ex(_0, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_1);
				ZEPHIR_CONCAT_SVS(_1, "Cannot resolve attribute '", extraMethod, "' in the model");
				zephir_call_method_p1_noret(_0, "__construct", _1);
				zephir_throw_exception(_0 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		}
	}
	RETURN_MM_NULL();

}

/**
 * Magic method to assign values to the the model
 *
 * @param string property
 * @param mixed value
 */
PHP_METHOD(Phalcon_Mvc_Model, __set) {

	zval *property_param = NULL, *value, *lowerProperty = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &property_param, &value);

		zephir_get_strval(property, property_param);


	if ((Z_TYPE_P(value) == IS_OBJECT)) {
		if (zephir_is_instance_of(value, SL("Phalcon\\Mvc\\ModelInterface") TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(lowerProperty);
			zephir_fast_strtolower(lowerProperty, property);
			zephir_update_property_zval_zval(this_ptr, lowerProperty, value TSRMLS_CC);
			zephir_update_property_array(this_ptr, SL("_related"), lowerProperty, value TSRMLS_CC);
			ZEPHIR_INIT_ZVAL_NREF(_0);
			ZVAL_LONG(_0, 1);
			zephir_update_property_this(this_ptr, SL("_dirtyState"), _0 TSRMLS_CC);
			RETURN_CCTOR(value);
		}
	}
	if ((Z_TYPE_P(value) == IS_ARRAY)) {
		ZEPHIR_INIT_NVAR(lowerProperty);
		zephir_fast_strtolower(lowerProperty, property);
		zephir_update_property_array(this_ptr, SL("_related"), lowerProperty, value TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_0);
		ZVAL_LONG(_0, 1);
		zephir_update_property_this(this_ptr, SL("_dirtyState"), _0 TSRMLS_CC);
		RETURN_CCTOR(value);
	}
	zephir_update_property_zval_zval(this_ptr, property, value TSRMLS_CC);
	RETURN_CCTOR(value);

}

/**
 * Magic method to get related records using the relation alias as a property
 *
 * @param string property
 * @return Phalcon\Mvc\Model\Resultset|Phalcon\Mvc\Model
 */
PHP_METHOD(Phalcon_Mvc_Model, __get) {

	zval *_0, *_2;
	zval *property_param = NULL, *modelName, *manager, *lowerProperty, *relation, *result, *_1, *_3;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(modelName);
	zephir_get_class(modelName, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(manager);
	zephir_call_method(manager, this_ptr, "getmodelsmanager");
	ZEPHIR_INIT_VAR(lowerProperty);
	zephir_fast_strtolower(lowerProperty, property);
	ZEPHIR_INIT_VAR(relation);
	zephir_call_method_p2(relation, manager, "getrelationbyalias", modelName, lowerProperty);
	if ((Z_TYPE_P(relation) == IS_OBJECT)) {
		ZEPHIR_INIT_VAR(result);
		ZEPHIR_INIT_VAR(_0);
		array_init_size(_0, 3);
		zephir_array_fast_append(_0, manager);
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "getRelationRecords", 1);
		zephir_array_fast_append(_0, _1);
		ZEPHIR_INIT_VAR(_2);
		array_init_size(_2, 7);
		zephir_array_fast_append(_2, relation);
		zephir_array_fast_append(_2, ZEPHIR_GLOBAL(global_null));
		zephir_array_fast_append(_2, this_ptr);
		zephir_array_fast_append(_2, ZEPHIR_GLOBAL(global_null));
		ZEPHIR_CALL_USER_FUNC_ARRAY(result, _0, _2);
		if ((Z_TYPE_P(result) == IS_OBJECT)) {
			zephir_update_property_zval_zval(this_ptr, lowerProperty, result TSRMLS_CC);
			if (zephir_is_instance_of(result, SL("Phalcon\\Mvc\\ModelInterface") TSRMLS_CC)) {
				zephir_update_property_array(this_ptr, SL("_related"), lowerProperty, result TSRMLS_CC);
			}
		}
		RETURN_CCTOR(result);
	}
	ZEPHIR_INIT_VAR(_3);
	ZEPHIR_CONCAT_SVSV(_3, "Access to undefined property ", modelName, "::", property);
	zephir_call_func_p1_noret("trigger_error", _3);
	RETURN_MM_NULL();

}

/**
 * Magic method to check if a property is a valid relation
 *
 * @param string property
 */
PHP_METHOD(Phalcon_Mvc_Model, __isset) {

	zval *property_param = NULL, *modelName, *manager, *relation;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(modelName);
	zephir_get_class(modelName, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(manager);
	zephir_call_method(manager, this_ptr, "getmodelsmanager");
	ZEPHIR_INIT_VAR(relation);
	zephir_call_method_p2(relation, manager, "getrelationbyalias", modelName, property);
	if ((Z_TYPE_P(relation) == IS_OBJECT)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

/**
 * Serializes the object ignoring connections, services, related objects or static properties
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model, serialize) {

	HashTable *_2;
	HashPosition _1;
	zval *data, *metaData, *value = NULL, *attribute = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p1(_0, metaData, "getattributes", this_ptr);
	zephir_is_iterable(_0, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(attribute, _3);
		ZEPHIR_OBS_NVAR(value);
		if (zephir_read_property_zval(&value, this_ptr, attribute, PH_SILENT_CC)) {
			zephir_array_update_zval(&data, attribute, &value, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_update_zval(&data, attribute, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		}
	}
	zephir_call_func_p1(return_value, "serialize", data);
	RETURN_MM();

}

/**
 * Unserializes the object from a serialized string
 *
 * @param string data
 */
PHP_METHOD(Phalcon_Mvc_Model, unserialize) {

	HashTable *_2;
	HashPosition _1;
	zval *data, *attributes, *dependencyInjector, *manager, *key = NULL, *value = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &data);



	if ((Z_TYPE_P(data) == IS_STRING)) {
		ZEPHIR_INIT_VAR(attributes);
		zephir_call_func_p1(attributes, "unserialize", data);
		if ((Z_TYPE_P(attributes) == IS_ARRAY)) {
			ZEPHIR_INIT_VAR(dependencyInjector);
			zephir_call_static(dependencyInjector, "Phalcon\\Di", "getdefault");
			if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "A dependency injector container is required to obtain the services related to the ORM");
				return;
			}
			zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);
			ZEPHIR_INIT_VAR(_0);
			ZVAL_STRING(_0, "modelsManager", 1);
			ZEPHIR_INIT_VAR(manager);
			zephir_call_method_p1(manager, dependencyInjector, "getshared", _0);
			if ((Z_TYPE_P(manager) != IS_OBJECT)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The injected service 'modelsManager' is not valid");
				return;
			}
			zephir_update_property_this(this_ptr, SL("_modelsManager"), manager TSRMLS_CC);
			zephir_call_method_p1_noret(manager, "initialize", this_ptr);
			zephir_is_iterable(attributes, &_2, &_1, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
				; zend_hash_move_forward_ex(_2, &_1)
			) {
				ZEPHIR_GET_HMKEY(key, _2, _1);
				ZEPHIR_GET_HVALUE(value, _3);
				zephir_update_property_zval_zval(this_ptr, key, value TSRMLS_CC);
			}
			RETURN_MM_NULL();
		}
	}
	ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Invalid serialization data");
	return;

}

/**
 * Returns a simple representation of the object that can be used with var_dump
 *
 *<code>
 * var_dump($robot->dump());
 *</code>
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model, dump) {

	ZEPHIR_MM_GROW();

	zephir_call_func_p1(return_value, "get_object_vars", this_ptr);
	RETURN_MM();

}

/**
 * Returns the instance as an array representation
 *
 *<code>
 * print_r($robot->toArray());
 *</code>
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model, toArray) {

	zend_function *_6 = NULL;
	HashTable *_2;
	HashPosition _1;
	zval *data, *metaData, *columnMap, *attribute = NULL, *attributeField = NULL, *value = NULL, *_0, **_3, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method(metaData, this_ptr, "getmodelsmetadata");
	ZEPHIR_INIT_VAR(columnMap);
	zephir_call_method_p1(columnMap, metaData, "getcolumnmap", this_ptr);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p1(_0, metaData, "getattributes", this_ptr);
	zephir_is_iterable(_0, &_2, &_1, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		; zend_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HVALUE(attribute, _3);
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(attributeField);
			if (!(zephir_array_isset_fetch(&attributeField, columnMap, attribute, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_LNVAR(_4);
				object_init_ex(_4, phalcon_mvc_model_exception_ce);
				ZEPHIR_INIT_LNVAR(_5);
				ZEPHIR_CONCAT_SVS(_5, "Column '", attribute, "' doesn't make part of the column map");
				zephir_call_method_p1_cache_noret(_4, "__construct", &_6, _5);
				zephir_throw_exception(_4 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			ZEPHIR_CPY_WRT(attributeField, attribute);
		}
		ZEPHIR_OBS_NVAR(value);
		if (zephir_read_property_zval(&value, this_ptr, attributeField, PH_SILENT_CC)) {
			zephir_array_update_zval(&data, attributeField, &value, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_update_zval(&data, attributeField, &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CCTOR(data);

}

/**
 * Enables/disables options in the ORM
 *
 * @param array options
 */
PHP_METHOD(Phalcon_Mvc_Model, setup) {

	zval *options, *disableEvents, *columnRenaming, *notNullValidations, *exceptionOnFailedSave, *phqlLiterals, *virtualForeignKeys, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &options);



	if ((Z_TYPE_P(options) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Options must be an array");
		return;
	}
	ZEPHIR_OBS_VAR(disableEvents);
	if (zephir_array_isset_string_fetch(&disableEvents, options, SS("events"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "orm.events", 1);
		zephir_call_func_p2_noret("globals_set", _0, disableEvents);
	}
	ZEPHIR_OBS_VAR(virtualForeignKeys);
	if (zephir_array_isset_string_fetch(&virtualForeignKeys, options, SS("virtualForeignKeys"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "orm.virtual_foreign_keys", 1);
		zephir_call_func_p2_noret("globals_set", _0, virtualForeignKeys);
	}
	ZEPHIR_OBS_VAR(columnRenaming);
	if (zephir_array_isset_string_fetch(&columnRenaming, options, SS("columnRenaming"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "orm.column_renaming", 1);
		zephir_call_func_p2_noret("globals_set", _0, columnRenaming);
	}
	ZEPHIR_OBS_VAR(notNullValidations);
	if (zephir_array_isset_string_fetch(&notNullValidations, options, SS("notNullValidations"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "orm.not_null_validations", 1);
		zephir_call_func_p2_noret("globals_set", _0, notNullValidations);
	}
	ZEPHIR_OBS_VAR(exceptionOnFailedSave);
	if (zephir_array_isset_string_fetch(&exceptionOnFailedSave, options, SS("exceptionOnFailedSave"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "orm.exception_on_failed_save", 1);
		zephir_call_func_p2_noret("globals_set", _0, exceptionOnFailedSave);
	}
	ZEPHIR_OBS_VAR(phqlLiterals);
	if (zephir_array_isset_string_fetch(&phqlLiterals, options, SS("phqlLiterals"), 0 TSRMLS_CC)) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "orm.enable_literals", 1);
		zephir_call_func_p2_noret("globals_set", _0, phqlLiterals);
	}
	ZEPHIR_MM_RESTORE();

}

