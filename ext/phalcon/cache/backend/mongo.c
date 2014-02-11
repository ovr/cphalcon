
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
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/hash.h"


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
 * Phalcon\Cache\Backend\Mongo
 *
 * Allows to cache output fragments, PHP data or raw data to a MongoDb backend
 *
 *<code>
 *
 * // Cache data for 2 days
 * $frontCache = new Phalcon\Cache\Frontend\Base64(array(
 *		"lifetime" => 172800
 * ));
 *
 * //Create a MongoDB cache
 * $cache = new Phalcon\Cache\Backend\Mongo($frontCache, array(
 *		'server' => "mongodb://localhost",
 *      'db' => 'caches',
 *		'collection' => 'images'
 * ));
 *
 * //Cache arbitrary data
 * $cache->save('my-data', file_get_contents('some-image.jpg'));
 *
 * //Get data
 * $data = $cache->get('my-data');
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Cache_Backend_Mongo) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Cache\\Backend, Mongo, phalcon, cache_backend_mongo, phalcon_cache_backend_ce, phalcon_cache_backend_mongo_method_entry, 0);

	zend_declare_property_null(phalcon_cache_backend_mongo_ce, SL("_collection"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_cache_backend_mongo_ce TSRMLS_CC, 1, phalcon_cache_backendinterface_ce);

	return SUCCESS;

}

/**
 * Phalcon\Cache\Backend\Mongo constructor
 *
 * @param Phalcon\Cache\FrontendInterface $frontend
 * @param array $options
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, __construct) {

	zval *frontend, *options = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &frontend, &options);

	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_array_isset_string(options, SS("mongo")))) {
		if (!(zephir_array_isset_string(options, SS("server")))) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The parameter 'server' is required");
			return;
		}
	}
	if (!(zephir_array_isset_string(options, SS("db")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The parameter 'db' is required");
		return;
	}
	if (!(zephir_array_isset_string(options, SS("collection")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The parameter 'collection' is required");
		return;
	}
	zephir_call_parent_p2_noret(this_ptr, phalcon_cache_backend_mongo_ce, "__construct", frontend, options);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns a MongoDb collection based on the backend parameters
 *
 * @return MongoCollection
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, _getCollection) {

	zend_class_entry *_1;
	zend_bool _0;
	zval *options, *mongo = NULL, *server, *database, *collection, *mongoDatabase, *mongoCollection;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(mongoCollection);
	zephir_read_property_this(&mongoCollection, this_ptr, SL("_collection"), PH_NOISY_CC);
	if ((Z_TYPE_P(mongoCollection) != IS_OBJECT)) {
		ZEPHIR_OBS_VAR(options);
		zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
		if (zephir_array_isset_string(options, SS("mongo"))) {
			ZEPHIR_OBS_VAR(mongo);
			zephir_array_fetch_string(&mongo, options, SL("mongo"), PH_NOISY TSRMLS_CC);
			if ((Z_TYPE_P(mongo) != IS_OBJECT)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The 'mongo' parameter must be a valid Mongo instance");
				return;
			}
		} else {
			ZEPHIR_OBS_VAR(server);
			zephir_array_fetch_string(&server, options, SL("server"), PH_NOISY TSRMLS_CC);
			_0 = zephir_is_true(server);
			if (!(_0)) {
				_0 = (Z_TYPE_P(server) != IS_STRING);
			}
			if (!(_0)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The backend requires a valid MongoDB connection string");
				return;
			}
			ZEPHIR_INIT_NVAR(mongo);
			_1 = zend_fetch_class(SL("Mongo"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
			object_init_ex(mongo, _1);
			if (zephir_has_constructor(mongo TSRMLS_CC)) {
				zephir_call_method_noret(mongo, "__construct");
			}
		}
		ZEPHIR_OBS_VAR(database);
		zephir_array_fetch_string(&database, options, SL("db"), PH_NOISY TSRMLS_CC);
		_0 = zephir_is_true(database);
		if (!(_0)) {
			_0 = (Z_TYPE_P(database) != IS_STRING);
		}
		if (!(_0)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The backend requires a valid MongoDB db");
			return;
		}
		ZEPHIR_OBS_VAR(collection);
		zephir_array_fetch_string(&collection, options, SL("collection"), PH_NOISY TSRMLS_CC);
		_0 = zephir_is_true(collection);
		if (!(_0)) {
			_0 = (Z_TYPE_P(collection) != IS_STRING);
		}
		if (!(_0)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The backend requires a valid MongoDB collection");
			return;
		}
		ZEPHIR_INIT_VAR(mongoDatabase);
		zephir_call_method_p1(mongoDatabase, mongo, "selectdb", database);
		zephir_call_method_p1_noret(mongoDatabase, "selectcollection", collection);
	} else {
		RETURN_CCTOR(mongoCollection);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns a cached content
 *
 * @param int|string keyName
 * @param   long lifetime
 * @return  mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, get) {

	zval *keyName, *lifetime = NULL, *frontend, *prefixedKey, *conditions, *timeCondition, *document, *cachedContent, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &keyName, &lifetime);

	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(conditions);
	array_init(conditions);
	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, _0, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	zephir_array_update_string(&conditions, SL("key"), &prefixedKey, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(timeCondition);
	array_init_size(timeCondition, 2);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func(_1, "time");
	zephir_array_update_string(&timeCondition, SL("$gt"), &_1, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&conditions, SL("time"), &timeCondition, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	zephir_call_method(_1, this_ptr, "_getcollection");
	ZEPHIR_INIT_VAR(document);
	zephir_call_method_p1(document, _1, "findone", conditions);
	if ((Z_TYPE_P(document) == IS_ARRAY)) {
		if (zephir_array_isset_string(document, SS("data"))) {
			ZEPHIR_OBS_VAR(cachedContent);
			zephir_array_fetch_string(&cachedContent, document, SL("data"), PH_NOISY TSRMLS_CC);
			if (zephir_is_numeric(cachedContent)) {
				RETURN_CCTOR(cachedContent);
			} else {
				zephir_call_method_p1(return_value, frontend, "afterretrieve", cachedContent);
				RETURN_MM();
			}
		} else {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache is corrupt");
			return;
		}
	} else {
		RETURN_MM_NULL();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Stores cached content into the file backend and stops the frontend
 *
 * @param int|string keyName
 * @param string content
 * @param long lifetime
 * @param boolean stopBuffer
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, save) {

	zval *keyName = NULL, *content = NULL, *lifetime = NULL, *stopBuffer = NULL, *lastkey, *prefix, *frontend, *cachedContent = NULL, *tmp, *ttl = NULL, *collection, *timestamp, *conditions, *document, *preparedContent, *isBuffering, *data, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 4, &keyName, &content, &lifetime, &stopBuffer);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!content) {
		content = ZEPHIR_GLOBAL(global_null);
	}
	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}
	if (!stopBuffer) {
		stopBuffer = ZEPHIR_GLOBAL(global_true);
	}


	ZEPHIR_INIT_VAR(conditions);
	array_init(conditions);
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_INIT_VAR(lastkey);
	if (!(zephir_is_true(keyName))) {
		zephir_read_property_this(&lastkey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		ZEPHIR_OBS_VAR(prefix);
		zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_VV(lastkey, prefix, keyName);
	}
	if (!(zephir_is_true(lastkey))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache must be started first");
		return;
	}
	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	if (!(zephir_is_true(content))) {
		ZEPHIR_INIT_VAR(cachedContent);
		zephir_call_method(cachedContent, frontend, "getcontent");
	} else {
		ZEPHIR_CPY_WRT(cachedContent, content);
	}
	if (!(zephir_is_numeric(cachedContent))) {
		ZEPHIR_INIT_VAR(preparedContent);
		zephir_call_method_p1(preparedContent, frontend, "beforestore", cachedContent);
	}
	if (!(zephir_is_true(lifetime))) {
		ZEPHIR_OBS_VAR(tmp);
		zephir_read_property_this(&tmp, this_ptr, SL("_lastLifetime"), PH_NOISY_CC);
		if (!(zephir_is_true(tmp))) {
			ZEPHIR_INIT_VAR(ttl);
			zephir_call_method(ttl, frontend, "getlifetime");
		} else {
			ZEPHIR_CPY_WRT(ttl, tmp);
		}
	} else {
		ZEPHIR_CPY_WRT(ttl, lifetime);
	}
	ZEPHIR_INIT_VAR(collection);
	zephir_call_method(collection, this_ptr, "_getcollection");
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "time");
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "intval", ttl);
	ZEPHIR_INIT_VAR(timestamp);
	zephir_add_function(timestamp, _0, _1 TSRMLS_CC);
	zephir_array_update_string(&conditions, SL("key"), &lastkey, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(document);
	zephir_call_method_p1(document, collection, "findone", conditions);
	if ((Z_TYPE_P(document) == IS_ARRAY)) {
		zephir_array_update_string(&document, SL("time"), &timestamp, PH_COPY | PH_SEPARATE);
		if (!(zephir_is_numeric(cachedContent))) {
			zephir_array_update_string(&document, SL("data"), &preparedContent, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_update_string(&document, SL("data"), &cachedContent, PH_COPY | PH_SEPARATE);
		}
		zephir_call_method_p1_noret(collection, "save", document);
	} else {
		zephir_array_update_string(&data, SL("key"), &lastkey, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&data, SL("time"), &timestamp, PH_COPY | PH_SEPARATE);
		if (!(zephir_is_numeric(cachedContent))) {
			zephir_array_update_string(&data, SL("data"), &preparedContent, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_update_string(&data, SL("data"), &cachedContent, PH_COPY | PH_SEPARATE);
		}
		zephir_call_method_p1_noret(collection, "save", data);
	}
	ZEPHIR_INIT_VAR(isBuffering);
	zephir_call_method(isBuffering, frontend, "isbuffering");
	if (ZEPHIR_IS_TRUE(stopBuffer)) {
		zephir_call_method_noret(frontend, "stop");
	}
	if (ZEPHIR_IS_TRUE(isBuffering)) {
		zend_print_zval(cachedContent, 0);
	}
	zephir_update_property_this(this_ptr, SL("_started"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Deletes a value from the cache by its key
 *
 * @param int|string keyName
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, delete) {

	zval *_1;
	zval *keyName, *_0, *_2, *_3, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &keyName);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "_getcollection");
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 2);
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_3);
	ZEPHIR_CONCAT_VV(_3, _2, keyName);
	zephir_array_update_string(&_1, SL("key"), &_3, PH_COPY | PH_SEPARATE);
	zephir_call_method_p1_noret(_0, "remove", _1);
	ZEPHIR_INIT_VAR(_4);
	zephir_call_func(_4, "rand");
	if (((zephir_get_intval(_4) % 100) == 0)) {
		zephir_call_method_noret(this_ptr, "gc");
	}
	RETURN_MM_BOOL(1);

}

/**
 * Query the existing cached keys
 *
 * @param string prefix
 * @return array
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, queryKeys) {

	HashTable *_6;
	HashPosition _5;
	zend_class_entry *_2;
	zval *prefix = NULL, *collection, *fields, *conditions, *timeCondition, *documents, *keys, *index = NULL, *key = NULL, *_0, *_1, *_3, *_4, **_7;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &prefix);

	if (!prefix) {
		prefix = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(fields);
	array_init(fields);
	ZEPHIR_INIT_VAR(timeCondition);
	array_init_size(timeCondition, 2);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "time");
	zephir_array_update_string(&timeCondition, SL("$gt"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(collection);
	zephir_call_method(collection, this_ptr, "_getcollection");
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_update_string(&fields, SL("key"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(conditions);
	array_init(conditions);
	if (zephir_is_true(prefix)) {
		ZEPHIR_INIT_VAR(_1);
		_2 = zend_fetch_class(SL("MongoRegex"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
		object_init_ex(_1, _2);
		if (zephir_has_constructor(_1 TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(_3);
			ZEPHIR_CONCAT_SVS(_3, "/^", prefix, "/");
			zephir_call_method_p1_noret(_1, "__construct", _3);
		}
		zephir_array_update_string(&conditions, SL("key"), &_1, PH_COPY | PH_SEPARATE);
	}
	zephir_array_update_string(&conditions, SL("time"), &timeCondition, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(documents);
	zephir_call_method_p2(documents, collection, "find", conditions, fields);
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_INIT_VAR(_4);
	zephir_call_func_p1(_4, "iterator_to_array", documents);
	zephir_is_iterable(_4, &_6, &_5, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_6, (void**) &_7, &_5) == SUCCESS
		; zend_hash_move_forward_ex(_6, &_5)
	) {
		ZEPHIR_GET_HMKEY(index, _6, _5);
		ZEPHIR_GET_HVALUE(key, _7);
		if (ZEPHIR_IS_STRING(index, "key")) {
			zephir_array_append(&keys, key, PH_SEPARATE);
		}
	}
	RETURN_CCTOR(keys);

}

/**
 * Checks if cache exists and it isn't expired
 *
 * @param string keyName
 * @param   long lifetime
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, exists) {

	int number;
	zval *keyName = NULL, *lifetime = NULL, *lastKey, *collection, *conditions, *timeCondition, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &keyName, &lifetime);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(conditions);
	array_init(conditions);
	ZEPHIR_INIT_VAR(timeCondition);
	array_init_size(timeCondition, 2);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "time");
	zephir_array_update_string(&timeCondition, SL("$gt"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(lastKey);
	if ((Z_TYPE_P(keyName) == IS_NULL)) {
		zephir_read_property_this(&lastKey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_VV(lastKey, _1, keyName);
	}
	if (zephir_is_true(lastKey)) {
		ZEPHIR_INIT_VAR(collection);
		zephir_call_method(collection, this_ptr, "_getcollection");
		zephir_array_update_string(&conditions, SL("key"), &lastKey, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&conditions, SL("time"), &timeCondition, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_BNVAR(_0);
		zephir_call_method_p1(_0, collection, "count", conditions);
		number = zephir_get_intval(_0);
		if ((number > 0)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}

/**
 * gc
 * @return collection->remove(...)
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, gc) {

	zval *conditions, *timeCondition, *collection, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(timeCondition);
	array_init_size(timeCondition, 2);
	ZEPHIR_INIT_VAR(_0);
	zephir_call_func(_0, "time");
	zephir_array_update_string(&timeCondition, SL("$gt"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(conditions);
	array_init_size(conditions, 2);
	zephir_array_update_string(&conditions, SL("time"), &timeCondition, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(collection);
	zephir_call_method(collection, this_ptr, "_getcollection");
	zephir_call_method_p1(return_value, collection, "remove", conditions);
	RETURN_MM();

}

/**
 * Increment of a given key by $value
 *
 * @param int|string $keyName
 * @param   long $value
 * @return  mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, increment) {

	zval *_0;
	zend_bool notExpired;
	zval *keyName, *value = NULL, *frontend, *prefix, *prefixedKey, *collection, *document, *timestamp, *lifetime, *ttl = NULL, *modifiedTime, difference, *cachedContent, *keys;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &keyName, &value);

	if (!value) {
		ZEPHIR_INIT_VAR(value);
		ZVAL_LONG(value, 1);
	}


	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_INIT_VAR(collection);
	zephir_call_method(collection, this_ptr, "_getcollection");
	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 2);
	zephir_array_update_string(&_0, SL("key"), &prefixedKey, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(document);
	zephir_call_method_p1(document, collection, "findone", _0);
	ZEPHIR_INIT_VAR(timestamp);
	zephir_call_func(timestamp, "time");
	ZEPHIR_OBS_VAR(lifetime);
	zephir_read_property_this(&lifetime, this_ptr, SL("_lastLifetime"), PH_NOISY_CC);
	if (!(zephir_is_true(lifetime))) {
		ZEPHIR_INIT_VAR(ttl);
		zephir_call_method(ttl, frontend, "getlifetime");
	} else {
		ZEPHIR_CPY_WRT(ttl, lifetime);
	}
	if (!(zephir_array_isset_string(document, SS("time")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache is currupted");
		return;
	}
	ZEPHIR_OBS_VAR(modifiedTime);
	zephir_array_fetch_string(&modifiedTime, document, SL("time"), PH_NOISY TSRMLS_CC);
	ZEPHIR_SINIT_VAR(difference);
	sub_function(&difference, timestamp, ttl TSRMLS_CC);
	notExpired = ZEPHIR_LT(&difference, modifiedTime);
	if ((notExpired == 1)) {
		if (!(zephir_array_isset_string(document, SS("data")))) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache is currupted");
			return;
		}
		ZEPHIR_OBS_VAR(cachedContent);
		zephir_array_fetch_string(&cachedContent, document, SL("data"), PH_NOISY TSRMLS_CC);
		if (zephir_is_numeric(cachedContent)) {
			ZEPHIR_INIT_VAR(keys);
			zephir_add_function(keys, cachedContent, value TSRMLS_CC);
			ZEPHIR_INIT_NVAR(ttl);
			zephir_add_function(ttl, lifetime, timestamp TSRMLS_CC);
			zephir_call_method_p2_noret(this_ptr, "save", prefixedKey, keys);
		}
	}
	RETURN_MM_NULL();

}

/**
 * Decrement of a given key by $value
 *
 * @param int|string $keyName
 * @param   long $value
 * @return  mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, decrement) {

	zend_bool notExpired;
	zval *keyName, *value = NULL, *frontend, *prefix, *prefixedKey, *collection, *conditions, *document, *timestamp, *lifetime, *ttl = NULL, *modifiedTime, difference, *cachedContent, *keys;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &keyName, &value);

	if (!value) {
		ZEPHIR_INIT_VAR(value);
		ZVAL_LONG(value, 1);
	}


	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_INIT_VAR(collection);
	zephir_call_method(collection, this_ptr, "_getcollection");
	ZEPHIR_INIT_VAR(conditions);
	array_init_size(conditions, 2);
	zephir_array_update_string(&conditions, SL("key"), &prefixedKey, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(document);
	zephir_call_method_p1(document, collection, "findone", conditions);
	ZEPHIR_INIT_VAR(timestamp);
	zephir_call_func(timestamp, "time");
	ZEPHIR_OBS_VAR(lifetime);
	zephir_read_property_this(&lifetime, this_ptr, SL("_lastLifetime"), PH_NOISY_CC);
	if (!(zephir_is_true(lifetime))) {
		ZEPHIR_INIT_VAR(ttl);
		zephir_call_method(ttl, frontend, "getlifetime");
	} else {
		ZEPHIR_CPY_WRT(ttl, lifetime);
	}
	if (!(zephir_array_isset_string(document, SS("time")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache is currupted");
		return;
	}
	ZEPHIR_OBS_VAR(modifiedTime);
	zephir_array_fetch_string(&modifiedTime, document, SL("time"), PH_NOISY TSRMLS_CC);
	ZEPHIR_SINIT_VAR(difference);
	sub_function(&difference, timestamp, ttl TSRMLS_CC);
	notExpired = ZEPHIR_LT(&difference, modifiedTime);
	if ((notExpired == 1)) {
		if (!(zephir_array_isset_string(document, SS("data")))) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache is currupted");
			return;
		}
		ZEPHIR_OBS_VAR(cachedContent);
		zephir_array_fetch_string(&cachedContent, document, SL("data"), PH_NOISY TSRMLS_CC);
		if (zephir_is_numeric(cachedContent)) {
			ZEPHIR_INIT_VAR(keys);
			sub_function(keys, cachedContent, value TSRMLS_CC);
			ZEPHIR_INIT_NVAR(ttl);
			zephir_add_function(ttl, lifetime, timestamp TSRMLS_CC);
			zephir_call_method_p2_noret(this_ptr, "save", prefixedKey, keys);
		}
	}
	RETURN_MM_NULL();

}

/**
 * Immediately invalidates all existing items.
 *
 * @return bool
 */
PHP_METHOD(Phalcon_Cache_Backend_Mongo, flush) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "_getcollection");
	zephir_call_method_noret(_0, "remove");
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func(_1, "rand");
	if (((zephir_get_intval(_1) % 100) == 0)) {
		zephir_call_method_noret(this_ptr, "gc");
	}
	RETURN_MM_BOOL(1);

}

