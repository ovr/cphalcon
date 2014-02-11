
#ifdef HAVE_CONFIG_H
#include "../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../php_ext.h"
#include "../../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/memory.h"
#include "kernel/require.h"
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"


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
 * Phalcon\Mvc\View\Engine\Php
 *
 * Adapter to use PHP itself as templating engine
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_View_Engine_Php) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Mvc\\View\\Engine, Php, phalcon, mvc_view_engine_php, phalcon_mvc_view_engine_ce, phalcon_mvc_view_engine_php_method_entry, 0);

	zend_class_implements(phalcon_mvc_view_engine_php_ce TSRMLS_CC, 1, phalcon_mvc_view_engineinterface_ce);

	return SUCCESS;

}

/**
 * Renders a view using the template engine
 *
 * @param string path
 * @param array params
 * @param boolean mustClean
 */
PHP_METHOD(Phalcon_Mvc_View_Engine_Php, render) {

	HashTable *_1;
	HashPosition _0;
	zend_bool mustClean;
	zval *path_param = NULL, *params, *mustClean_param = NULL, *key = NULL, *value = NULL, **_2, *_3, *_4, *_5;
	zval *path = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &path_param, &params, &mustClean_param);

	if (Z_TYPE_P(path_param) != IS_STRING && Z_TYPE_P(path_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'path' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(path_param) == IS_STRING) {
		path = path_param;
	} else {
		ZEPHIR_INIT_VAR(path);
		ZVAL_EMPTY_STRING(path);
	}
	if (!mustClean_param) {
		mustClean = 0;
	} else {
		mustClean = zephir_get_boolval(mustClean_param);
	}


	if ((mustClean == 1)) {
		zephir_call_func_noret("ob_clean");
	}
	if ((Z_TYPE_P(params) == IS_ARRAY)) {
		zephir_is_iterable(params, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(key, _1, _0);
			ZEPHIR_GET_HVALUE(value, _2);
			if (zephir_set_symbol(key, value TSRMLS_CC) == FAILURE){
			  return;
			}
		}
	}
	ZEPHIR_INIT_VAR(_3);
	if (zephir_require_ret(_3, path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	if ((mustClean == 1)) {
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("_view"), PH_NOISY_CC);
		ZEPHIR_INIT_VAR(_5);
		zephir_call_func(_5, "ob_get_contents");
		zephir_call_method_p1_noret(_4, "setcontent", _5);
	}
	ZEPHIR_MM_RESTORE();

}

