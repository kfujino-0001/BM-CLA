// EvalCallback.cpp

/** 
 * @file
 * Implementation of EvalCallback.cpp
 */

#include "cla/model/core/CoreCLA.hpp" // for cross-referencing
#include "cla/model/module/callback/EvalCallback.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

/************************************************
 * EvalCallback public functions.
 ***********************************************/

EvalCallback::EvalCallback(
	const Dim valueDimension,
	const Step nbEvalSteps,
	const int verbose
) {
	initialize(
		valueDimension,
		nbEvalSteps,
		verbose
	);
}

void EvalCallback::initialize(
	const Dim valueDimension,
	const Step nbEvalSteps,
	const int verbose
) {
	CLA_ASSERT(valueDimension > 0u);
	CLA_ASSERT(nbEvalSteps > 0u);

	valueDimension_ = valueDimension;
	nbEvalSteps_ = nbEvalSteps;
	verbose_ = verbose;

	evals_.resize(valueDimension_);
	for(auto& eval : evals_)
		eval.initialize();
}

void EvalCallback::doPostProcessing(
	const Step step,
	const Values& inputs,
	const Values& nexts,
	const Values& outputs,
	const CoreCLA* cla
) {
	CLA_ASSERT(nexts.size() == valueDimension_);
	CLA_ASSERT(outputs.size() == valueDimension_);

	for(std::size_t i = 0u; i < valueDimension_; ++i)
		evals_.at(i).add(outputs.at(i), nexts.at(i));

	if((verbose_ > 0) && ((step + 1u) % nbEvalSteps_ == 0u))
		printLog_(step);
}


/************************************************
 * EvalCallback private functions.
 ***********************************************/

void EvalCallback::printLog_(const Step step) {
	std::printf("step = [%6zu]: error = {", step);

	for(auto& eval : evals_)
		std::printf(
			"[mae = %.5lf, rmse = %.5lf],", 
			eval.getMAE(), 
			eval.getRMSE()
		);

	std::printf("}\n");

	for(auto& eval : evals_)
		eval.initialize();
}

} // namespace cla