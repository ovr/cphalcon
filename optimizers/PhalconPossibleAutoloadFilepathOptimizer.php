<?php

/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
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

namespace Phalcon\Optimizers;

use Zephir\Optimizers\OptimizerAbstract,
	Zephir\Call,
	Zephir\CompilationContext,
	Zephir\CompilerException,
	Zephir\CompiledExpression;

class PhalconPossibleAutoloadFilepathOptimizer
	extends OptimizerAbstract
{
	/**
	 * Optimize function call
	 *
	 * @param array $expression
	 * @param Call $call
	 * @param CompilationContext $context
	 * @return bool|mixed|CompiledExpression
	 * @throws CompilerException
	 */
	public function optimize(array $expression, Call $call, CompilationContext $context)
	{

		if (!isset($expression['parameters'])) {
			return false;
		}

		if (count($expression['parameters']) != 4) {
			throw new CompilerException("phalcon_possible_autoload_filepath only accepts three parameter", $expression);
		}

		/**
		 * Process the expected symbol to be returned
		 */
		$call->processExpectedReturn($context);

		$symbolVariable = $call->getSymbolVariable();
		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Returned values by functions can only be assigned to variant variables", $expression);
		}

		if ($call->mustInitSymbolVariable()) {
			$symbolVariable->initVariant($context);
		}

		$context->headersManager->add('kernel/array');

		$symbolVariable->setDynamicTypes('array');

		$resolvedParams = $call->getReadOnlyResolvedParams($expression['parameters'], $context, $expression);
		//$context->codePrinter->output('zephir_fast_array_merge(' . $symbolVariable->getName() . ', &(' . $resolvedParams[0] . '), &(' . $resolvedParams[1] . ') TSRMLS_CC);');
		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
