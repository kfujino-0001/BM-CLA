// CompositeWrapper.cpp

/** 
 * @file
 * Implementation of CompositeWrapper.cpp
 */

#include <numeric>

#include "cla/utils/Checker.hpp"
#include "cla/environment/wrappers/CompositeWrapper.hpp"

namespace cla {

/************************************************
 * CompositeWrapper public functions.
 ***********************************************/

CompositeWrapper::CompositeWrapper(const std::vector<PEnv>& envs) {
	initialize(envs);
}

void CompositeWrapper::initialize(const std::vector<PEnv>& envs) {
	CLA_ASSERT(!envs.empty());
	for(const auto& env : envs_)
		CLA_ASSERT(env)

	envs_ = envs;

	_setDimension( 
		std::accumulate
			(envs_.begin(), envs_.end(), static_cast<Dim>(0), 
			[](const Dim acc, const PEnv& env){return acc + env->getDimension();}
		)
	);

	Values minsEnvs, maxsEnvs;
	for(const auto& env : envs_) {
		const auto& mins = env->getMins();
		std::copy(mins.begin(), mins.end(), std::back_inserter(minsEnvs));

		const auto& maxs = env->getMaxs();
		std::copy(maxs.begin(), maxs.end(), std::back_inserter(maxsEnvs));
	}

	_setMins(minsEnvs);
	_setMaxs(maxsEnvs);

	reset();
}

void CompositeWrapper::reset() {
	CoreEnv::reset();

	for(auto&& env : envs_)
		env->reset();
}

const Values CompositeWrapper::getValues() const {
	Values results = {};

	for(auto&& env : envs_) {
		const Values& values = env->getValues();
		results.insert(results.end(), values.begin(), values.end());
	}

	CLA_ASSERT(results.size() == getDimension());
	return results;
}

const json CompositeWrapper::getJsonConfig() const {
	json j;

	for(const PEnv& env : envs_) 
		j[name]["envs"].emplace_back(env->getJsonConfig());

	return j;
}

void CompositeWrapper::increment() {
	for(auto&& env : envs_)
		env->increment();

	CoreEnv::increment();
}

const std::vector<PEnv>& CompositeWrapper::getEnvs() const {
	return envs_;
}


} // namespace cla