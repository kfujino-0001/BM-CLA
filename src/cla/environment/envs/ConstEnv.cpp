// ConstEnv.cpp

/** 
 * @file
 * Implementation of ConstEnv.cpp
 */

#include "cla/environment/envs/ConstEnv.hpp"

namespace cla {

/************************************************
 * ConstEnv public functions
 ***********************************************/

ConstEnv::ConstEnv(const Value value):
	value_(value), CoreEnv()
{
	_setDimension(1u);

	const Value distance = std::abs(value)/2.0 + 1.0;
	_setMaxs({value + distance});
	_setMins({value - distance});
}

ConstEnv::ConstEnv(
	const Value value,
	const Value min,
	const Value max
): 
	value_(value), CoreEnv()
{
	_setDimension(1u);
	_setMaxs({max});
	_setMins({min});
}

const Values ConstEnv::getValues() const {
	return {value_};
}

const json ConstEnv::getJsonConfig() const {
	json j;

	j[name]["value"] = value_;
	return j;
}


} // namespace cla