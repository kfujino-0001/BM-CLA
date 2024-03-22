// LogisticMapEnv.cpp

/** 
 * @file
 * Implementation of LogisticMapEnv.cpp
 */

#include "cla/environment/envs/LogisticMapEnv.hpp"

namespace cla {

/************************************************
 * LogisticMapEnv public functions
 ***********************************************/

LogisticMapEnv::LogisticMapEnv(
	const Value alpha,
	const Value init
):
	alpha_(alpha), previous_(init), CoreEnv()
{
	_setDimension(1u);
	_setMaxs({1.0});
	_setMins({0.0});
}

const Values LogisticMapEnv::getValues() const {
	const Value value 
		= alpha_ * previous_ * (1.0 - previous_);

	previous_ = value;

	return {value};
}

const json LogisticMapEnv::getJsonConfig() const {
	json j;

	j[name]["alpha"] = alpha_;
	j[name]["init"] = previous_;

	return j;
}


} // namespace cla