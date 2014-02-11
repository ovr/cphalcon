
extern zend_class_entry *phalcon_debug_dump_ce;

ZEPHIR_INIT_CLASS(Phalcon_Debug_Dump);

PHP_METHOD(Phalcon_Debug_Dump, __construct);
PHP_METHOD(Phalcon_Debug_Dump, vars);
PHP_METHOD(Phalcon_Debug_Dump, dump);
PHP_METHOD(Phalcon_Debug_Dump, output);
PHP_METHOD(Phalcon_Debug_Dump, setStyles);
PHP_METHOD(Phalcon_Debug_Dump, getStyle);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_debug_dump___construct, 0, 0, 0)
  ZEND_ARG_INFO(0, styles)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_debug_dump_dump, 0, 0, 1)
  ZEND_ARG_INFO(0, variable)
  ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_debug_dump_output, 0, 0, 1)
  ZEND_ARG_INFO(0, variable)
  ZEND_ARG_INFO(0, name)
  ZEND_ARG_INFO(0, tab)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_debug_dump_setstyles, 0, 0, 1)
  ZEND_ARG_INFO(0, styles)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_debug_dump_getstyle, 0, 0, 1)
  ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(phalcon_debug_dump_method_entry) {
	PHP_ME(Phalcon_Debug_Dump, __construct, arginfo_phalcon_debug_dump___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Phalcon_Debug_Dump, vars, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Debug_Dump, dump, arginfo_phalcon_debug_dump_dump, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Debug_Dump, output, arginfo_phalcon_debug_dump_output, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Debug_Dump, setStyles, arginfo_phalcon_debug_dump_setstyles, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Debug_Dump, getStyle, arginfo_phalcon_debug_dump_getstyle, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
