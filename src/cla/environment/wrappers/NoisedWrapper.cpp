// NoisedWrapper.cpp

/** 
 * @file
 * Implementation of NoisedWrapper.cpp
 */

#include <random>
#include <algorithm> // for clamp

#include "cla/utils/Checker.hpp"
#include "cla/environment/wrappers/NoisedWrapper.hpp"

namespace cla {

/************************************************
 * NoisedWrapper public functions.
 ***********************************************/

NoisedWrapper::NoisedWrapper(
	const PEnv& env,
	const Value sigma,
	const int seed
) {
	initialize(env, sigma, seed);
}

void NoisedWrapper::initialize(
	const PEnv& env,
	const Value sigma,
	const int seed
) {
	CLA_ASSERT(env);
	env_ = env;
	sigma_ = sigma;
	seed_ = seed;
	
	mt_.seed(seed_);

	_setDimension(env_->getDimension());

	Values mins = env_->getMins();
	Values maxs = env_->getMaxs();

	for(Dim i = 0u, size = env_->getDimension(); i < size; ++i) {
		// Extend the maximum value by 3 sigma from the original environmental maximum.
		// In the normal distribution, 99.7% of the points lie in the 3 sigma range.
		mins.at(i) = mins.at(i) - 3.0 * sigma_;
		maxs.at(i) = maxs.at(i) + 3.0 * sigma_;
	}
	
	reset();
}

void NoisedWrapper::reset() {
	CoreEnv::reset();
	env_->reset();
}

const Values NoisedWrapper::getValues() const {
	std::normal_distribution<> dist(0.0, sigma_);

	const Values& values = env_->getValues();
	const Values& mins = env_->getMins();
	const Values& maxs = env_->getMaxs();

	Values results(values.size());

	for(std::size_t i = 0u, size = values.size(); i < size; ++i) {
		results.at(i) = values.at(i) + static_cast<Value>(dist(mt_));
		results.at(i) = std::clamp(results.at(i), mins.at(i), maxs.at(i));
	}

	return results;
}

const json NoisedWrapper::getJsonConfig() const {
	json j;

	j[name]["env"] = env_->getJsonConfig();
	j[name]["sigma"] = sigma_;
	j[name]["seed"] = seed_;

	return j;
}

void NoisedWrapper::increment() {
	env_->increment();
	CoreEnv::increment();
}

const PEnv& NoisedWrapper::getEnv() const {
	return env_;
}

} // namespace cla