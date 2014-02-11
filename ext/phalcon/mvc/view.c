
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
#include "kernel/array.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/file.h"
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
 * Phalcon\Mvc\View
 *
 * Phalcon\Mvc\View is a class for working with the "view" portion of the model-view-controller pattern.
 * That is, it exists to help keep the view script separate from the model and controller scripts.
 * It provides a system of helpers, output filters, and variable escaping.
 *
 * <code>
 * //Setting views directory
 * $view = new Phalcon\Mvc\View();
 * $view->setViewsDir('app/views/');
 *
 * $view->start();
 * //Shows recent posts view (app/views/posts/recent.phtml)
 * $view->render('posts', 'recent');
 * $view->finish();
 *
 * //Printing views output
 * echo $view->getContent();
 * </code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_View) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Mvc, View, phalcon, mvc_view, phalcon_di_injectable_ce, phalcon_mvc_view_method_entry, 0);

	zend_declare_property_null(phalcon_mvc_view_ce, SL("_options"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(phalcon_mvc_view_ce, SL("_basePath"), "", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(phalcon_mvc_view_ce, SL("_content"), "", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_mvc_view_ce, SL("_renderLevel"), 5, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_disabledLevels"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_viewParams"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_layout"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(phalcon_mvc_view_ce, SL("_layoutsDir"), "", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(phalcon_mvc_view_ce, SL("_partialsDir"), "", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_viewsDir"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_templatesBefore"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_templatesAfter"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(phalcon_mvc_view_ce, SL("_engines"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_registeredEngines"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(phalcon_mvc_view_ce, SL("_mainView"), "index", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_controllerName"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_actionName"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_params"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_pickView"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_cache"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_long(phalcon_mvc_view_ce, SL("_cacheLevel"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_view_ce, SL("_activeRenderPath"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(phalcon_mvc_view_ce, SL("_disabled"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
/**
 * Render Level: To the main layout
 *
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_MAIN_LAYOUT"), 5 TSRMLS_CC);
/**
 * Render Level: Render to the templates "after"
 *
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_AFTER_TEMPLATE"), 4 TSRMLS_CC);
/**
 * Render Level: Hasta el layout del controlador
 *
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_LAYOUT"), 3 TSRMLS_CC);
/**
 * Render Level: To the templates "before"
 *
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_BEFORE_TEMPLATE"), 2 TSRMLS_CC);
/**
 * Render Level: To the action view
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_ACTION_VIEW"), 1 TSRMLS_CC);
/**
 * Render Level: No render any view
 *
 */
	zend_declare_class_constant_long(phalcon_mvc_view_ce, SL("LEVEL_NO_RENDER"), 0 TSRMLS_CC);

	zend_class_implements(phalcon_mvc_view_ce TSRMLS_CC, 1, phalcon_mvc_viewinterface_ce);

	return SUCCESS;

}

/**
 * Phalcon\Mvc\View constructor
 *
 * @param array options
 */
PHP_METHOD(Phalcon_Mvc_View, __construct) {

	zval *options = NULL;

	zephir_fetch_params(0, 0, 1, &options);

	if (!options) {
		options = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(options) == IS_ARRAY)) {
		zephir_update_property_this(this_ptr, SL("_options"), options TSRMLS_CC);
	}

}

/**
 * Sets the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string viewsDir
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setViewsDir) {

	zval *viewsDir_param = NULL;
	zval *viewsDir = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &viewsDir_param);

		zephir_get_strval(viewsDir, viewsDir_param);


	zephir_update_property_this(this_ptr, SL("_viewsDir"), viewsDir TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Gets views directory
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getViewsDir) {


	RETURN_MEMBER(this_ptr, "_viewsDir");

}

/**
 * Sets the layouts sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 *<code>
 * $view->setLayoutsDir('../common/layouts/');
 *</code>
 *
 * @param string layoutsDir
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setLayoutsDir) {

	zval *layoutsDir_param = NULL;
	zval *layoutsDir = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &layoutsDir_param);

		zephir_get_strval(layoutsDir, layoutsDir_param);


	zephir_update_property_this(this_ptr, SL("_layoutsDir"), layoutsDir TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Gets the current layouts sub-directory
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getLayoutsDir) {


	RETURN_MEMBER(this_ptr, "_layoutsDir");

}

/**
 * Sets a partials sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 *<code>
 * $view->setPartialsDir('../common/partials/');
 *</code>
 *
 * @param string partialsDir
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setPartialsDir) {

	zval *partialsDir_param = NULL;
	zval *partialsDir = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &partialsDir_param);

		zephir_get_strval(partialsDir, partialsDir_param);


	zephir_update_property_this(this_ptr, SL("_partialsDir"), partialsDir TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Gets the current partials sub-directory
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getPartialsDir) {


	RETURN_MEMBER(this_ptr, "_partialsDir");

}

/**
 * Sets base path. Depending of your platform, always add a trailing slash or backslash
 *
 * <code>
 * 	$view->setBasePath(__DIR__ . '/');
 * </code>
 *
 * @param string basePath
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setBasePath) {

	zval *basePath_param = NULL;
	zval *basePath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &basePath_param);

		zephir_get_strval(basePath, basePath_param);


	zephir_update_property_this(this_ptr, SL("_basePath"), basePath TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Sets the render level for the view
 *
 * <code>
 * 	//Render the view related to the controller only
 * 	$this->view->setRenderLevel(View::LEVEL_VIEW);
 * </code>
 *
 * @param int level
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setRenderLevel) {

	zval *level_param = NULL, *_0;
	int level;

	zephir_fetch_params(0, 1, 0, &level_param);

		level = zephir_get_intval(level_param);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, level);
	zephir_update_property_zval(this_ptr, SL("_renderLevel"), _0 TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Disables a specific level of rendering
 *
 *<code>
 * //Render all levels except ACTION level
 * $this->view->disableLevel(View::LEVEL_ACTION_VIEW);
 *</code>
 *
 * @param int|array level
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, disableLevel) {

	zval *level;

	zephir_fetch_params(0, 1, 0, &level);



	if ((Z_TYPE_P(level) == IS_ARRAY)) {
		zephir_update_property_this(this_ptr, SL("_disabledLevels"), level TSRMLS_CC);
	} else {
		zephir_update_property_array(this_ptr, SL("_disabledLevels"), level, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	}
	RETURN_THISW();

}

/**
 * Sets default view name. Must be a file without extension in the views directory
 *
 * <code>
 * 	//Renders as main view views-dir/base.phtml
 * 	$this->view->setMainView('base');
 * </code>
 *
 * @param string viewPath
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setMainView) {

	zval *viewPath_param = NULL;
	zval *viewPath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &viewPath_param);

		zephir_get_strval(viewPath, viewPath_param);


	zephir_update_property_this(this_ptr, SL("_mainView"), viewPath TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the name of the main view
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getMainView) {


	RETURN_MEMBER(this_ptr, "_mainView");

}

/**
 * Change the layout to be used instead of using the name of the latest controller name
 *
 * <code>
 * 	$this->view->setLayout('main');
 * </code>
 *
 * @param string layout
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setLayout) {

	zval *layout_param = NULL;
	zval *layout = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &layout_param);

		zephir_get_strval(layout, layout_param);


	zephir_update_property_this(this_ptr, SL("_layout"), layout TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns the name of the main view
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getLayout) {


	RETURN_MEMBER(this_ptr, "_layout");

}

/**
 * Sets a template before the controller layout
 *
 * @param string|array templateBefore
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setTemplateBefore) {

	zval *_0;
	zval *templateBefore;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &templateBefore);



	if ((Z_TYPE_P(templateBefore) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(_0);
		array_init_size(_0, 2);
		zephir_array_fast_append(_0, templateBefore);
		zephir_update_property_this(this_ptr, SL("_templatesBefore"), _0 TSRMLS_CC);
	} else {
		zephir_update_property_this(this_ptr, SL("_templatesBefore"), templateBefore TSRMLS_CC);
	}
	RETURN_THIS();

}

/**
 * Resets any "template before" layouts
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, cleanTemplateBefore) {


	zephir_update_property_this(this_ptr, SL("_templatesBefore"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Sets a "template after" controller layout
 *
 * @param string|array templateAfter
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setTemplateAfter) {

	zval *_0;
	zval *templateAfter;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &templateAfter);



	if ((Z_TYPE_P(templateAfter) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(_0);
		array_init_size(_0, 2);
		zephir_array_fast_append(_0, templateAfter);
		zephir_update_property_this(this_ptr, SL("_templatesAfter"), _0 TSRMLS_CC);
	} else {
		zephir_update_property_this(this_ptr, SL("_templatesAfter"), templateAfter TSRMLS_CC);
	}
	RETURN_THIS();

}

/**
 * Resets any template before layouts
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, cleanTemplateAfter) {


	zephir_update_property_this(this_ptr, SL("_templatesAfter"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Adds parameters to views (alias of setVar)
 *
 *<code>
 *	$this->view->setParamToView('products', $products);
 *</code>
 *
 * @param string key
 * @param mixed value
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setParamToView) {

	zval *key_param = NULL, *value;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &key_param, &value);

	if (Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(key_param) == IS_STRING) {
		key = key_param;
	} else {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	}


	zephir_update_property_array(this_ptr, SL("_viewParams"), key, value TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Set all the render params
 *
 *<code>
 *	$this->view->setVars(array('products' => $products));
 *</code>
 *
 * @param array params
 * @param boolean merge
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setVars) {

	zend_bool merge;
	zval *params, *merge_param = NULL, *viewParams, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &params, &merge_param);

	if (!merge_param) {
		merge = 1;
	} else {
		merge = zephir_get_boolval(merge_param);
	}


	if ((Z_TYPE_P(params) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_view_exception_ce, "The render parameters must be an array");
		return;
	}
	if (merge) {
		ZEPHIR_OBS_VAR(viewParams);
		zephir_read_property_this(&viewParams, this_ptr, SL("_viewParams"), PH_NOISY_CC);
		if ((Z_TYPE_P(viewParams) == IS_ARRAY)) {
			ZEPHIR_INIT_VAR(_0);
			zephir_fast_array_merge(_0, &(viewParams), &(params) TSRMLS_CC);
			zephir_update_property_this(this_ptr, SL("_viewParams"), _0 TSRMLS_CC);
		} else {
			zephir_update_property_this(this_ptr, SL("_viewParams"), params TSRMLS_CC);
		}
	} else {
		zephir_update_property_this(this_ptr, SL("_viewParams"), params TSRMLS_CC);
	}
	RETURN_THIS();

}

/**
 * Set a single view parameter
 *
 *<code>
 *	$this->view->setVar('products', $products);
 *</code>
 *
 * @param string key
 * @param mixed value
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setVar) {

	zval *key_param = NULL, *value;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &key_param, &value);

	if (Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(key_param) == IS_STRING) {
		key = key_param;
	} else {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	}


	zephir_update_property_array(this_ptr, SL("_viewParams"), key, value TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns a parameter previously set in the view
 *
 * @param string key
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_View, getVar) {

	zval *key_param = NULL, *params, *value;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &key_param);

	if (Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(key_param) == IS_STRING) {
		key = key_param;
	} else {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	}


	params = zephir_fetch_nproperty_this(this_ptr, SL("_viewParams"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&value, params, key, 1 TSRMLS_CC)) {
		RETURN_CTOR(value);
	}
	RETURN_MM_NULL();

}

/**
 * Returns parameters to views
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_View, getParamsToView) {


	RETURN_MEMBER(this_ptr, "_viewParams");

}

/**
 * Gets the name of the controller rendered
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getControllerName) {


	RETURN_MEMBER(this_ptr, "_controllerName");

}

/**
 * Gets the name of the action rendered
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getActionName) {


	RETURN_MEMBER(this_ptr, "_actionName");

}

/**
 * Gets extra parameters of the action rendered
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_View, getParams) {


	RETURN_MEMBER(this_ptr, "_params");

}

/**
 * Starts rendering process enabling the output buffering
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, start) {

	ZEPHIR_MM_GROW();

	zephir_call_func_noret("ob_start");
	zephir_update_property_this(this_ptr, SL("_content"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Loads registered template engines, if none is registered it will use Phalcon\Mvc\View\Engine\Php
 *
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_View, _loadTemplateEngines) {

	zend_function *_7 = NULL, *_8 = NULL;
	HashTable *_3;
	HashPosition _2;
	zval *engines, *dependencyInjector = NULL, *registeredEngines, *arguments, *engineService = NULL, *extension = NULL, *_0, *_1 = NULL, **_4, *_5 = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(engines);
	zephir_read_property_this(&engines, this_ptr, SL("_engines"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE(engines)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(dependencyInjector, _0);
		ZEPHIR_INIT_BNVAR(engines);
		array_init(engines);
		ZEPHIR_OBS_VAR(registeredEngines);
		zephir_read_property_this(&registeredEngines, this_ptr, SL("_registeredEngines"), PH_NOISY_CC);
		if ((Z_TYPE_P(registeredEngines) != IS_ARRAY)) {
			ZEPHIR_INIT_VAR(_1);
			object_init_ex(_1, phalcon_mvc_view_engine_php_ce);
			zephir_call_method_p2_noret(_1, "__construct", this_ptr, dependencyInjector);
			zephir_array_update_string(&engines, SL(".phtml"), &_1, PH_COPY | PH_SEPARATE);
		} else {
			if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
				ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_view_exception_ce, "A dependency injector container is required to obtain the application services");
				return;
			}
			ZEPHIR_INIT_VAR(arguments);
			array_init_size(arguments, 3);
			zephir_array_fast_append(arguments, this_ptr);
			zephir_array_fast_append(arguments, dependencyInjector);
			zephir_is_iterable(registeredEngines, &_3, &_2, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
				; zend_hash_move_forward_ex(_3, &_2)
			) {
				ZEPHIR_GET_HMKEY(extension, _3, _2);
				ZEPHIR_GET_HVALUE(engineService, _4);
				if ((Z_TYPE_P(engineService) == IS_OBJECT)) {
					if (zephir_is_instance_of(engineService, SL("Closure") TSRMLS_CC)) {
						ZEPHIR_INIT_NVAR(_5);
						ZEPHIR_CALL_USER_FUNC_ARRAY(_5, engineService, arguments);
						zephir_array_update_zval(&engines, extension, &_5, PH_COPY | PH_SEPARATE);
					} else {
						zephir_array_update_zval(&engines, extension, &engineService, PH_COPY | PH_SEPARATE);
					}
				} else {
					if ((Z_TYPE_P(engineService) != IS_STRING)) {
						ZEPHIR_INIT_LNVAR(_1);
						object_init_ex(_1, phalcon_mvc_view_exception_ce);
						ZEPHIR_INIT_LNVAR(_6);
						ZEPHIR_CONCAT_SV(_6, "Invalid template engine registration for extension: ", extension);
						zephir_call_method_p1_cache_noret(_1, "__construct", &_7, _6);
						zephir_throw_exception(_1 TSRMLS_CC);
						ZEPHIR_MM_RESTORE();
						return;
					}
					ZEPHIR_INIT_NVAR(_5);
					zephir_call_method_p2_cache(_5, dependencyInjector, "getshared", &_8, engineService, arguments);
					zephir_array_update_zval(&engines, extension, &_5, PH_COPY | PH_SEPARATE);
				}
			}
		}
		zephir_update_property_this(this_ptr, SL("_engines"), engines TSRMLS_CC);
	}
	RETURN_CCTOR(engines);

}

/**
 * Checks whether view exists on registered extensions and render it
 *
 * @param array engines
 * @param string viewPath
 * @param boolean silence
 * @param boolean mustClean
 * @param Phalcon\Cache\BackendInterface cache
 */
PHP_METHOD(Phalcon_Mvc_View, _engineRender) {

	zend_function *_8 = NULL, *_9 = NULL;
	HashTable *_6;
	HashPosition _5;
	int renderLevel, cacheLevel;
	zend_bool silence, mustClean, notExists;
	zval *viewPath = NULL;
	zval *engines, *viewPath_param = NULL, *silence_param = NULL, *mustClean_param = NULL, *cache, *key = NULL, *lifetime = NULL, *viewsDir, *basePath, *viewsDirPath, *viewOptions, *cacheOptions, *cachedView, *viewParams, *eventsManager = NULL, *extension = NULL, *engine = NULL, *viewEnginePath = NULL, *_0, *_1, *_2 = NULL, *_3 = NULL, *_4, **_7, *_10, *_11;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 5, 0, &engines, &viewPath_param, &silence_param, &mustClean_param, &cache);

		zephir_get_strval(viewPath, viewPath_param);
		silence = zephir_get_boolval(silence_param);
		mustClean = zephir_get_boolval(mustClean_param);


	notExists = 1;
	ZEPHIR_OBS_VAR(viewsDir);
	zephir_read_property_this(&viewsDir, this_ptr, SL("_viewsDir"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(basePath);
	zephir_read_property_this(&basePath, this_ptr, SL("_basePath"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(viewsDirPath);
	ZEPHIR_CONCAT_VVV(viewsDirPath, basePath, viewsDir, viewPath);
	if ((Z_TYPE_P(cache) == IS_OBJECT)) {
		ZEPHIR_OBS_VAR(_0);
		zephir_read_property_this(&_0, this_ptr, SL("_renderLevel"), PH_NOISY_CC);
		renderLevel = zephir_get_intval(_0);
		ZEPHIR_OBS_VAR(_1);
		zephir_read_property_this(&_1, this_ptr, SL("_cacheLevel"), PH_NOISY_CC);
		cacheLevel = zephir_get_intval(_1);
		if ((renderLevel >= cacheLevel)) {
			ZEPHIR_INIT_VAR(_2);
			zephir_call_method(_2, cache, "isstarted");
			if (ZEPHIR_IS_FALSE(_2)) {
				ZEPHIR_INIT_VAR(key);
				ZVAL_NULL(key);
				ZEPHIR_INIT_VAR(lifetime);
				ZVAL_NULL(lifetime);
				ZEPHIR_OBS_VAR(viewOptions);
				zephir_read_property_this(&viewOptions, this_ptr, SL("_options"), PH_NOISY_CC);
				if ((Z_TYPE_P(viewOptions) == IS_ARRAY)) {
					ZEPHIR_OBS_VAR(cacheOptions);
					if (zephir_array_isset_string_fetch(&cacheOptions, viewOptions, SS("cache"), 0 TSRMLS_CC)) {
						if ((Z_TYPE_P(cacheOptions) == IS_ARRAY)) {
							ZEPHIR_OBS_NVAR(key);
							ZEPHIR_OBS_NVAR(lifetime);
						}
					}
				}
				if ((Z_TYPE_P(key) == IS_NULL)) {
					ZEPHIR_INIT_NVAR(key);
					zephir_call_func_p1(key, "md5", viewPath);
				}
				ZEPHIR_INIT_VAR(cachedView);
				zephir_call_method_p2(cachedView, cache, "start", key, lifetime);
				if ((Z_TYPE_P(cachedView) != IS_NULL)) {
					zephir_update_property_this(this_ptr, SL("_content"), cachedView TSRMLS_CC);
					RETURN_MM_NULL();
				}
			}
			ZEPHIR_INIT_VAR(_3);
			zephir_call_method(_3, cache, "isfresh");
			if (!(zephir_is_true(_3))) {
				RETURN_MM_NULL();
			}
		}
	}
	ZEPHIR_OBS_VAR(viewParams);
	zephir_read_property_this(&viewParams, this_ptr, SL("_viewParams"), PH_NOISY_CC);
	_4 = zephir_fetch_nproperty_this(this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(eventsManager, _4);
	//missing empty
	zephir_is_iterable(engines, &_6, &_5, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_6, (void**) &_7, &_5) == SUCCESS
		; zend_hash_move_forward_ex(_6, &_5)
	) {
		ZEPHIR_GET_HMKEY(extension, _6, _5);
		ZEPHIR_GET_HVALUE(engine, _7);
		ZEPHIR_INIT_NVAR(viewEnginePath);
		ZEPHIR_CONCAT_VV(viewEnginePath, viewsDirPath, extension);
		if ((zephir_file_exists(viewEnginePath TSRMLS_CC) == SUCCESS)) {
			if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
				zephir_update_property_this(this_ptr, SL("_activeRenderPath"), viewEnginePath TSRMLS_CC);
				ZEPHIR_INIT_NVAR(_2);
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "view:beforeRenderView", 1);
				zephir_call_method_p3_cache(_2, eventsManager, "fire", &_8, _3, this_ptr, viewEnginePath);
				if (ZEPHIR_IS_FALSE(_2)) {
					continue;
				}
			}
			zephir_call_method_p3_noret(engine, "render", viewEnginePath, viewParams, (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
			notExists = 0;
			if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "view:afterRenderView", 1);
				zephir_call_method_p2_cache_noret(eventsManager, "fire", &_9, _3, this_ptr);
			}
			break;
		}
	}
	if ((notExists == 1)) {
		if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
			zephir_update_property_this(this_ptr, SL("_activeRenderPath"), viewEnginePath TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_STRING(_2, "view:notFoundView", 1);
			zephir_call_method_p3_noret(eventsManager, "fire", _2, this_ptr, viewEnginePath);
		}
		if (!(silence)) {
			ZEPHIR_INIT_VAR(_10);
			object_init_ex(_10, phalcon_mvc_view_exception_ce);
			ZEPHIR_INIT_VAR(_11);
			ZEPHIR_CONCAT_SVS(_11, "View '", viewsDirPath, "' was not found in the views directory");
			zephir_call_method_p1_noret(_10, "__construct", _11);
			zephir_throw_exception(_10 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Register templating engines
 *
 *<code>
 *$this->view->registerEngines(array(
 *  ".phtml" => "Phalcon\Mvc\View\Engine\Php",
 *  ".volt"  => "Phalcon\Mvc\View\Engine\Volt",
 *  ".mhtml" => "MyCustomEngine"
 *));
 *</code>
 *
 * @param array engines
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, registerEngines) {

	zval *engines;

	zephir_fetch_params(0, 1, 0, &engines);



	if ((Z_TYPE_P(engines) != IS_ARRAY)) {
		ZEPHIR_THROW_EXCEPTION_STRW(phalcon_mvc_view_exception_ce, "Engines to register must be an array");
		return;
	}
	zephir_update_property_this(this_ptr, SL("_registeredEngines"), engines TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Executes render process from dispatching data
 *
 *<code>
 * //Shows recent posts view (app/views/posts/recent.phtml)
 * $view->start()->render('posts', 'recent')->finish();
 *</code>
 *
 * @param string controllerName
 * @param string actionName
 * @param array params
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, render) {

	zend_function *_10 = NULL, *_15 = NULL;
	HashTable *_7, *_12;
	HashPosition _6, _11;
	int cacheLevel, renderLevel;
	zend_bool silence, mustClean;
	zval *controllerName_param = NULL, *actionName_param = NULL, *params = NULL, *layoutsDir, *layout, *pickView, *layoutName = NULL, *engines, *renderView = NULL, *pickViewAction, *cache, *eventsManager, *disabledLevels, *templatesBefore, *templatesAfter, *templateBefore = NULL, *templateAfter = NULL, *_0, *_1 = NULL, *_3, *_4 = NULL, *_5, **_8, *_9 = NULL, **_13, *_14 = NULL, *_16, *_17;
	zval *controllerName = NULL, *actionName = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &controllerName_param, &actionName_param, &params);

	if (Z_TYPE_P(controllerName_param) != IS_STRING && Z_TYPE_P(controllerName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'controllerName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(controllerName_param) == IS_STRING) {
		controllerName = controllerName_param;
	} else {
		ZEPHIR_INIT_VAR(controllerName);
		ZVAL_EMPTY_STRING(controllerName);
	}
	if (Z_TYPE_P(actionName_param) != IS_STRING && Z_TYPE_P(actionName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'actionName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(actionName_param) == IS_STRING) {
		actionName = actionName_param;
	} else {
		ZEPHIR_INIT_VAR(actionName);
		ZVAL_EMPTY_STRING(actionName);
	}
	if (!params) {
		params = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_disabled"), PH_NOISY_CC);
	if (!ZEPHIR_IS_FALSE(_0)) {
		ZEPHIR_INIT_VAR(_1);
		zephir_call_func(_1, "ob_get_contents");
		zephir_update_property_this(this_ptr, SL("_content"), _1 TSRMLS_CC);
		RETURN_THIS();
	}
	zephir_update_property_this(this_ptr, SL("_controllerName"), controllerName TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_actionName"), actionName TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_params"), params TSRMLS_CC);
	ZEPHIR_OBS_VAR(layoutsDir);
	zephir_read_property_this(&layoutsDir, this_ptr, SL("_layoutsDir"), PH_NOISY_CC);
	if (!(zephir_is_true(layoutsDir))) {
		ZEPHIR_INIT_BNVAR(layoutsDir);
		ZVAL_STRING(layoutsDir, "layouts/", 1);
	}
	ZEPHIR_OBS_VAR(layout);
	zephir_read_property_this(&layout, this_ptr, SL("_layout"), PH_NOISY_CC);
	if (zephir_is_true(layout)) {
		ZEPHIR_CPY_WRT(layoutName, layout);
	} else {
		ZEPHIR_CPY_WRT(layoutName, controllerName);
	}
	ZEPHIR_INIT_VAR(engines);
	zephir_call_method(engines, this_ptr, "_loadtemplateengines");
	ZEPHIR_OBS_VAR(pickView);
	zephir_read_property_this(&pickView, this_ptr, SL("_pickView"), PH_NOISY_CC);
	if ((Z_TYPE_P(pickView) == IS_NULL)) {
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_CONCAT_VSV(_2, controllerName, "/", actionName);
		ZEPHIR_CPY_WRT(renderView, _2);
	} else {
		ZEPHIR_OBS_VAR(renderView);
		zephir_array_fetch_long(&renderView, pickView, 0, PH_NOISY TSRMLS_CC);
		ZEPHIR_OBS_VAR(pickViewAction);
		if (zephir_array_isset_long_fetch(&pickViewAction, pickView, 1, 0 TSRMLS_CC)) {
			ZEPHIR_CPY_WRT(layoutName, pickViewAction);
		}
	}
	ZEPHIR_INIT_VAR(cache);
	ZVAL_NULL(cache);
	ZEPHIR_OBS_VAR(_3);
	zephir_read_property_this(&_3, this_ptr, SL("_cacheLevel"), PH_NOISY_CC);
	cacheLevel = zephir_get_intval(_3);
	if (cacheLevel) {
		ZEPHIR_INIT_BNVAR(cache);
		zephir_call_method(cache, this_ptr, "getcache");
	}
	ZEPHIR_OBS_VAR(eventsManager);
	zephir_read_property_this(&eventsManager, this_ptr, SL("_eventsManager"), PH_NOISY_CC);
	zephir_create_symbol_table(TSRMLS_C);
	
	if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(_1);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "view:beforeRender", 1);
		zephir_call_method_p2(_1, eventsManager, "fire", _4, this_ptr);
		if (ZEPHIR_IS_FALSE(_1)) {
			RETURN_THIS();
		}
	}
	ZEPHIR_INIT_NVAR(_1);
	zephir_call_func(_1, "ob_get_contents");
	zephir_update_property_this(this_ptr, SL("_content"), _1 TSRMLS_CC);
	mustClean = 1;
	silence = 1;
	ZEPHIR_OBS_VAR(disabledLevels);
	zephir_read_property_this(&disabledLevels, this_ptr, SL("_disabledLevels"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(_5);
	zephir_read_property_this(&_5, this_ptr, SL("_renderLevel"), PH_NOISY_CC);
	renderLevel = zephir_get_intval(_5);
	if (renderLevel) {
		if ((renderLevel >= 1)) {
			if (!(zephir_array_isset_long(disabledLevels, 1))) {
				zephir_call_method_p5_noret(this_ptr, "_enginerender", engines, renderView, (silence ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), cache);
			}
		}
		if ((renderLevel >= 2)) {
			if (!(zephir_array_isset_long(disabledLevels, 2))) {
				ZEPHIR_OBS_VAR(templatesBefore);
				zephir_read_property_this(&templatesBefore, this_ptr, SL("_templatesBefore"), PH_NOISY_CC);
				if ((Z_TYPE_P(templatesBefore) == IS_ARRAY)) {
					silence = 0;
					zephir_is_iterable(templatesBefore, &_7, &_6, 0, 0);
					for (
						; zend_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
						; zend_hash_move_forward_ex(_7, &_6)
					) {
						ZEPHIR_GET_HVALUE(templateBefore, _8);
						ZEPHIR_INIT_LNVAR(_9);
						ZEPHIR_CONCAT_VV(_9, layoutsDir, templateBefore);
						zephir_call_method_p5_cache_noret(this_ptr, "_enginerender", &_10, engines, _9, (silence ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), cache);
					}
					silence = 1;
				}
			}
		}
		if ((renderLevel >= 3)) {
			if (!(zephir_array_isset_long(disabledLevels, 3))) {
				ZEPHIR_INIT_LNVAR(_9);
				ZEPHIR_CONCAT_VV(_9, layoutsDir, layoutName);
				zephir_call_method_p5_noret(this_ptr, "_enginerender", engines, _9, (silence ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), cache);
			}
		}
		if ((renderLevel >= 4)) {
			if (!(zephir_array_isset_long(disabledLevels, 4))) {
				ZEPHIR_OBS_VAR(templatesAfter);
				zephir_read_property_this(&templatesAfter, this_ptr, SL("_templatesAfter"), PH_NOISY_CC);
				if ((Z_TYPE_P(templatesAfter) == IS_ARRAY)) {
					silence = 0;
					zephir_is_iterable(templatesAfter, &_12, &_11, 0, 0);
					for (
						; zend_hash_get_current_data_ex(_12, (void**) &_13, &_11) == SUCCESS
						; zend_hash_move_forward_ex(_12, &_11)
					) {
						ZEPHIR_GET_HVALUE(templateAfter, _13);
						ZEPHIR_INIT_LNVAR(_14);
						ZEPHIR_CONCAT_VV(_14, layoutsDir, templateAfter);
						zephir_call_method_p5_cache_noret(this_ptr, "_enginerender", &_15, engines, _14, (silence ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), cache);
					}
					silence = 1;
				}
			}
		}
		if ((renderLevel >= 5)) {
			if (!(zephir_array_isset_long(disabledLevels, 5))) {
				_16 = zephir_fetch_nproperty_this(this_ptr, SL("_mainView"), PH_NOISY_CC);
				zephir_call_method_p5_noret(this_ptr, "_enginerender", engines, _16, (silence ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), (mustClean ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)), cache);
			}
		}
		if ((Z_TYPE_P(cache) == IS_OBJECT)) {
			ZEPHIR_INIT_NVAR(_4);
			zephir_call_method(_4, cache, "isstarted");
			if (ZEPHIR_IS_TRUE(_4)) {
				ZEPHIR_INIT_VAR(_17);
				zephir_call_method(_17, cache, "isfresh");
				if (ZEPHIR_IS_TRUE(_17)) {
					zephir_call_method_noret(cache, "save");
				} else {
					zephir_call_method_noret(cache, "stop");
				}
			} else {
				zephir_call_method_noret(cache, "stop");
			}
		}
	}
	if ((Z_TYPE_P(eventsManager) == IS_OBJECT)) {
		ZEPHIR_INIT_NVAR(_1);
		ZVAL_STRING(_1, "view:afterRender", 1);
		zephir_call_method_p2_noret(eventsManager, "fire", _1, this_ptr);
	}
	RETURN_THIS();

}

/**
 * Choose a different view to render instead of last-controller/last-action
 *
 * <code>
 * class ProductsController extends Phalcon\Mvc\Controller
 * {
 *
 *    public function saveAction()
 *    {
 *
 *         //Do some save stuff...
 *
 *         //Then show the list view
 *         $this->view->pick("products/list");
 *    }
 * }
 * </code>
 *
 * @param string|array renderView
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, pick) {

	zval *renderView, *pickView = NULL, *layout = NULL, *parts;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &renderView);



	if ((Z_TYPE_P(renderView) == IS_ARRAY)) {
		ZEPHIR_CPY_WRT(pickView, renderView);
	} else {
		ZEPHIR_INIT_VAR(layout);
		ZVAL_NULL(layout);
		if (zephir_memnstr_str(renderView, SL("/"), "phalcon/mvc/view.zep", 973)) {
			ZEPHIR_INIT_VAR(parts);
			zephir_fast_explode_str(parts, SL("/"), renderView, LONG_MAX TSRMLS_CC);
			ZEPHIR_OBS_NVAR(layout);
			zephir_array_fetch_long(&layout, parts, 0, PH_NOISY TSRMLS_CC);
		}
		ZEPHIR_INIT_VAR(pickView);
		array_init_size(pickView, 2);
		zephir_array_fast_append(pickView, renderView);
		if ((Z_TYPE_P(layout) != IS_NULL)) {
			zephir_array_append(&pickView, layout, PH_SEPARATE);
		}
	}
	zephir_update_property_this(this_ptr, SL("_pickView"), pickView TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Renders a partial view
 *
 * <code>
 * 	//Show a partial inside another view
 * 	$this->partial('shared/footer');
 * </code>
 *
 * <code>
 * 	//Show a partial inside another view with parameters
 * 	$this->partial('shared/footer', array('content' => $html));
 * </code>
 *
 * @param string partialPath
 * @param array params
 */
PHP_METHOD(Phalcon_Mvc_View, partial) {

	zval *partialPath_param = NULL, *params = NULL, *viewParams, *_0 = NULL, *_1, *_2;
	zval *partialPath = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &partialPath_param, &params);

	if (Z_TYPE_P(partialPath_param) != IS_STRING && Z_TYPE_P(partialPath_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'partialPath' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(partialPath_param) == IS_STRING) {
		partialPath = partialPath_param;
	} else {
		ZEPHIR_INIT_VAR(partialPath);
		ZVAL_EMPTY_STRING(partialPath);
	}
	if (!params) {
		params = ZEPHIR_GLOBAL(global_null);
	}


	if ((Z_TYPE_P(params) == IS_ARRAY)) {
		ZEPHIR_OBS_VAR(viewParams);
		zephir_read_property_this(&viewParams, this_ptr, SL("_viewParams"), PH_NOISY_CC);
		if ((Z_TYPE_P(viewParams) == IS_ARRAY)) {
			ZEPHIR_INIT_VAR(_0);
			zephir_fast_array_merge(_0, &(viewParams), &(params) TSRMLS_CC);
			zephir_update_property_this(this_ptr, SL("_viewParams"), _0 TSRMLS_CC);
		} else {
			zephir_update_property_this(this_ptr, SL("_viewParams"), params TSRMLS_CC);
		}
		zephir_create_symbol_table(TSRMLS_C);
		
	}
	ZEPHIR_INIT_NVAR(_0);
	zephir_call_method(_0, this_ptr, "_loadtemplateengines");
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_partialsDir"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_CONCAT_VV(_2, _1, partialPath);
	zephir_call_method_p5_noret(this_ptr, "_enginerender", _0, _2, ZEPHIR_GLOBAL(global_false), ZEPHIR_GLOBAL(global_false), ZEPHIR_GLOBAL(global_false));
	if ((Z_TYPE_P(params) == IS_ARRAY)) {
		zephir_update_property_this(this_ptr, SL("_viewParams"), viewParams TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Perform the automatic rendering returning the output as a string
 *
 * <code>
 * 	$template = $this->view->getRender('products', 'show', array('products' => $products));
 * </code>
 *
 * @param string controllerName
 * @param string actionName
 * @param array params
 * @param mixed configCallback
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getRender) {

	zval *_1;
	zval *controllerName_param = NULL, *actionName_param = NULL, *params = NULL, *configCallback = NULL, *view, *_0;
	zval *controllerName = NULL, *actionName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 2, &controllerName_param, &actionName_param, &params, &configCallback);

	if (Z_TYPE_P(controllerName_param) != IS_STRING && Z_TYPE_P(controllerName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'controllerName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(controllerName_param) == IS_STRING) {
		controllerName = controllerName_param;
	} else {
		ZEPHIR_INIT_VAR(controllerName);
		ZVAL_EMPTY_STRING(controllerName);
	}
	if (Z_TYPE_P(actionName_param) != IS_STRING && Z_TYPE_P(actionName_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'actionName' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(actionName_param) == IS_STRING) {
		actionName = actionName_param;
	} else {
		ZEPHIR_INIT_VAR(actionName);
		ZVAL_EMPTY_STRING(actionName);
	}
	if (!params) {
		params = ZEPHIR_GLOBAL(global_null);
	}
	if (!configCallback) {
		configCallback = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(view);
	if (zephir_clone(view, this_ptr TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	zephir_call_method_noret(view, "reset");
	if ((Z_TYPE_P(params) == IS_ARRAY)) {
		zephir_call_method_p1_noret(view, "setvars", params);
	}
	if ((Z_TYPE_P(configCallback) == IS_OBJECT)) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_INIT_VAR(_1);
		array_init_size(_1, 2);
		zephir_array_fast_append(_1, view);
		ZEPHIR_CALL_USER_FUNC_ARRAY(_0, configCallback, _1);
	}
	zephir_call_method_noret(view, "start");
	zephir_call_method_p2_noret(view, "render", controllerName, actionName);
	zephir_call_func_noret("ob_end_clean");
	zephir_call_method(return_value, view, "getcontent");
	RETURN_MM();

}

/**
 * Finishes the render process by stopping the output buffering
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, finish) {

	ZEPHIR_MM_GROW();

	zephir_call_func_noret("ob_end_clean");
	RETURN_THIS();

}

/**
 * Create a Phalcon\Cache based on the internal cache options
 *
 * @return Phalcon\Cache\BackendInterface
 */
PHP_METHOD(Phalcon_Mvc_View, _createCache) {

	zval *dependencyInjector = NULL, *cacheService = NULL, *viewCache = NULL, *viewOptions, *cacheOptions, *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_dependencyInjector"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(dependencyInjector, _0);
	if ((Z_TYPE_P(dependencyInjector) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_view_exception_ce, "A dependency injector container is required to obtain the view cache services");
		return;
	}
	ZEPHIR_INIT_VAR(cacheService);
	ZVAL_STRING(cacheService, "viewCache", 1);
	ZEPHIR_OBS_VAR(viewOptions);
	zephir_read_property_this(&viewOptions, this_ptr, SL("_options"), PH_NOISY_CC);
	if ((Z_TYPE_P(viewOptions) == IS_ARRAY)) {
		ZEPHIR_OBS_VAR(cacheOptions);
		if (zephir_array_isset_string_fetch(&cacheOptions, viewOptions, SS("cache"), 0 TSRMLS_CC)) {
			if (zephir_array_isset_string(cacheOptions, SS("service"))) {
				ZEPHIR_OBS_NVAR(cacheService);
				zephir_array_fetch_string(&cacheService, cacheOptions, SL("service"), PH_NOISY TSRMLS_CC);
			}
		}
	}
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p1(_1, dependencyInjector, "getshared", cacheService);
	ZEPHIR_CPY_WRT(viewCache, _1);
	if (!ZEPHIR_IS_STRING(viewCache, "object")) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_view_exception_ce, "The injected caching service is invalid");
		return;
	}
	RETURN_CCTOR(viewCache);

}

/**
 * Check if the component is currently caching the output content
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_View, isCaching) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_cacheLevel"), PH_NOISY_CC);
	RETURN_BOOL(ZEPHIR_GT_LONG(_0, 0));

}

/**
 * Returns the cache instance used to cache
 *
 * @return Phalcon\Cache\BackendInterface
 */
PHP_METHOD(Phalcon_Mvc_View, getCache) {

	zval *cache;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(cache);
	zephir_read_property_this(&cache, this_ptr, SL("_cache"), PH_NOISY_CC);
	if (zephir_is_true(cache)) {
		if ((Z_TYPE_P(cache) != IS_OBJECT)) {
			ZEPHIR_INIT_BNVAR(cache);
			zephir_call_method(cache, this_ptr, "_createcache");
			zephir_update_property_this(this_ptr, SL("_cache"), cache TSRMLS_CC);
		}
	} else {
		ZEPHIR_INIT_BNVAR(cache);
		zephir_call_method(cache, this_ptr, "_createcache");
		zephir_update_property_this(this_ptr, SL("_cache"), cache TSRMLS_CC);
	}
	RETURN_CCTOR(cache);

}

/**
 * Cache the actual view render to certain level
 *
 *<code>
 *  $this->view->cache(array('key' => 'my-key', 'lifetime' => 86400));
 *</code>
 *
 * @param boolean|array options
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, cache) {

	HashTable *_1;
	HashPosition _0;
	zval *options = NULL, *viewOptions = NULL, *cacheOptions = NULL, *key = NULL, *value = NULL, *cacheLevel, **_2, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &options);

	if (!options) {
		options = ZEPHIR_GLOBAL(global_true);
	}


	if ((Z_TYPE_P(options) == IS_ARRAY)) {
		ZEPHIR_OBS_VAR(viewOptions);
		zephir_read_property_this(&viewOptions, this_ptr, SL("_options"), PH_NOISY_CC);
		if ((Z_TYPE_P(viewOptions) != IS_ARRAY)) {
			ZEPHIR_INIT_NVAR(viewOptions);
			array_init(viewOptions);
		}
		ZEPHIR_OBS_VAR(cacheOptions);
		if (!(zephir_array_isset_string_fetch(&cacheOptions, viewOptions, SS("cache"), 0 TSRMLS_CC))) {
			ZEPHIR_INIT_NVAR(cacheOptions);
			array_init(cacheOptions);
		}
		zephir_is_iterable(options, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(key, _1, _0);
			ZEPHIR_GET_HVALUE(value, _2);
			zephir_array_update_zval(&cacheOptions, key, &value, PH_COPY | PH_SEPARATE);
		}
		if (zephir_array_isset_string_fetch(&cacheLevel, cacheOptions, SS("level"), 1 TSRMLS_CC)) {
			zephir_update_property_this(this_ptr, SL("_cacheLevel"), cacheLevel TSRMLS_CC);
		} else {
			ZEPHIR_INIT_ZVAL_NREF(_3);
			ZVAL_LONG(_3, 5);
			zephir_update_property_this(this_ptr, SL("_cacheLevel"), _3 TSRMLS_CC);
		}
		zephir_array_update_string(&viewOptions, SL("cache"), &cacheOptions, PH_COPY | PH_SEPARATE);
		zephir_update_property_this(this_ptr, SL("_options"), viewOptions TSRMLS_CC);
	} else {
		if (zephir_is_true(options)) {
			ZEPHIR_INIT_ZVAL_NREF(_3);
			ZVAL_LONG(_3, 5);
			zephir_update_property_this(this_ptr, SL("_cacheLevel"), _3 TSRMLS_CC);
		} else {
			ZEPHIR_INIT_ZVAL_NREF(_3);
			ZVAL_LONG(_3, 0);
			zephir_update_property_this(this_ptr, SL("_cacheLevel"), _3 TSRMLS_CC);
		}
	}
	RETURN_THIS();

}

/**
 * Externally sets the view content
 *
 *<code>
 *	$this->view->setContent("<h1>hello</h1>");
 *</code>
 *
 * @param string content
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, setContent) {

	zval *content_param = NULL;
	zval *content = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &content_param);

		zephir_get_strval(content, content_param);


	zephir_update_property_this(this_ptr, SL("_content"), content TSRMLS_CC);
	RETURN_THIS();

}

/**
 * Returns cached output from another view stage
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getContent) {


	RETURN_MEMBER(this_ptr, "_content");

}

/**
 * Returns the path of the view that is currently rendered
 *
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_View, getActiveRenderPath) {


	RETURN_MEMBER(this_ptr, "_activeRenderPath");

}

/**
 * Disables the auto-rendering process
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, disable) {


	zephir_update_property_this(this_ptr, SL("_disabled"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Enables the auto-rendering process
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, enable) {


	zephir_update_property_this(this_ptr, SL("_disabled"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Resets the view component to its factory default values
 *
 * @return Phalcon\Mvc\View
 */
PHP_METHOD(Phalcon_Mvc_View, reset) {

	zval *_0;


	zephir_update_property_this(this_ptr, SL("_disabled"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_engines"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_cache"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 5);
	zephir_update_property_this(this_ptr, SL("_renderLevel"), _0 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("_cacheLevel"), _0 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_content"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_templatesBefore"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_templatesAfter"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Magic method to pass variables to the views
 *
 *<code>
 *	$this->view->products = $products;
 *</code>
 *
 * @param string key
 * @param mixed value
 */
PHP_METHOD(Phalcon_Mvc_View, __set) {

	zval *key_param = NULL, *value;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &key_param, &value);

	if (Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(key_param) == IS_STRING) {
		key = key_param;
	} else {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	}


	zephir_update_property_array(this_ptr, SL("_viewParams"), key, value TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Magic method to retrieve a variable passed to the view
 *
 *<code>
 *	echo $this->view->products;
 *</code>
 *
 * @param string key
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_View, __get) {

	zval *key_param = NULL, *value, *_0;
	zval *key = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &key_param);

	if (Z_TYPE_P(key_param) != IS_STRING && Z_TYPE_P(key_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'key' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(key_param) == IS_STRING) {
		key = key_param;
	} else {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_viewParams"), PH_NOISY_CC);
	if (zephir_array_isset_fetch(&value, _0, key, 1 TSRMLS_CC)) {
		RETURN_CTOR(value);
	}
	RETURN_MM_NULL();

}

