// HtmSpatialPooler.cpp

/**
 * @file
 * Implementation of HtmSpatialPooler.cpp
 */

#include "cla/model/module/sp/HtmSpatialPooler.hpp"

namespace cla {

/************************************************
 * HtmSpatialPooler public functions.
 ***********************************************/

HtmSpatialPooler::HtmSpatialPooler(
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
) {
	initialize(
		inputDimensions, columnDimensions, potentialRadius, potentialPct,
		globalInhibition, localAreaDensity, stimulusThreshold,
		synPermInactiveDec, synPermActiveInc, synPermConnected,
		synInitPermanence, minPctOverlapDutyCycles, dutyCyclePeriod,
		boostStrength, seed, spVerbosity, wrapAround,
		constSynInitPermanence
	);
}

void HtmSpatialPooler::initialize(
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
) {
	sp_.initialize(
		inputDimensions, columnDimensions, potentialRadius, potentialPct,
		globalInhibition, localAreaDensity, stimulusThreshold,
		synPermInactiveDec, synPermActiveInc, synPermConnected,
		synInitPermanence, minPctOverlapDutyCycles, dutyCyclePeriod,
		boostStrength, seed, spVerbosity, wrapAround,
		constSynInitPermanence
	);
}

void HtmSpatialPooler::summary(std::ostream& os) const {
	os << "HtmSpatialPooler";
}

void HtmSpatialPooler::detail(std::ostream& os) const {
	os << " -- HtmSpatialPooler -------------------------------------"
	   << std::endl
	   << std::endl;
	
	os << "\tpotentialRadius\t\t\t= " << sp_.getPotentialRadius() << "u" << std::endl;
	os << "\tpotentialPct\t\t\t= " << sp_.getPotentialPct() << "f" << std::endl;
	os << "\tglobalInhibition\t\t= " << ((sp_.getGlobalInhibition()) ? "true" : "false") << std::endl;
	os << "\tlocalAreaDensity\t\t= " << sp_.getLocalAreaDensity() << "f" << std::endl;
	os << "\tstimulusThreshold\t\t= " << sp_.getStimulusThreshold() << "u" << std::endl;
	os << "\tsynPermInactiveDec\t\t= " << sp_.getSynPermInactiveDec() << "f" << std::endl;
	os << "\tsynPermActiveInc\t\t= " << sp_.getSynPermActiveInc() << "f" << std::endl;
	os << "\tsynPermConnected\t\t= " << sp_.getSynPermConnected() << "f" << std::endl;
	os << "\tsynInitPermanence\t\t= " << sp_.getSynInitPermanence() << "f" << std::endl;
	os << "\tminPctOverlapDutyCycles\t\t= " << sp_.getMinPctOverlapDutyCycles() << "f" << std::endl;
	os << "\tdutyCyclePeriod\t\t\t= " << sp_.getDutyCyclePeriod() << "u" << std::endl;
	os << "\tboostStrength\t\t\t= " << sp_.getBoostStrength() << "f" << std::endl;
	os << "\tspVerbosity\t\t\t= " << sp_.getSpVerbosity() << "f" << std::endl;
	os << "\twrapAround\t\t\t= " << ((sp_.getWrapAround()) ? "true" : "false") << std::endl;
	os << "\tconstSynInitPermanence\t\t= " << ((sp_.getConstSynInitPermanence()) ? "true" : "false") << std::endl;
	os << std::endl;
}

void HtmSpatialPooler::compute(
	const htm::SDR& activeBits,
	const bool learn,
	htm::SDR& activeColumns
) {
	sp_.compute(activeBits, learn, activeColumns);
}

const std::vector<htm::CellIdx> HtmSpatialPooler::bitsForColumn(
	const htm::CellIdx column
) const {
	std::set<htm::CellIdx> bits;
	const auto& synapses = sp_.connections.synapsesForSegment(column);

	for(const htm::Synapse synapse : synapses) {
		const auto& synData = sp_.connections.dataForSynapse(synapse);

		if(synData.permanence > sp_.getSynPermConnected()) {
			bits.insert(synData.presynapticCell);
		}
	}

	return std::vector(bits.begin(), bits.end());
}

const htm::Connections& HtmSpatialPooler::getConnections() const {
	return sp_.connections;
}

} // namespace cla