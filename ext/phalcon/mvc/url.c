
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
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
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
 * Phalcon\Mvc\Url
 *
 * This components aids in the generation of: URIs, URLs and Paths
 *
 *<code>
 *
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_Url) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Mvc, Url, phalcon, mvc_url, phalcon_mvc_url_method_entry, 0);

	zend_declare_property_null(phalcon_mvc_url_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_url_ce, SL("_baseUri"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_url_ce, SL("_staticBaseUri"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_url_ce, SL("_basePath"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_url_ce, SL("_router"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_mvc_url_ce TSRMLS_CC, 1, phalcon_mvc_urlinterface_ce);
	zend_class_implements(phalcon_mvc_url_ce TSRMLS_CC, 1, phalcon_di_injectionawareinterface_ce);

	return SUCCESS;

}

/**
 * Sets the DependencyInjector container
 *
 * @param Phalcon\DiInterface dependencyInjector
 */
PHP_METHOD(Phalcon_Mvc_Url, setDI) {

	zval *dependencyInjector;

	zephir_fetch_params(0, 1, 0, &dependencyInjector);



	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);

}

/**
 * Returns the DependencyInjector container
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Phalcon_Mvc_Url, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Sets a prefix for all the URIs to be generated
 *
 *<code>
 *	$url->setBaseUri('/invo/');
 *	$url->setBaseUri('/invo/index.php/');
 *</code>
 *
 * @param string baseUri
 * @return Phalcon\Mvc\Url
 */
PHP_METHOD(Phalcon_Mvc_Url, setBaseUri) {

	zval *baseUri_param = NULL, *_0;
	zval *baseUri = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &baseUri_param);

	if (Z_TYPE_P(baseUri_param) != IS_STRING && Z_TYPE_P(baseUri_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'baseUri' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(baseUri_param) == IS_STRING) {
		baseUri = baseUri_param;
	} else {
		ZEPHIR_INIT_VAR(baseUri);
		ZVAL_EMPTY_STRING(baseUri);
	}


	zephir_update_property_this(this_ptr, SL("_baseUri"), baseUri TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_staticBaseUri"), PH_NOISY_CC);
	if ((Z_TYPE_P(_0) == IS_NULL)) {
		zephir_update_property_this(this_ptr, SL("_staticBaseUri"), baseUri TSRMLS_CC);
	}
	RETURN_THIS();

}

/**
 * Sets a prefix for all static URLs generated
 *
 *<code>
 *	$url->setStaticBaseUri('/invo/');
 *</code>
 *
 * @param string staticBaseUri
 * @return Phalcon\Mvc\Url
 */
PHP_METHOD(Phalcon_Mvc_Url, setStaticBaseUri) {

	zval *staticBaseUri_param = NULL;
	zval *staticBaseUri = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &staticBaseUri_param);

	if (Z_TYPE_P(staticBaseUri_param) != IS_STRING && Z_TYPE_P(staticBaseUri_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'staticBaseUri' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(staticBaseUri_param) == IS_STRING) {
		staticBaseUri = staticBaseUri_param;
	} else {
		ZEPHIR_INIT_VAR(staticBaseUri);
		ZVAL_EMPTY_STRING(staticBaseUri);
	}


	zephir_update_property_this(this_ptr, SL("_staticBaseUri"), staticBaseUri TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the prefix for all the generated urls. By default /
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, getBaseUri) {

	zval *baseUri, *phpSelf, *uri, *_SERVER;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(baseUri);
	zephir_read_property_this(&baseUri, this_ptr, SL("_baseUri"), PH_NOISY_CC);
	if ((Z_TYPE_P(baseUri) == IS_NULL)) {
		ZEPHIR_INIT_VAR(uri);
		ZEPHIR_OBS_VAR(phpSelf);
		zephir_get_global(&_SERVER, SS("_SERVER") TSRMLS_CC);
		if (zephir_array_isset_string_fetch(&phpSelf, _SERVER, SS("PHP_SELF"), 0 TSRMLS_CC)) {
			zephir_call_func_p1(uri, "phalcon_get_uri", phpSelf);
		} else {
			ZVAL_NULL(uri);
		}
		ZEPHIR_INIT_BNVAR(baseUri);
		if (!(zephir_is_true(uri))) {
			ZVAL_STRING(baseUri, "/", 1);
		} else {
			ZEPHIR_CONCAT_SVS(baseUri, "/", uri, "/");
		}
		zephir_update_property_this(this_ptr, SL("_baseUri"), baseUri TSRMLS_CC);
	}
	RETURN_CCTOR(baseUri);

}

/**
 * Returns the prefix for all the generated static urls. By default /
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, getStaticBaseUri) {

	zval *staticBaseUri;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(staticBaseUri);
	zephir_read_property_this(&staticBaseUri, this_ptr, SL("_staticBaseUri"), PH_NOISY_CC);
	if ((Z_TYPE_P(staticBaseUri) != IS_NULL)) {
		RETURN_CCTOR(staticBaseUri);
	}
	zephir_call_method(return_value, this_ptr, "getbaseuri");
	RETURN_MM();

}

/**
 * Sets a base path for all the generated paths
 *
 *<code>
 *	$url->setBasePath('/var/www/htdocs/');
 *</code>
 *
 * @param string basePath
 * @return Phalcon\Mvc\Url
 */
PHP_METHOD(Phalcon_Mvc_Url, setBasePath) {

	zval *basePath_param = NULL;
	zval *basePath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &basePath_param);

	if (Z_TYPE_P(basePath_param) != IS_STRING && Z_TYPE_P(basePath_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'basePath' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(basePath_param) == IS_STRING) {
		basePath = basePath_param;
	} else {
		ZEPHIR_INIT_VAR(basePath);
		ZVAL_EMPTY_STRING(basePath);
	}


	zephir_update_property_this(this_ptr, SL("_basePath"), basePath TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the base path
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, getBasePath) {


	RETURN_MEMBER(this_ptr, "_basePath");

}

/**
 * Generates a URL
 *
 *<code>
 *
 * //Generate a URL appending the URI to the base URI
 * echo $url->get('products/edit/1');
 *
 * //Generate a URL for a predefined route
 * echo $url->get(array('for' => 'blog-post', 'title' => 'some-cool-stuff', 'year' => '2012'));
 *
 *</code>
 *
 * @param string|array uri
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, get) {

	zval *uri = NULL, *baseUri, *router = NULL, *dependencyInjector = NULL, *routeName, *route = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, *_4, *_5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(baseUri);
	zephir_call_method(baseUri, this_ptr, "getbaseuri");
	if ((Z_TYPE_P(uri) == IS_ARRAY)) {
		ZEPHIR_OBS_VAR(routeName);
		if (!(zephir_array_isset_string_fetch(&routeName, uri, SS("for"), 0 TSRMLS_CC))) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_url_exception_ce, "It's necessary to define the route name with the parameter 'for'");
			return;
		}
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_router"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(router, _0);
		if ((Z_TYPE_P(router) != IS_OBJECT)) {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
			ZEPHIR_CPY_WRT(dependencyInjector, _0);
			if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_url_exception_ce, "A dependency injector container is required to obtain the 'router' service");
				return;
			}
			ZEPHIR_INIT_VAR(_1);
			ZEPHIR_INIT_VAR(_2);
			ZVAL_STRING(_2, "router", 1);
			zephir_call_method_p1(_1, dependencyInjector, "getshared", _2);
			ZEPHIR_CPY_WRT(router, _1);
			zephir_update_property_this(this_ptr, SL("_router"), router TSRMLS_CC);
		}
		ZEPHIR_INIT_NVAR(_1);
		zephir_call_method_p1(_1, router, "getroutebyname", routeName);
		ZEPHIR_CPY_WRT(route, _1);
		if ((Z_TYPE_P(route) != IS_OBJECT)) {
			ZEPHIR_INIT_VAR(_3);
			object_init_ex(_3, phalcon_mvc_url_exception_ce);
			ZEPHIR_INIT_VAR(_4);
			ZEPHIR_CONCAT_SVS(_4, "Cannot obtain a route using the name '", routeName, "'");
			zephir_call_method_p1_noret(_3, "__construct", _4);
			zephir_throw_exception(_3 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(_1);
		zephir_call_method(_1, route, "getpattern");
		ZEPHIR_INIT_NVAR(_2);
		zephir_call_method(_2, route, "getreversedpaths");
		ZEPHIR_INIT_VAR(_5);
		zephir_call_func_p3(_5, "phalcon_replace_paths", _1, _2, uri);
		ZEPHIR_CONCAT_VV(return_value, baseUri, _5);
		RETURN_MM();
	}
	ZEPHIR_CONCAT_VV(return_value, baseUri, uri);
	RETURN_MM();

}

/**
 * Generates a URL for a static resource
 *
 * @param string|array uri
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, getStatic) {

	zval *uri = NULL, *staticBaseUri, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &uri);

	if (!uri) {
		uri = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(staticBaseUri);
	zephir_read_property_this(&staticBaseUri, this_ptr, SL("_staticBaseUri"), PH_NOISY_CC);
	if ((Z_TYPE_P(staticBaseUri) != IS_NULL)) {
		ZEPHIR_CONCAT_VV(return_value, staticBaseUri, uri);
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "getbaseuri");
	ZEPHIR_CONCAT_VV(return_value, _0, uri);
	RETURN_MM();

}

/**
 * Generates a local path
 *
 * @param string path
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Url, path) {

	zval *path = NULL, *_0;

	zephir_fetch_params(0, 0, 1, &path);

	if (!path) {
		path = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_basePath"), PH_NOISY_CC);
	ZEPHIR_CONCAT_VV(return_value, _0, path);
	return;

}

