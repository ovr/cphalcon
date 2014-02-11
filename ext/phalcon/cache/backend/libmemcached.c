
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
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
 * Phalcon\Cache\Backend\Libmemcached
 *
 * Allows to cache output fragments, PHP data or raw data to a libmemcached backend
 *
 * This adapter uses the special memcached key "_PHCM" to store all the keys internally used by the adapter
 *
 *<code>
 *
 * // Cache data for 2 days
 * $frontCache = new Phalcon\Cache\Frontend\Data(array(
 *    "lifetime" => 172800
 * ));
 *
 * //Create the Cache setting memcached connection options
 * $cache = new Phalcon\Cache\Backend\Libmemcached($frontCache, array(
 *     "servers" => array(
 *         array('host' => 'localhost',
 *               'port' => 11211,
 *               'weight' => 1),
 *     ),
 *     "client" => array(
 *         Memcached::OPT_HASH => Memcached::HASH_MD5,
 *         Memcached::OPT_PREFIX_KEY => 'prefix.',
 *     )
 * ));
 *
 * //Cache arbitrary data
 * $cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 * //Get data
 * $data = $cache->get('my-data');
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Cache_Backend_Libmemcached) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Cache\\Backend, Libmemcached, phalcon, cache_backend_libmemcached, phalcon_cache_backend_ce, phalcon_cache_backend_libmemcached_method_entry, 0);

	zend_declare_property_null(phalcon_cache_backend_libmemcached_ce, SL("_memcache"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_cache_backend_libmemcached_ce TSRMLS_CC, 1, phalcon_cache_backendinterface_ce);

	return SUCCESS;

}

/**
 * Phalcon\Cache\Backend\Memcache constructor
 *
 * @param	Phalcon\Cache\FrontendInterface frontend
 * @param	array options
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, __construct) {

	zval *_0;
	zval *frontend, *options = NULL, *servers, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &frontend, &options);

	if (!options) {
		ZEPHIR_CPY_WRT(options, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(options);
	}


	if ((Z_TYPE_P(options) != IS_ARRAY)) {
		ZEPHIR_INIT_NVAR(options);
		array_init(options);
	}
	if (!(zephir_array_isset_string(options, SS("servers")))) {
		ZEPHIR_INIT_VAR(servers);
		array_init_size(servers, 2);
		ZEPHIR_INIT_VAR(_0);
		array_init_size(_0, 5);
		add_assoc_stringl_ex(_0, SS("host"), SL("127.0.0.1"), 1);
		add_assoc_long_ex(_0, SS("port"), 11211);
		add_assoc_long_ex(_0, SS("weigth"), 1);
		zephir_array_update_long(&servers, 0, &_0, PH_COPY, "phalcon/cache/backend/libmemcached.zep", 77);
		zephir_array_update_string(&options, SL("servers"), &servers, PH_COPY | PH_SEPARATE);
	}
	if (!(zephir_array_isset_string(options, SS("statsKey")))) {
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "_PHCM", 1);
		zephir_array_update_string(&options, SL("statsKey"), &_1, PH_COPY | PH_SEPARATE);
	}
	zephir_call_parent_p2_noret(this_ptr, phalcon_cache_backend_libmemcached_ce, "__construct", frontend, options);
	ZEPHIR_MM_RESTORE();

}

/**
 * Create internal connection to memcached
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, _connect) {

	zend_function *_5 = NULL;
	HashTable *_3;
	HashPosition _2;
	zend_class_entry *_0;
	zval *options, *memcache, *client, *servers = NULL, *option = NULL, *res = NULL, *_1, **_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(options);
	zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(memcache);
	_0 = zend_fetch_class(SL("Memcached"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(memcache, _0);
	zephir_call_method_noret(memcache, "__construct");
	if (!(zephir_array_isset_string(options, SS("servers")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Servers must be an array");
		return;
	} else {
		ZEPHIR_OBS_VAR(servers);
		zephir_array_fetch_string(&servers, options, SL("servers"), PH_NOISY TSRMLS_CC);
		if ((Z_TYPE_P(servers) != IS_ARRAY)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Servers must be an array");
			return;
		}
	}
	ZEPHIR_OBS_VAR(client);
	zephir_array_fetch_string(&client, options, SL("client"), PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p1(_1, memcache, "addservers", servers);
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Cannot connect to Memcached server");
		return;
	}
	if ((Z_TYPE_P(client) == IS_ARRAY)) {
		zephir_is_iterable(client, &_3, &_2, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
			; zend_hash_move_forward_ex(_3, &_2)
		) {
			ZEPHIR_GET_HVALUE(option, _4);
			if ((Z_TYPE_P(option) == IS_STRING)) {
				ZEPHIR_INIT_NVAR(res);
				ZEPHIR_CALL_INTERNAL_FUNCTION(res, &res, "constant", &_5, 1, option);
				if (zephir_is_true(res)) {
					zephir_call_method_p1_noret(memcache, "setoption", res);
				}
			} else {
				zephir_call_method_p1_noret(memcache, "setoption", option);
			}
		}
	}
	zephir_update_property_this(this_ptr, SL("_memcache"), memcache TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns a cached content
 *
 * @param int|string keyName
 * @param   long lifetime
 * @return  mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, get) {

	zval *keyName, *lifetime = NULL, *memcache = NULL, *frontend, *prefix, *prefixedKey, *cachedContent;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &keyName, &lifetime);

	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_INIT_VAR(cachedContent);
	zephir_call_method_p1(cachedContent, memcache, "get", prefixedKey);
	if (!(zephir_is_true(cachedContent))) {
		RETURN_MM_NULL();
	}
	if (zephir_is_numeric(cachedContent)) {
		RETURN_CCTOR(cachedContent);
	} else {
		zephir_call_method_p1_noret(frontend, "afterretrieve", cachedContent);
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
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, save) {

	zval *keyName = NULL, *content = NULL, *lifetime = NULL, *stopBuffer = NULL, *lastKey, *prefix, *frontend, *memcache = NULL, *cachedContent = NULL, *preparedContent, *tmp, *tt1 = NULL, *success, *options, *specialKey, *keys, *isBuffering;

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


	ZEPHIR_INIT_VAR(lastKey);
	if (!(zephir_is_true(keyName))) {
		zephir_read_property_this(&lastKey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		ZEPHIR_OBS_VAR(prefix);
		zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_VV(lastKey, prefix, keyName);
	}
	if (!(zephir_is_true(lastKey))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "The cache must be started first");
		return;
	}
	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
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
			ZEPHIR_INIT_VAR(tt1);
			zephir_call_method(tt1, frontend, "getlifetime");
		} else {
			ZEPHIR_CPY_WRT(tt1, tmp);
		}
	} else {
		ZEPHIR_CPY_WRT(tt1, lifetime);
	}
	ZEPHIR_INIT_VAR(success);
	if (zephir_is_numeric(cachedContent)) {
		zephir_call_method_p3(success, memcache, "set", lastKey, cachedContent, tt1);
	} else {
		zephir_call_method_p3(success, memcache, "set", lastKey, preparedContent, tt1);
	}
	if (!(zephir_is_true(success))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Failed storing data in memcached");
		return;
	}
	ZEPHIR_OBS_VAR(options);
	zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
	if (!(zephir_array_isset_string(options, SS("statsKey")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}
	ZEPHIR_OBS_VAR(specialKey);
	zephir_array_fetch_string(&specialKey, options, SL("statsKey"), PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(keys);
	zephir_call_method_p1(keys, memcache, "get", specialKey);
	if ((Z_TYPE_P(keys) != IS_ARRAY)) {
		ZEPHIR_INIT_BNVAR(keys);
		array_init(keys);
	}
	if (!(zephir_array_isset(keys, lastKey))) {
		zephir_array_update_zval(&keys, lastKey, &tt1, PH_COPY | PH_SEPARATE);
		zephir_call_method_p2_noret(memcache, "set", specialKey, keys);
	}
	ZEPHIR_INIT_VAR(isBuffering);
	zephir_call_method(isBuffering, frontend, "isbuffering");
	if (!(zephir_is_true(stopBuffer))) {
		zephir_call_method_noret(frontend, "stop");
	}
	if (ZEPHIR_IS_TRUE(isBuffering)) {
		zend_print_zval(cachedContent, 0);
	}
	zephir_update_property_this(this_ptr, SL("_started"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Increment of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, increment) {

	zval *keyName = NULL, *value = NULL, *memcache = NULL, *prefix, *prefixedKey, *cachedContent;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &keyName, &value);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!value) {
		ZEPHIR_CPY_WRT(value, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(value);
	}


	if (!(zephir_is_true(value))) {
		ZEPHIR_INIT_NVAR(value);
		ZVAL_LONG(value, 1);
	}
	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_INIT_VAR(cachedContent);
	zephir_call_method_p2(cachedContent, memcache, "increment", prefixedKey, value);
	if (!(zephir_is_true(cachedContent))) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(cachedContent);

}

/**
 * Decrement of a given key, by number $value
 *
 * @param  string $keyName
 * @param  long $value
 * @return mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, decrement) {

	zval *keyName = NULL, *value = NULL, *memcache = NULL, *prefix, *prefixedKey, *cachedContent;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &keyName, &value);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!value) {
		ZEPHIR_CPY_WRT(value, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(value);
	}


	if (!(zephir_is_true(value))) {
		ZEPHIR_INIT_NVAR(value);
		ZVAL_LONG(value, 1);
	}
	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_INIT_VAR(cachedContent);
	zephir_call_method_p2(cachedContent, memcache, "decrement", prefixedKey, value);
	if (!(zephir_is_true(cachedContent))) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(cachedContent);

}

/**
 * Deletes a value from the cache by its key
 *
 * @param int|string keyName
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, delete) {

	zval *keyName, *memcache = NULL, *prefix, *prefixedKey, *options, *keys, *specialKey, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &keyName);



	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(prefix);
	zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_VV(prefixedKey, prefix, keyName);
	ZEPHIR_OBS_VAR(options);
	zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
	if (!(zephir_array_isset_string(options, SS("statsKey")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}
	ZEPHIR_OBS_VAR(specialKey);
	zephir_array_fetch_string(&specialKey, options, SL("statsKey"), PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(keys);
	zephir_call_method_p1(keys, memcache, "get", specialKey);
	if ((Z_TYPE_P(keys) == IS_ARRAY)) {
		zephir_array_fetch(&_0, keys, prefixedKey, PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_unset(&keys, _0, PH_SEPARATE);
		zephir_call_method_p2_noret(memcache, "set", specialKey, keys);
	}
	zephir_call_method_p1_noret(memcache, "delete", prefixedKey);
	ZEPHIR_MM_RESTORE();

}

/**
 * Query the existing cached keys
 *
 * @param string prefix
 * @return array
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, queryKeys) {

	zend_bool _3;
	HashTable *_1;
	HashPosition _0;
	zval *prefix = NULL, *memcache = NULL, *options, *keys, *specialKey, *key = NULL, **_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &prefix);

	if (!prefix) {
		prefix = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(options);
	zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
	if (!(zephir_array_isset_string(options, SS("statsKey")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}
	ZEPHIR_OBS_VAR(specialKey);
	zephir_array_fetch_string(&specialKey, options, SL("statsKey"), PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(keys);
	zephir_call_method_p1(keys, memcache, "get", specialKey);
	if ((Z_TYPE_P(keys) == IS_ARRAY)) {
		zephir_is_iterable(keys, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(key, _2);
			_3 = zephir_is_true(prefix);
			if (!(_3)) {
				_3 = zephir_start_with(key, prefix, 0);
			}
			if (!(_3)) {
				RETURN_CCTOR(key);
			}
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Checks if cache exists and it isn't expired
 *
 * @param string keyName
 * @param   long lifetime
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, exists) {

	zval *keyName = NULL, *lifetime = NULL, *lastKey, *memcache = NULL, *value, *prefix;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &keyName, &lifetime);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(lastKey);
	if (!(zephir_is_true(keyName))) {
		zephir_read_property_this(&lastKey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		ZEPHIR_OBS_VAR(prefix);
		zephir_read_property_this(&prefix, this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_VV(lastKey, prefix, keyName);
	}
	if (zephir_is_true(lastKey)) {
		ZEPHIR_OBS_VAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
		if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
			zephir_call_method_noret(this_ptr, "_connect");
			ZEPHIR_OBS_NVAR(memcache);
			zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
		}
		ZEPHIR_INIT_VAR(value);
		zephir_call_method_p1(value, memcache, "get", lastKey);
		if (!(zephir_is_true(value))) {
			RETURN_MM_BOOL(0);
		}
		RETURN_MM_BOOL(1);
	} else {
		RETURN_MM_BOOL(0);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Immediately invalidates all existing items.
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Libmemcached, flush) {

	HashTable *_1;
	HashPosition _0;
	zval *memcache = NULL, *options, *keys, *specialKey, *key = NULL, **_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(memcache);
	zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	if ((Z_TYPE_P(memcache) != IS_OBJECT)) {
		zephir_call_method_noret(this_ptr, "_connect");
		ZEPHIR_OBS_NVAR(memcache);
		zephir_read_property_this(&memcache, this_ptr, SL("_memcache"), PH_NOISY_CC);
	}
	ZEPHIR_OBS_VAR(options);
	zephir_read_property_this(&options, this_ptr, SL("_options"), PH_NOISY_CC);
	if (!(zephir_array_isset_string(options, SS("statsKey")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_cache_exception_ce, "Unexpected inconsistency in options");
		return;
	}
	ZEPHIR_OBS_VAR(specialKey);
	zephir_array_fetch_string(&specialKey, options, SL("statsKey"), PH_NOISY TSRMLS_CC);
	ZEPHIR_INIT_VAR(keys);
	zephir_call_method_p1(keys, memcache, "get", specialKey);
	if ((Z_TYPE_P(keys) == IS_ARRAY)) {
		zephir_is_iterable(keys, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HVALUE(key, _2);
			zephir_call_method_p1_noret(memcache, "delete", key);
		}
		zephir_call_method_p2_noret(memcache, "set", specialKey, keys);
	}
	RETURN_MM_BOOL(1);

}

