// IoBuilder.cpp

/**
 * @file
 * Implementation of IoBuilder.cpp
 */

#include "cla/model/generator/IoGenerator.hpp"
#include "cla/config/builder/module/IoBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using IJLabel = IoJsonLabels;


/************************************************
 * IoJsonBuilder private functions.
 ***********************************************/

PIO IoJsonBuilder::buildScalarIo_(const json& config) {
	htm::UInt nbInputs;
	std::vector<htm::UInt> inputDimensions;
	htm::UInt nbActiveBits;
	Values mins;
	Values maxs;
	int seed;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, IJLabel::PARAM_NB_INPUTS)) {
			value.get_to(nbInputs);
			continue;
		}

		if(KeyHelper::contain(key, IJLabel::PARAM_INPUT_DIMENSIONS)) {
			value.get_to(inputDimensions);
			continue;
		}

		if(KeyHelper::contain(key, IJLabel::PARAM_NB_ACTIVE_BITS)) {
			value.get_to(nbActiveBits);
			continue;
		}

		if(KeyHelper::contain(key, IJLabel::PARAM_MINS)) {
			value.get_to(mins);
			continue;
		}

		if(KeyHelper::contain(key, IJLabel::PARAM_MAXS)) {
			value.get_to(maxs);
			continue;
		}

		if(KeyHelper::contain(key, IJLabel::PARAM_SEED)) {
			value.get_to(seed);
			continue;
		}

		CLA_ALERT("Error: There are parameters that are not assumed.");
	}

	return IoGenerator<ScalarIO>::generate(
		nbInputs, inputDimensions, nbActiveBits, mins, maxs, seed
	);
}


/************************************************
 * IoJsonBuilder public functions.
 ***********************************************/

PIO IoJsonBuilder::buildIO(const json& config) {
	if(KeyHelper::contain(config, IJLabel::SCALAR_IO_LABEL)) {
		return buildScalarIo_(config.at(IJLabel::SCALAR_IO_LABEL));
	}

	CLA_ALERT("Error: There is no io that can be built.");
}

PIO IoJsonBuilder::buildIO(const std::string& type, const json& config) {
	if(KeyHelper::contain(type, IJLabel::SCALAR_IO_LABEL)) {
		return buildScalarIo_(config);
	}

	CLA_ALERT("Error: There is no io that can be built.");
}

} // namespace cla