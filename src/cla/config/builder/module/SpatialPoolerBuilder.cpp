// SpatialPoolerBuilder.cpp

/**
 * @file
 * Implementation of SpatialPoolerBuilder.cpp
 */

#include "cla/model/generator/SpatialPoolerGenerator.hpp"
#include "cla/config/builder/module/SpatialPoolerBuilder.hpp"
#include "cla/config/utils/ConfigHelpers.hpp"
#include "cla/config/utils/JsonParamDefinition.hpp"
#include "cla/utils/Checker.hpp"

namespace cla {

using SPJLabel = SpatialPoolerJsonLabels;


/************************************************
 * SpatialPoolerJsonBuilder private functions.
 ***********************************************/

PSpatialPooler SpatialPoolerJsonBuilder::buildHtmSpatialPooler_(
	const json& config
) {
	std::vector<htm::UInt> inputDimensions;
	std::vector<htm::UInt> columnDimensions;
	htm::UInt potentialRadius;
	htm::Real potentialPct;
	bool globalInhibition;
	htm::Real localAreaDensity;
	htm::UInt stimulusThreshold;
	htm::Real synPermInactiveDec;
	htm::Real synPermActiveInc;
	htm::Real synPermConnected;
	htm::Real synInitPermanence;
	htm::Real minPctOverlapDutyCycles;
	htm::UInt dutyCyclePeriod;
	htm::Real boostStrength;
	htm::Int seed;
	htm::UInt spVerbosity;
	bool wrapAround;
	bool constSynInitPermanence;

	for(const auto& [key, value] : config.items()) {
		if(KeyHelper::contain(key, SPJLabel::PARAM_INPUT_DIMENSIONS)) {
			value.get_to(inputDimensions);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_COLUMN_DIMENSIONS)) {
			value.get_to(columnDimensions);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_POTENTIAL_RADIUS)) {
			value.get_to(potentialRadius);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_POTENTIAL_PCT)) {
			value.get_to(potentialPct);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_GLOBAL_INHIBITION)) {
			value.get_to(globalInhibition);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_LOCAL_AREA_DENSITY)) {
			value.get_to(localAreaDensity);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_STIMULUS_THRESHOLD)) {
			value.get_to(stimulusThreshold);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SYN_PERM_INACTIVE_DEC)) {
			value.get_to(synPermInactiveDec);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SYN_PERM_ACTIVE_INC)) {
			value.get_to(synPermActiveInc);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SYN_PERM_CONNECTED)) {
			value.get_to(synPermConnected);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SYN_INIT_PERMANENCE)) {
			value.get_to(synInitPermanence);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_MIN_PCT_OVERLAP_DUTY_CYCLES)) {
			value.get_to(minPctOverlapDutyCycles);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_DUTY_CYCLE_PERIOD)) {
			value.get_to(dutyCyclePeriod);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_BOOST_STRENGTH)) {
			value.get_to(boostStrength);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SEED)) {
			value.get_to(seed);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_SP_VERBOSITY)) {
			value.get_to(spVerbosity);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_WRAP_AROUND)) {
			value.get_to(wrapAround);
			continue;
		}

		if(KeyHelper::contain(key, SPJLabel::PARAM_CONST_SYN_INIT_PERMANENCE)) {
			value.get_to(constSynInitPermanence);
			continue;
		}

		CLA_ALERT("Error: There are parameters that are not assumed.");
	}


	return SpatialPoolerGenerator<HtmSpatialPooler>::generate(
		inputDimensions, columnDimensions, potentialRadius, potentialPct,
		globalInhibition, localAreaDensity, stimulusThreshold,
		synPermInactiveDec, synPermActiveInc, synPermConnected,
		synInitPermanence, minPctOverlapDutyCycles, dutyCyclePeriod,
		boostStrength, seed, spVerbosity, wrapAround,
		constSynInitPermanence
	);
}


/************************************************
 * SpatialPoolerJsonBuilder public functions.
 ***********************************************/

PSpatialPooler SpatialPoolerJsonBuilder::buildSpatialPooler(const json& config) {
	if(KeyHelper::contain(config, SPJLabel::HTM_SPATIAL_POOLER_LABEL)) {
		return buildHtmSpatialPooler_(config.at(SPJLabel::HTM_SPATIAL_POOLER_LABEL));
	}

	CLA_ALERT("Error: There is no spatial pooler that can be built.");
}

PSpatialPooler SpatialPoolerJsonBuilder::buildSpatialPooler(
	const std::string& type,
	const json& config
) {
	if(KeyHelper::contain(type, SPJLabel::HTM_SPATIAL_POOLER_LABEL)) {
		return buildHtmSpatialPooler_(config);
	}

	CLA_ALERT("Error: There is no spatial pooler that can be built.");
}



} // namespace cla