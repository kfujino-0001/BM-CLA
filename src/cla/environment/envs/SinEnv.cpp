// SinEnv.cpp

/** 
 * @file
 * Implementation of SinEnv.cpp
 */

#define _USE_MATH_DEFINES

#include <cmath>

#include "cla/environment/envs/SinEnv.hpp"

namespace cla {

/************************************************
 * SinEnv public functions
 ***********************************************/

SinEnv::SinEnv(
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

const Values SinEnv::getValues() const {
	const Step step = refresh_ * (getStep() / refresh_);
	const Value stepRate
		= static_cast<Value>((step + phase_) % period_) / static_cast<Value>(period_);

	return {static_cast<Value>(std::sin(2.0 * M_PI * stepRate))};
}

const json SinEnv::getJsonConfig() const {
	json j;

	j[name]["period"] = period_;
	j[name]["phase"] = phase_;
	j[name]["refresh"] = refresh_;

	return j;
}


} // namespace cla