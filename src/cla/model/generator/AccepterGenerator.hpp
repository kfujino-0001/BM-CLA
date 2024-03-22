// AccepterGenerator.hpp

/** 
 * @file
 * Definitions for the AccepterGenerator class in C++
 */

#ifndef ACCEPTER_GENERATOR_HPP
#define ACCEPTER_GENERATOR_HPP

#include "cla/model/core/CoreAccepter.hpp"
#include "cla/model/module/accepter/FullAccepter.hpp"
#include "cla/model/module/accepter/IntensityAccepter.hpp"

namespace cla {

/**
 * AccepterGenerator implementation in C++.
 *
 * @b Description
 * The AccepterGenerator is the generator class for the accepter modules.
 * This class hides the pointer type of the accepter module.
 */
template <typename AccepterType>
struct AccepterGenerator {

	/**
	 * Generate the accepter module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the accepter.
	 * @return PAccepter The pointer of the accepter module.
	 */
	template <typename... Args>
	static PAccepter generate(const Args&... args) {
		return std::make_unique<AccepterType>(args...);
	}
};

} // namespace cla

#endif // ACCEPTER_GENERATOR_HPP