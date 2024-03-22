// CoreEnv.cpp

/** 
 * @file
 * Implementation of CoreEnv.cpp
 */

#include "cla/utils/Checker.hpp"
#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/************************************************
 * CoreEnv protected functions.
 ***********************************************/

void CoreEnv::_setDimension(const Dim dimension){
	dimension_ = dimension;	
}

void CoreEnv::_setMaxs(const Values& maxs) {
	maxs_ = maxs;
}

void CoreEnv::_setMins(const Values& mins) {
	mins_ = mins;
}


/************************************************
 * CoreEnv virtual public functions.
 ***********************************************/

void CoreEnv::reset() {
	t_ = 0u;
}

const Step& CoreEnv::getStep() const {
	return t_;
}

void CoreEnv::increment() {
	++t_;
}

const json CoreEnv::getJsonConfig() const {
	return json::object();
}


/************************************************
 * CoreEnv public functions.
 ***********************************************/

const Dim CoreEnv::getDimension() const {
	CLA_ASSERT(dimension_ != 0u);
	return dimension_;
}

const Values& CoreEnv::getMaxs() const {
	CLA_ASSERT(dimension_ == maxs_.size());
	return maxs_;
}

const Values& CoreEnv::getMins() const {
	CLA_ASSERT(dimension_ == mins_.size());
	return mins_;
}

void CoreEnv::summary(std::ostream& os) const {
	const unsigned int indent_ = 4;

	os << getJsonConfig().dump(indent_);
}


/************************************************
 * CoreEnv helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const CoreEnv& env) {
	env.summary(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const PEnv& penv) {
	CLA_ASSERT(penv);

	penv->summary(os);
	return os;
}

} // namespace cla