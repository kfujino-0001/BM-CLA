// SenderGenerator.hpp

/** 
 * @file
 * Definitions for the SenderGenerator class in C++
 */

#ifndef SENDER_GENERATOR_HPP
#define SENDER_GENERATOR_HPP

#include "cla/model/core/CoreSender.hpp"
#include "cla/model/module/sender/ActiveColumnSender.hpp"
#include "cla/model/module/sender/BurstColumnSender.hpp"

namespace cla {

/**
 * SenderGenerator implementation in C++.
 *
 * @b Description
 * The SenderGenerator is the generator class for the Sender modules.
 * This class hides the pointer type of the Sender module.
 */
template <typename SenderType>
struct SenderGenerator {

	/**
	 * Generate the Sender module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Sender.
	 * @return PSender The pointer of the Sender module.
	 */
	template <typename... Args>
	static PSender generate(const Args&... args) {
		return std::make_unique<SenderType>(args...);
	}
};

} // namespace cla

#endif // SENDER_GENERATOR_HPP