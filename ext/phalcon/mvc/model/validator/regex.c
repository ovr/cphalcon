
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
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/array.h"


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
 * Phalcon\Mvc\Model\Validator\Regex
 *
 * Allows validate if the value of a field matches a regular expression
 *
 *<code>
 *use Phalcon\Mvc\Model\Validator\Regex as RegexValidator;
 *
 *class Subscriptors extends Phalcon\Mvc\Model
 *{
 *
 *  public function validation()
 *  {
 *      this->validate(new RegexValidator(array(
 *          "field" => 'created_at',
 *          'pattern' => '/^[0-9]{4}[-\/](0[1-9]|1[12])[-\/](0[1-9]|[12][0-9]|3[01])/'
 *      )));
 *      if (this->validationHasFailed() == true) {
 *          return false;
 *      }
 *  }
 *
 *}
 *</code>
 *
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_Model_Validator_Regex) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Mvc\\Model\\Validator, Regex, phalcon, mvc_model_validator_regex, phalcon_mvc_model_validator_ce, phalcon_mvc_model_validator_regex_method_entry, 0);

	zend_class_implements(phalcon_mvc_model_validator_regex_ce TSRMLS_CC, 1, phalcon_mvc_model_validatorinterface_ce);

	return SUCCESS;

}

/**
 * Executes the validator
 *
 * @param Phalcon\Mvc\ModelInterface record
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model_Validator_Regex, validate) {

	zend_bool failed, _2;
	zval *record, *field, *visSet, *value, *matches, *pattern, *matchPattern, *matchZero, *message = NULL, *replacePairs, *_0, *_1, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &record);



	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "field", 1);
	ZEPHIR_INIT_VAR(field);
	zephir_call_method_p1(field, this_ptr, "getoption", _0);
	if ((Z_TYPE_P(field) != IS_STRING)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Field name must be a string");
		return;
	}
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "pattern", 1);
	ZEPHIR_INIT_VAR(visSet);
	zephir_call_method_p1(visSet, this_ptr, "issetoption", _0);
	if (!(zephir_is_true(visSet))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Validator requires a perl-compatible regex pattern");
		return;
	}
	ZEPHIR_INIT_VAR(value);
	zephir_call_method_p1(value, record, "readattribute", field);
	failed = 0;
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);
	ZEPHIR_INIT_BNVAR(_0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "allowEmpty", 1);
	zephir_call_method_p1(_0, this_ptr, "issetoption", _1);
	_2 = zephir_is_true(_0);
	if (_2) {
		_2 = ZEPHIR_IS_EMPTY(value);
	}
	if (_2) {
		RETURN_MM_BOOL(1);
	}
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "pattern", 1);
	ZEPHIR_INIT_VAR(pattern);
	zephir_call_method_p1(pattern, this_ptr, "getoption", _1);
	ZEPHIR_INIT_VAR(matchPattern);
	zephir_preg_match(matchPattern, &(matchPattern), pattern, value, matches, 0, 0 , 0  TSRMLS_CC);
	if (zephir_is_true(matchPattern)) {
		ZEPHIR_OBS_VAR(matchZero);
		zephir_array_fetch_long(&matchZero, matches, 0, PH_NOISY TSRMLS_CC);
		failed = (!ZEPHIR_IS_EQUAL(matchZero, value));
	} else {
		failed = 1;
	}
	if ((failed == 1)) {
		ZEPHIR_INIT_BNVAR(_1);
		ZVAL_STRING(_1, "message", 1);
		ZEPHIR_INIT_VAR(message);
		zephir_call_method_p1(message, this_ptr, "getoption", _1);
		ZEPHIR_INIT_VAR(replacePairs);
		array_init_size(replacePairs, 2);
		zephir_array_update_string(&replacePairs, SL(":field"), &field, PH_COPY | PH_SEPARATE);
		if (ZEPHIR_IS_EMPTY(message)) {
			ZEPHIR_INIT_NVAR(message);
			ZVAL_STRING(message, "Value of field :field doesn't match regular expression", 1);
		}
		ZEPHIR_INIT_BNVAR(_1);
		zephir_call_func_p2(_1, "strtr", message, replacePairs);
		ZEPHIR_INIT_VAR(_3);
		ZVAL_STRING(_3, "Regex", 1);
		zephir_call_method_p3_noret(this_ptr, "appendmessage", _1, field, _3);
		RETURN_MM_BOOL(0);
	}
	RETURN_MM_BOOL(1);

}

