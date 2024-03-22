// ScalarIO.cpp

/**
 * @file
 * Implementation of ScalarIO.cpp
 */

#include "cla/model/module/io/ScalarIO.hpp"
#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"

namespace cla {

/************************************************
 * ScalarIO public functions.
 ***********************************************/

ScalarIO::ScalarIO(
	const htm::UInt nbInputs,
	const std::vector<htm::UInt>& inputDimensions,
	const htm::UInt nbActiveBits,
	const Values& mins,
	const Values& maxs,
	const int seed
) {
	initialize(nbInputs, inputDimensions, nbActiveBits, mins, maxs, seed);
}

void ScalarIO::initialize(
	const htm::UInt nbInputs,
	const std::vector<htm::UInt>& inputDimensions,
	const htm::UInt nbActiveBits,
	const Values& mins,
	const Values& maxs,
	const int seed
) {
	// Check the parameters.
	CLA_ASSERT(nbInputs > 0u);
	CLA_ASSERT(static_cast<htm::UInt>(mins.size()) == nbInputs);
	CLA_ASSERT(static_cast<htm::UInt>(maxs.size()) == nbInputs);

	for(htm::UInt i = 0u; i < nbInputs; ++i)
		CLA_ASSERT(mins.at(i) < maxs.at(i));


	const htm::UInt inputDimensionSize = htm::SDR(inputDimensions).size;

	CLA_ASSERT(inputDimensions.size() > 0u);
	// The number of bits to be activated must be less than the total
	// number of bits.
	CLA_ASSERT(nbActiveBits <= inputDimensionSize);

	CLA_ASSERT((nbActiveBits % nbInputs) == 0u);
	CLA_ASSERT((inputDimensionSize % nbInputs) == 0u);


	// Initialize the parameters.
	nbInputs_ = nbInputs;
	inputDimensions_ = inputDimensions;
	nbActiveBits_ = nbActiveBits;
	mins_ = mins;
	maxs_ = maxs;
	seed_ = seed;


	// Initialize the encoders and decoders.
	encoders_.clear();
	decoders_.clear();

	encoders_.resize(nbInputs);
	decoders_.resize(nbInputs);

	htm::ScalarEncoderParameters params;
	params.activeBits = nbActiveBits / nbInputs;
	params.size = inputDimensionSize / nbInputs;

	for(htm::UInt i = 0u; i < nbInputs; ++i) {
		params.minimum = mins.at(i);
		params.maximum = maxs.at(i);

		encoders_.at(i).initialize(params);
		decoders_.at(i).initialize(params);
	}
}

void ScalarIO::summary(std::ostream& os) const {
	os << "ScalarIO";
}

void ScalarIO::detail(std::ostream& os) const {
	os << " == ScalarIO ============================================="
	   << std::endl
	   << std::endl;

	os << "\tnbInputs\t\t\t= " << nbInputs_ << "u" << std::endl;
	os << "\tinputDimensions\t\t\t= {" << inputDimensions_ << "}"
	   << std::endl;
	os << "\tnbActiveBits\t\t\t= " << nbActiveBits_ << "u" << std::endl;
	os << "\tminimumValues\t\t\t= {" << mins_ << "}" << std::endl;
	os << "\tmaximumValues\t\t\t= {" << maxs_ << "}" << std::endl;
	os << "\tseed\t\t\t\t= " << seed_ << std::endl;
	os << std::endl;

	os << " =========================================================" << std::endl;
}

void ScalarIO::encode(const Values& inputs, htm::SDR& activeBits) const {
	CLA_ASSERT(static_cast<htm::UInt>(inputs.size()) == nbInputs_);

	htm::PSDR actives(inputDimensions_, nbInputs_);
	std::vector<htm::SDR> sdrs(nbInputs_);
	const std::vector<htm::UInt> subInputDimension
		= htm::PartialDenseFuncs::splitDimensions(
			inputDimensions_, nbInputs_
		);

	for(htm::UInt i = 0u; i < nbInputs_; ++i) {
		sdrs.at(i).initialize(subInputDimension);
		sdrs.at(i).zero();

		encoders_.at(i).encode(inputs.at(i), sdrs.at(i));
	}

	actives.concatenate(sdrs);
	activeBits = actives;
}

const Values ScalarIO::decode(const PLayerProxy& layer) const {
	const auto& predictiveBits = layer->getPredictiveBitsWithNbPCells();
	CLA_ASSERT(predictiveBits.dimensions == inputDimensions_);

	const auto& predictiveBitsVec
		= htm::PSDRex<htm::NumCells>(predictiveBits, nbInputs_).split();
	
	CLA_ASSERT(
		static_cast<htm::UInt>(predictiveBitsVec.size()) == nbInputs_
	);

	Values outputs(nbInputs_);

	for(htm::UInt i = 0u; i < nbInputs_; ++i) {
		outputs.at(i) = decoders_.at(i).infer(predictiveBitsVec.at(i));
	}

	return outputs;
}

} // namespace cla