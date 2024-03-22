// CoreCLA.cpp

/**
 * @file
 * Implementation of CoreCLA.cpp
 */

#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/core/CoreCallback.hpp" // for cross-referencing
#include "cla/model/generator/CallbackGenerator.hpp"
#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"

namespace cla {

/************************************************
 * CoreCLA private functions.
 ***********************************************/

void CoreCLA::compile_(
	const Step nbStep,
	const int verbose,
	const bool learn,
	PEnv& env,
	PCallback& callback
) {
	CLA_ASSERT(env);
	CLA_ASSERT(callback);

	env->reset();
	Values inputs = env->getValues();
	Values predictions, nexts;

	callback->doStartProcessing(this);

	for(Step t = 0u; t < nbStep; ++t) {
		callback->doPreProcessing(t, inputs, this);
		predictions = feedforward(inputs, learn);

		env->increment();
		nexts = env->getValues();

		callback->doPostProcessing(t, inputs, nexts, predictions, this);

		feedback(nexts, learn);

		if(verbose > 0)
			printLog_(t, nexts, predictions);

		inputs = nexts;
	}

	callback->doEndProcessing(this);
}

void CoreCLA::printLog_(
	const Step step,
	const Values& nexts,
	const Values& predictions
) const {
	std::cout << "[step = " << step << ", "
			  << "actls = {" << nexts << "},"
			  << "preds = {" << predictions << "}]" << std::endl;
}


/************************************************
 * CoreCLA public functions.
 ***********************************************/

void CoreCLA::fit(
	const Step nbStep, 
	const int verbose, 
	PEnv& env
) {
	PCallback callback = CallbackGenerator<CoreCallback>::generate();
	compile_(nbStep, verbose, true, env, callback);
}

void CoreCLA::fit(
	const Step nbStep,
	const int verbose,
	PEnv& env,
	PCallback& callback
) {
	compile_(nbStep, verbose, true, env, callback);
}

void CoreCLA::test(
	const Step nbStep, 
	const int verbose, 
	PEnv& env
) {
	PCallback callback = CallbackGenerator<CoreCallback>::generate();
	compile_(nbStep, verbose, false, env, callback);
}

void CoreCLA::test(
	const Step nbStep,
	const int verbose,
	PEnv& env,
	PCallback& callback
) {
	compile_(nbStep, verbose, false, env, callback);
}


/************************************************
 * CoreCLA helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const CoreCLA& cla) {
	cla.summary(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const PCLA& pcla) {
	pcla->summary(os);
	return os;
}

} // namespace cla