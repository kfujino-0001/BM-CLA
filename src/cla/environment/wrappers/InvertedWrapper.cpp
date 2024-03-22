// InvertedWrapper.cpp

/** 
 * @file
 * Implementation of InvertedWrapper.cpp
 */

#include <algorithm> // for for_each

#include "cla/utils/Checker.hpp"
#include "cla/environment/wrappers/InvertedWrapper.hpp"

namespace cla {

/************************************************
 * InvertedWrapper public functions.
 ***********************************************/

InvertedWrapper::InvertedWrapper(const PEnv& env) {
	initialize(env);
}

void InvertedWrapper::initialize(const PEnv& env) {
	CLA_ASSERT(env);
	env_ = env;

	_setDimension(env_->getDimension());

	Values mins = env_->getMins();
	std::for_each(mins.begin(), mins.end(), minus_);
	_setMaxs(mins);

	Values maxs = env_->getMaxs();
	std::for_each(maxs.begin(), maxs.end(), minus_);
	_setMins(maxs);

	reset();
}

void InvertedWrapper::reset() {
	CoreEnv::reset();
	env_->reset();
}

const Values InvertedWrapper::getValues() const {
	Values values = env_->getValues();
	std::for_each(values.begin(), values.end(), minus_);

	return values;
}

const json InvertedWrapper::getJsonConfig() const {
	json j;

	j[name]["env"] = env_->getJsonConfig();

	return j;
}

void InvertedWrapper::increment() {
	env_->increment();
	CoreEnv::increment();
}

const PEnv& InvertedWrapper::getEnv() const {
	return env_;
}

} // namespace cla