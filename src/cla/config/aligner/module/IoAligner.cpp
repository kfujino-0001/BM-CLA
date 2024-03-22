// IoAligner.cpp

/** 
 * @file
 * Implementation of IoAligner.cpp
 */

#include "cla/config/aligner/module/IoAligner.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using IJLabel = IoJsonLabels;


/************************************************
 * IoJsonAligner private functions.
 **********************************************/

void IoJsonAligner::initializeByScalarIO_(
	const std::string& type,
	const json& config
) {
	type_ = type;
	config_ = config;

	isMinsUpdated_ = false;
	isMaxsUpdated_ = false;
	isSeedUpdated_ = false;

}

void IoJsonAligner::alignScalarIOConfig_() {
	if(isMinsUpdated_) 
		ConfigHelper::assign(config_, IJLabel::PARAM_MINS, mins_);
	
	if(isMaxsUpdated_) 
		ConfigHelper::assign(config_, IJLabel::PARAM_MAXS, maxs_);

	if(isSeedUpdated_) 
		ConfigHelper::assign(config_, IJLabel::PARAM_SEED, seed_);
		
}


/************************************************
 * IoJsonAligner public functions.
 ***********************************************/

IoJsonAligner::IoJsonAligner(
	const json& config
) {
	initialize(config);
}

IoJsonAligner::IoJsonAligner(
	const std::string& type,
	const json& config
) {
	initialize(type, config);
}

void IoJsonAligner::initialize(const json& config) {
	if(KeyHelper::contain(config, IJLabel::SCALAR_IO_LABEL)) {
		initializeByScalarIO_(IJLabel::SCALAR_IO_LABEL, config.at(IJLabel::SCALAR_IO_LABEL));
		return;
	}

	CLA_ALERT("Error: There is no module that can be aligned in IoJsonAligner.");
}

void IoJsonAligner::initialize(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, IJLabel::SCALAR_IO_LABEL)) {
		initializeByScalarIO_(type, config);
		return;
	}

	CLA_ALERT("Error: There is no module that can be aligned in IoJsonAligner.");
}

void IoJsonAligner::align() {
	if(KeyHelper::contain(type_, IJLabel::SCALAR_IO_LABEL)) {
		alignScalarIOConfig_();
		return;
	}

	CLA_ALERT("Error: There is no module that can be aligned in IoJsonAligner.");
}

const std::string& IoJsonAligner::getType() const {
	return type_;
}

const json& IoJsonAligner::getConfig() const {
	return config_;
}


/************************************************
 * IoJsonAligner public setters.
 ***********************************************/

void IoJsonAligner::setMins(const Values& mins) {
	mins_ = mins;
	isMinsUpdated_ = true;
}

void IoJsonAligner::setMaxs(const Values& maxs) {
	maxs_ = maxs;
	isMaxsUpdated_ = true;
}

void IoJsonAligner::setSeed(const int seed) {
	seed_ = seed;
	isSeedUpdated_ = true;
}


/************************************************
 * IoJsonAligner public getters.
 ***********************************************/

const std::vector<htm::UInt> IoJsonAligner::getDimensions() const {
	std::vector<htm::UInt> dimension;
	
	CLA_CHECK(
		KeyHelper::contain(config_, IJLabel::PARAM_INPUT_DIMENSIONS), 
		"Error: Config dose not have the assumed parameter."
	);

	config_.at(IJLabel::PARAM_INPUT_DIMENSIONS).get_to(dimension);
	return dimension;
}

} // namespace cla