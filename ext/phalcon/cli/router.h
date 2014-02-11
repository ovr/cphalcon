
extern zend_class_entry *phalcon_cli_router_ce;

ZEPHIR_INIT_CLASS(Phalcon_Cli_Router);

PHP_METHOD(Phalcon_Cli_Router, setDI);
PHP_METHOD(Phalcon_Cli_Router, getDI);
PHP_METHOD(Phalcon_Cli_Router, setDefaultModule);
PHP_METHOD(Phalcon_Cli_Router, setDefaultTask);
PHP_METHOD(Phalcon_Cli_Router, setDefaultAction);
PHP_METHOD(Phalcon_Cli_Router, handle);
PHP_METHOD(Phalcon_Cli_Router, getModuleName);
PHP_METHOD(Phalcon_Cli_Router, getTaskName);
PHP_METHOD(Phalcon_Cli_Router, getActionName);
PHP_METHOD(Phalcon_Cli_Router, getParams);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_cli_router_setdi, 0, 0, 1)
  ZEND_ARG_INFO(0, dependencyInjector)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_cli_router_setdefaultmodule, 0, 0, 1)
  ZEND_ARG_INFO(0, moduleName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_cli_router_setdefaulttask, 0, 0, 1)
  ZEND_ARG_INFO(0, taskName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_cli_router_setdefaultaction, 0, 0, 1)
  ZEND_ARG_INFO(0, actionName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_phalcon_cli_router_handle, 0, 0, 0)
  ZEND_ARG_INFO(0, arguments)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(phalcon_cli_router_method_entry) {
	PHP_ME(Phalcon_Cli_Router, setDI, arginfo_phalcon_cli_router_setdi, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, getDI, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, setDefaultModule, arginfo_phalcon_cli_router_setdefaultmodule, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, setDefaultTask, arginfo_phalcon_cli_router_setdefaulttask, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, setDefaultAction, arginfo_phalcon_cli_router_setdefaultaction, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, handle, arginfo_phalcon_cli_router_handle, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, getModuleName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, getTaskName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, getActionName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Phalcon_Cli_Router, getParams, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
