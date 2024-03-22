// CompositeWaveEnv.cpp

/** 
 * @file
 * Implementation of CompositeWaveEnv.cpp
 */

#define _USE_MATH_DEFINES

#include <cmath>

#include "cla/environment/envs/CompositeWaveEnv.hpp"

namespace cla {

/************************************************
 * CompositeWave public functions.
 ***********************************************/

CompositeWave::CompositeWave(
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

const Values CompositeWave::getValues() const {
	static const Value TWO_PI = 2.0 * M_PI;

	const Step step = refresh_ * (getStep() / refresh_);
	const Value stepRate
		= static_cast<Value>((step + phase_) % period_) / static_cast<Value>(period_);

	return {
		  std::sin(      TWO_PI * stepRate)
		+ std::sin(3.0 * TWO_PI * stepRate) / 3.0
		+ std::sin(5.0 * TWO_PI * stepRate) / 5.0
		+ std::sin(7.0 * TWO_PI * stepRate) / 7.0
		+ std::sin(9.0 * TWO_PI * stepRate) / 9.0
	};
}

const json CompositeWave::getJsonConfig() const {
	json j;

	j[name]["period"] = period_;
	j[name]["phase"] = phase_;
	j[name]["refresh"] = refresh_;

	return j;
}


} // namespace cla