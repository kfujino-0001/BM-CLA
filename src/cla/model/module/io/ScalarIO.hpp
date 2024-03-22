// ScalarIO.hpp

/**
 * @file
 * Definitions for the ScalarIO class in C++
 */

#ifndef SCALAR_IO_HPP
#define SCALAR_IO_HPP

#include <vector>

#include "cla/model/core/CoreIO.hpp"
#include "htm/encoders/ScalarEncoder.hpp"
#include "cla/extension/algorithms/SDRClassifierScalar.hpp"

namespace cla {

/**
 * ScalarIO implementation in C++.
 *
 * @b Description
 * The ScalarIO is extended class of CoreIO.
 * The ScalarIO is the most basic io for cla models. This class is
 * implemented by the scalar encoder(chunk encoder) and the sdr scalar
 * classifier.
 */
class ScalarIO : public CoreIO {

private:

	htm::UInt nbInputs_;
	std::vector<htm::UInt> inputDimensions_;
	htm::UInt nbActiveBits_;
	Values mins_;
	Values maxs_;
	int seed_;

	mutable std::vector<htm::ScalarEncoder> encoders_;
	mutable std::vector<htm::ClassifierScalar> decoders_;

public:

	/**
	 * ScalarIO constructor.
	 */
	ScalarIO() = default;

	/**
	 * ScalarIO constructor with the parameters.
	 * 
	 * @param nbInputs The number of input values.
	 * @param inputDimensions The input bits dimensions.
	 * @param nbActiveBits The number of the active bits.
	 * @param mins The minimum values of each input value.
	 * @param maxs The maximum values of each input value.
	 * @param seed The seed value.
	 */
	ScalarIO(
		const htm::UInt nbInputs,
		const std::vector<htm::UInt>& inputDimensions,
		const htm::UInt nbActiveBits,
		const Values& mins,
		const Values& maxs,
		const int seed
	);

	/**
	 * ScalarIO destructor.
	 */
	~ScalarIO() = default;

	/**
	 * Initialize the ScalarIO with the parameters.
	 * For more details of the parameters, see the description of the constructor.
	 */
	void initialize(
		const htm::UInt nbInputs,
		const std::vector<htm::UInt>& inputDimensions,
		const htm::UInt nbActiveBits,
		const Values& mins,
		const Values& maxs,
		const int seed
	);

	/**
	 * Summarize the input/output model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void summary(std::ostream& os = std::cout) const override;

	/**
	 * Show the detail of the input/output model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	void detail(std::ostream& os = std::cout) const override;

	/**
	 * Encode input values to an active bits, which is sdr representation.
	 *
	 * @param inputs The input values, which is floating vector.
	 * @param activeBits The active bits, which is sdr representation.
	 * (This param has a return value.)
	 */
	void encode(const Values& inputs, htm::SDR& activeBits) const override;

	/**
	 * Decode layer status to the output values, which is a floating vector.
	 *
	 * @param layer The layer proxy. This function selects a kind of sdr
	 * from the this layer proxy.
	 * @return const Values Output values, which is the floating values.
	 */
	const Values decode(const PLayerProxy& layer) const override;

	/**
	 * Learn the corresponding the sdr pattern in the layer proxy and
	 * input values on the next time step.
	 * 
	 * @param layer The layer proxy. This function selects a kind of sdr
	 * from the this layer proxy.
	 * @param nexts The input values on the next time step.
	 */
	void learn(const PLayerProxy& layer, const Values& nexts) override {}
};

} // namespace cla

#endif // SCALAR_IO_HPP