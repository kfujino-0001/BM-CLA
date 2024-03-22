// TriEnv.cpp

/** 
 * @file
 * Implementation of TriEnv.cpp
 */

#include "cla/environment/envs/TriEnv.hpp"

namespace cla {

/************************************************
 * TriEnv public functions
 ***********************************************/

TriEnv::TriEnv(
	const Step period,
	const int phase,
	const Step refresh
):
	period_(period), refresh_(refresh), CoreEnv()
{
	_setDimension(1u);
	_setMaxs({1.0});
	_setMins({-1.0});
	
	phase_ = period_ + phase % period_;
}

const Values TriEnv::getValues() const {
	const Step step = refresh_ * (getStep() / refresh_);
	const Value stepRate 
		= static_cast<Value>((step + phase_) % period_) / static_cast<Value>(period_);

	return {static_cast<Value>(
		std::abs(-4.0 * (stepRate - 0.25 - std::floor(std::abs(2.0 * (stepRate - 0.25))))) - 1.0
	)};
}

const json TriEnv::getJsonConfig() const {
	json j;

	j[name]["period"] = period_;
	j[name]["phase"] = phase_;
	j[name]["refresh"] = refresh_;

	return j;
}


} // namespace cla