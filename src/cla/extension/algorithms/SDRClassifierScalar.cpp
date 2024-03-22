//SDRClassifierScalar.cpp

#include <htm/utils/Log.hpp>
#include <cla/extension/algorithms/SDRClassifierScalar.hpp>

namespace htm {

ClassifierScalar::ClassifierScalar(const ScalarEncoderParameters& params) {
	initialize(params);
}

void ClassifierScalar::initialize(const ScalarEncoderParameters& params) {
	args_ = params;
}

Real64 ClassifierScalar::infer(const SDR& input)const {
	NTA_CHECK(args_.size == input.size) << "input different size SDR.";
	NTA_CHECK(args_.activeBits != 0u) << "activeBits is zero.";

	if (input.getSparse().empty())return 0.0;

	const auto& chunk = args_.activeBits;
	const auto& firstActiveBitIdx = input.getSparse().at(0);

	Real64 predictiveValue =
		(args_.maximum - args_.minimum) * (Real64)firstActiveBitIdx / (Real64)(args_.size - chunk) + args_.minimum;

	return predictiveValue;
}

// Real64 ClassifierScalar::infer(const SDRex<htm::NumCells>& input)const {
// 	NTA_CHECK(args_.size == input.size) << "input different size SDR.";

// 	return inferForInverse_(input.getExDataDense());
// }

} // namespace htm