// SawEnv.cpp

/** 
 * @file
 * Implementation of SawEnv.cpp
 */

#include "cla/environment/envs/SawEnv.hpp"

namespace cla {

/************************************************
 * SawEnv public functions
 ***********************************************/

SawEnv::SawEnv(
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

const Values SawEnv::getValues() const {
	const Step step = refresh_ * (getStep() / refresh_);
	const Value stepRate
		= static_cast<Value>((step + phase_) % period_) / static_cast<Value>(period_);

	return {static_cast<Value>(
		2.0 * stepRate - 2.0 * std::floor(2.0 * stepRate)
	)};
}

const json SawEnv::getJsonConfig() const {
	json j;

	j[name]["period"] = period_;
	j[name]["phase"] = phase_;
	j[name]["refresh"] = refresh_;

	return j;
}


} // namespace cla