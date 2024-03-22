// ReceiverGenerator.hpp

/** 
 * @file
 * Definitions for the ReceiverGenerator class in C++
 */

#ifndef RECEIVER_GENERATOR_HPP
#define RECEIVER_GENERATOR_HPP

#include "cla/model/core/CoreReceiver.hpp"
#include "cla/model/module/receiver/ActiveCellReceiver.hpp"
#include "cla/model/module/receiver/VolatileActiveCellReceiver.hpp"

namespace cla {

/**
 * ReceiverGenerator implementation in C++.
 *
 * @b Description
 * The ReceiverGenerator is the generator class for the Receiver modules.
 * This class hides the pointer type of the Receiver module.
 */
template <typename ReceiverType>
struct ReceiverGenerator {

	/**
	 * Generate the Receiver module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Receiver.
	 * @return PReceiver The pointer of the Receiver module.
	 */
	template <typename... Args>
	static PReceiver generate(const Args&... args) {
		return std::make_unique<ReceiverType>(args...);
	}
};

} // namespace cla

#endif // RECEIVER_GENERATOR_HPP