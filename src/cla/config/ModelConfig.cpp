// ModelConfig.cpp

/** 
 * @file
 * Implementation of ModelConfig.cpp
 */

#include "cla/config/ModelConfig.hpp"

namespace cla {

/************************************************
 * JsonConfig public functions.
 ***********************************************/

JsonConfig::JsonConfig(const json& config) {
	initialize_(config);
}

ModelJsonAligner& JsonConfig::getModel() {
	return aligner_;
}

const json& JsonConfig::getConfig() const {
	return config_;
}

void JsonConfig::align() {
	aligner_.align();
	config_.at(aligner_.getType()) = aligner_.getConfig();
}

PCLA JsonConfig::buildModel() {
	align();
	return ModelJsonBuilder::buildModel(config_);
}


/************************************************
 * JsonConfig private function.
 ***********************************************/

void JsonConfig::initialize_(const json& config) {
	config_ = config;
	aligner_.initialize(config);
}



/************************************************
 * JsonConfig helper functions.
 ***********************************************/

std::ostream& operator<<(std::ostream& os, const JsonConfig& config) {
	os << config.config_;
	return os;
}

std::istream& operator>>(std::istream& is, JsonConfig& config) {
	json j;
	is >> j;
	config.initialize_(j);
	return is;
}

} // namespace cla