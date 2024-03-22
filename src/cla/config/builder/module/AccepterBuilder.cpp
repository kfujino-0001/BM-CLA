// AccepterBuilder.cpp

/** 
 * @file
 * Implementation of AccepterBuilder.cpp
 */

#include "cla/model/generator/AccepterGenerator.hpp"
#include "cla/config/builder/module/AccepterBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using AJLabel = AccepterJsonLabels;


/************************************************
 * AccepterJsonBuilder private functions.
 ***********************************************/

PAccepter AccepterJsonBuilder::buildFullAccepter_(const json& config) {
	return AccepterGenerator<FullAccepter>::generate();
}

PAccepter AccepterJsonBuilder::buildIntensityAccepter_(const json& config) {
	htm::UInt intensityThreshold;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, AJLabel::PARAM_INTENSITY_THRESHOLD)) {
			value.get_to(intensityThreshold);
			continue;
		}

		CLA_ALERT("Error: There are parameters that are not assumed.");
	}

	return AccepterGenerator<IntensityAccepter>::generate(intensityThreshold);
}


/************************************************
 * AccepterJsonBuilder public functions.
 ***********************************************/


PAccepter AccepterJsonBuilder::buildAccepter(const json& config) {
	if(KeyHelper::contain(config, AJLabel::FULL_ACCEPTER_LABEL)) {
		return buildFullAccepter_(config.at(AJLabel::FULL_ACCEPTER_LABEL));
	}

	if(KeyHelper::contain(config, AJLabel::INTENSITY_ACCEPTER_LABEL)) {
		return buildIntensityAccepter_(config.at(AJLabel::INTENSITY_ACCEPTER_LABEL));
	}

	CLA_ALERT("Error: There is no accepter that can be built.");
}

PAccepter AccepterJsonBuilder::buildAccepter(
	const std::string& type, 
	const json& config
) {
	if(KeyHelper::contain(type, AJLabel::FULL_ACCEPTER_LABEL)) {
		return buildFullAccepter_(config);
	}

	if(KeyHelper::contain(type, AJLabel::INTENSITY_ACCEPTER_LABEL)) {
		return buildIntensityAccepter_(config);
	}

	CLA_ALERT("Error: There is no accepter that can be built.");
}


} // namespace cla