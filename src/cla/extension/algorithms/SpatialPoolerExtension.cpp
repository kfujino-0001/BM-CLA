// SpatialPoolerExtension.cpp

/** 
 * @file
 * Implementation of SpatialPoolerExtension
 */

#include <iostream>
#include <fstream>
#include <algorithm>

#include <cla/extension/algorithms/SpatialPoolerExtension.hpp>

namespace htm {

/**
 * SpatialPoolerExtensionParameters methods.
 */
std::ostream& htm::operator<<(
	std::ostream& os, 
	const SpatialPoolerExtensionParameters& params
) {

	os << "\tpotentialRadius\t\t\t= " << params.potentialRadius << "u" << std::endl;
	os << "\tpotentialPct\t\t\t= " << params.potentialPct << "f" << std::endl;
	os << "\tglobalInhibition\t\t= " << ((params.globalInhibition) ? "true" : "false") << std::endl;
	os << "\tlocalAreaDensity\t\t= " << params.localAreaDensity << "f" << std::endl;
	os << "\tstimulusThreshold\t\t= " << params.stimulusThreshold << "u" << std::endl;
	os << "\tsynPermInactiveDec\t\t= " << params.synPermInactiveDec << "f" << std::endl;
	os << "\tsynPermActiveInc\t\t= " << params.synPermActiveInc << "f" << std::endl;
	os << "\tsynPermConnected\t\t= " << params.synPermConnected << "f" << std::endl;
	os << "\tsynInitPermanence\t\t= " << params.synInitPermanence << "f" << std::endl;
	os << "\tminPctOverlapDutyCycles\t\t= " << params.minPctOverlapDutyCycles << "f" << std::endl;
	os << "\tdutyCyclePeriod\t\t\t= " << params.dutyCyclePeriod << "u" << std::endl;
	os << "\tboostStrength\t\t\t= " << params.boostStrength << "f" << std::endl;
	os << "\tseed\t\t\t\t= " << params.seed << std::endl;
	os << "\tspVerbosity\t\t\t= " << params.spVerbosity << "f" << std::endl;
	os << "\twrapAround\t\t\t= " << ((params.wrapAround) ? "true" : "false") << std::endl;
	os << "\tconstSynInitPermanence\t\t= " << ((params.constSynInitPermanence) ? "true" : "false") << std::endl;
	os << std::endl;

	return os;
}

void htm::to_json(
	json& j, 
	const SpatialPoolerExtensionParameters& p
){
	j = json{
		{"potentialRadius", p.potentialRadius},
		{"potentialPct", p.potentialPct},
		{"globalInhibition", p.globalInhibition},
		{"localAreaDensity", p.localAreaDensity},
		{"stimulusThreshold", p.stimulusThreshold},
		{"synPermInactiveDec", p.synPermInactiveDec},
		{"synPermActiveInc", p.synPermActiveInc},
		{"synPermConnected", p.synPermConnected},
		{"synInitPermanence", p.synInitPermanence},
		{"minPctOverlapDutyCycles", p.minPctOverlapDutyCycles},
		{"dutyCyclePeriod", p.dutyCyclePeriod},
		{"boostStrength", p.boostStrength},
		{"spVerbosity", p.spVerbosity},
		{"wrapAround", p.wrapAround},
		{"constSynInitPermanence", p.constSynInitPermanence}
	};
}

void htm::from_json(
	const json& j,
	SpatialPoolerExtensionParameters& p
){
	j.at("potentialRadius").get_to(p.potentialRadius);
	j.at("potentialPct").get_to(p.potentialPct);
	j.at("globalInhibition").get_to(p.globalInhibition);
	j.at("localAreaDensity").get_to(p.localAreaDensity);
	j.at("stimulusThreshold").get_to(p.stimulusThreshold);
	j.at("synPermInactiveDec").get_to(p.synPermInactiveDec);
	j.at("synPermActiveInc").get_to(p.synPermActiveInc);
	j.at("synPermConnected").get_to(p.synPermConnected);
	j.at("synInitPermanence").get_to(p.synInitPermanence);
	j.at("minPctOverlapDutyCycles").get_to(p.minPctOverlapDutyCycles);
	j.at("dutyCyclePeriod").get_to(p.dutyCyclePeriod);
	j.at("boostStrength").get_to(p.boostStrength);
	j.at("spVerbosity").get_to(p.spVerbosity);
	j.at("wrapAround").get_to(p.wrapAround);
	j.at("constSynInitPermanence").get_to(p.constSynInitPermanence);
}


/**
 * SpatialPoolerExtension methods
 */
SpatialPoolerExtension::SpatialPoolerExtension(
	const std::vector<htm::UInt>& inputDimensions,
	const std::vector<htm::UInt>& columnDimensions,
	const htm::UInt potentialRadius,
	const htm::Real potentialPct,
	const bool globalInhibition,
	const htm::Real localAreaDensity,
	const htm::UInt stimulusThreshold,
	const htm::Real synPermInactiveDec,
	const htm::Real synPermActiveInc,
	const htm::Real synPermConnected,
	const htm::Real synInitPermanence,
	const htm::Real minPctOverlapDutyCycles,
	const htm::UInt dutyCyclePeriod,
	const htm::Real boostStrength,
	const htm::Int seed,
	const htm::UInt spVerbosity,
	const bool wrapAround,
	const bool constSynInitPermanence
){
	
	initialize(
		inputDimensions,
		columnDimensions,
		potentialRadius,
		potentialPct,
		globalInhibition,
		localAreaDensity,
		stimulusThreshold,
		synPermInactiveDec,
		synPermActiveInc,
		synPermConnected,
		synInitPermanence,
		minPctOverlapDutyCycles,
		dutyCyclePeriod,
		boostStrength,
		seed,
		spVerbosity,
		wrapAround,
		constSynInitPermanence
	);
}

SpatialPoolerExtension::SpatialPoolerExtension(const SpatialPoolerExtensionParameters& params){
	initialize(
		params.inputDimensions,
		params.columnDimensions,
		params.potentialRadius,
		params.potentialPct,
		params.globalInhibition,
		params.localAreaDensity,
		params.stimulusThreshold,
		params.synPermInactiveDec,
		params.synPermActiveInc,
		params.synPermConnected,
		params.synInitPermanence,
		params.minPctOverlapDutyCycles,
		params.dutyCyclePeriod,
		params.boostStrength,
		params.seed,
		params.spVerbosity,
		params.wrapAround,
		params.constSynInitPermanence
	);
}

void SpatialPoolerExtension::initialize(const SpatialPoolerExtensionParameters& params) {
	initialize(
		params.inputDimensions,
		params.columnDimensions,
		params.potentialRadius,
		params.potentialPct,
		params.globalInhibition,
		params.localAreaDensity,
		params.stimulusThreshold,
		params.synPermInactiveDec,
		params.synPermActiveInc,
		params.synPermConnected,
		params.synInitPermanence,
		params.minPctOverlapDutyCycles,
		params.dutyCyclePeriod,
		params.boostStrength,
		params.seed,
		params.spVerbosity,
		params.wrapAround,
		params.constSynInitPermanence
	);
}

void SpatialPoolerExtension::initialize(
	const std::vector<htm::UInt>& inputDimensions,
	const std::vector<htm::UInt>& columnDimensions,
	const htm::UInt potentialRadius,
	const htm::Real potentialPct,
	const bool globalInhibition,
	const htm::Real localAreaDensity,
	const htm::UInt stimulusThreshold,
	const htm::Real synPermInactiveDec ,
	const htm::Real synPermActiveInc,
	const htm::Real synPermConnected,
	const htm::Real synInitPermanence,
	const htm::Real minPctOverlapDutyCycles,
	const htm::UInt dutyCyclePeriod,
	const htm::Real boostStrength,
	const htm::Int seed,
	const htm::UInt spVerbosity,
	const bool wrapAround,
	const bool constSynInitPermanence
){

	version_ = 2u;

	numInputs_ = 1u;
	inputDimensions_.clear();

	for (auto& inputDimension : inputDimensions) {
		NTA_CHECK(inputDimension > 0) << "Input dimensions must be positive integers!";
		numInputs_ *= inputDimension;
		inputDimensions_.push_back(inputDimension);
	}

	numColumns_ = 1u;
	columnDimensions_.clear();
	for (auto& columnDimension : columnDimensions) {
		NTA_CHECK(columnDimension > 0) << "Column dimensions must be positive integers!";
		numColumns_ *= columnDimension;
		columnDimensions_.push_back(columnDimension);
	}


	NTA_CHECK(numColumns_ > 0);
	NTA_CHECK(numInputs_ > 0);

	// 1D input produces 1D output; 2D => 2D, etc. //TODO allow nD -> mD conversion
	NTA_CHECK(inputDimensions_.size() == columnDimensions_.size());

	NTA_CHECK(localAreaDensity > 0 && localAreaDensity <= MAX_LOCALAREADENSITY);

	setLocalAreaDensity(localAreaDensity);


	rng_ = htm::Random(seed);

	potentialRadius_ = potentialRadius > numInputs_ ? numInputs_ : potentialRadius;
	NTA_CHECK(potentialPct > 0 && potentialPct <= 1);
	potentialPct_ = potentialPct;
	globalInhibition_ = globalInhibition;
	stimulusThreshold_ = stimulusThreshold;
	synPermInactiveDec_ = synPermInactiveDec;
	synPermActiveInc_ = synPermActiveInc;
	synPermBelowStimulusInc_ = synPermConnected / 10.0f;
	synPermConnected_ = synPermConnected;
	synInitPermanence_ = synInitPermanence;
	minPctOverlapDutyCycles_ = minPctOverlapDutyCycles;
	dutyCyclePeriod_ = dutyCyclePeriod;
	boostStrength_ = boostStrength;
	spVerbosity_ = spVerbosity;
	wrapAround_ = wrapAround;
	constSynInitPermanence_ = constSynInitPermanence;

	updatePeriod_ = 50u;
	initConnectedPct_ = 0.5f; //FIXME make SP's param, and much lower 0.01 https://discourse.numenta.org/t/spatial-pooler-implementation-for-mnist-dataset/2317/25?u=breznak 
	iterationNum_ = 0u;
	iterationLearnNum_ = 0u;

	overlapDutyCycles_.assign(numColumns_, 0); //TODO make all these sparse or rm to reduce footprint
	activeDutyCycles_.assign(numColumns_, 0);
	minOverlapDutyCycles_.assign(numColumns_, 0.0);
	boostFactors_.assign(numColumns_, 1.0); //1 is neutral value for boosting
	boostedOverlaps_.resize(numColumns_);

	inhibitionRadius_ = 0;

	connections_.initialize(numColumns_, synPermConnected_);

	for (htm::Size i = 0; i < numColumns_; ++i) {
		connections_.createSegment((htm::CellIdx)i, 1 /* max segments per cell is fixed for SP to 1 */);

		// Note: initMapPotential_ & initPermanence_ return dense arrays.

		std::vector<htm::UInt> potential = initMapPotential_((htm::UInt)i, wrapAround_);
		std::vector<htm::Real> perm;

		if(constSynInitPermanence_){
			perm.resize(numInputs_);
			std::fill(perm.begin(), perm.end(), synInitPermanence_);
		}else {
			perm = initPermanence_(potential, initConnectedPct_);
		}

		for (htm::UInt presyn = 0; presyn < numInputs_; presyn++) {

			if (potential[presyn])
				connections_.createSynapse((htm::Segment)i, presyn, perm[presyn]);
		}

		connections_.raisePermanencesToThreshold((htm::Segment)i, stimulusThreshold_);
	}

	updateInhibitionRadius_();

	if (spVerbosity_ > 0) {
		printParameters();
		std::cout << "CPP SP seed                 = " << seed << std::endl;
	}
}

const bool SpatialPoolerExtension::getConstSynInitPermanence() const {
	return constSynInitPermanence_;
}

const htm::Real SpatialPoolerExtension::getSynInitPermanence() const {
	return synInitPermanence_;
}


} // namespace htm