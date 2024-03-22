// CoreIO.hpp

/**
 * @file
 * Definitions for the CoreIO class in C++
 */

#ifndef CORE_IO_HPP
#define CORE_IO_HPP

#include <iostream>
#include <memory>

#include "htm/types/Sdr.hpp"

#include "cla/environment/core/CoreEnv.hpp" // for tyep values.
#include "cla/model/module/helper/LayerProxy.hpp"

namespace cla {

/**
 * CoreIO implementation in C++.
 *
 * @b Description
 * This class is an abstract class for defining functions that convert the
 * vector of floating values and the sdr representation. This class is
 * based on the strategy design pattern.
 */
class CoreIO {

public:

	/**
	 * CoreIO constructor.
	 */
	CoreIO() = default;

	/**
	 * CoreIO destructor.
	 */
	virtual ~CoreIO() = default;

	/**
	 * Summarize the input/output model.
	 *
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void summary(std::ostream& os = std::cout) const = 0;

	/**
	 * Show the detail of the input/output model.
	 * 
	 * @param os The output stream. The default os is std::cout.
	 */
	virtual void detail(std::ostream& os = std::cout) const = 0;

	/**
	 * Encode input values to an active bits, which is sdr representation.
	 *
	 * @param inputs The input values, which is floating vector.
	 * @param activeBits The active bits, which is sdr representation.
	 * (This param has a return value.)
	 */
	virtual void encode(const Values& inputs, htm::SDR& activeBits) const = 0;

	/**
	 * Decode layer status to the output values, which is a floating vector.
	 *
	 * @param layer The layer proxy. This function selects a kind of sdr
	 * from the this layer proxy.
	 * @return const Values Output values, which is the floating values.
	 */
	virtual const Values decode(const PLayerProxy& layer) const = 0;

	/**
	 * Learn the corresponding the sdr pattern in the layer proxy and
	 * input values on the next time step.
	 * 
	 * @param layer The layer proxy. This function selects a kind of sdr
	 * from the this layer proxy.
	 * @param nexts The input values on the next time step.
	 */
	virtual void learn(const PLayerProxy& layer, const Values& nexts) = 0;
};

using PIO = std::unique_ptr<CoreIO>;


} // namespace cla

#endif // CORE_IO_HPP