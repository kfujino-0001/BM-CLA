// TemporalMemoryGenerator.hpp

/** 
 * @file
 * Definitions for the TemporalMemoryGenerator class in C++
 */

#ifndef TEMPORAL_MEMORY_GENERATOR_HPP
#define TEMPORAL_MEMORY_GENERATOR_HPP

#include "cla/model/core/CoreTemporalMemory.hpp"
#include "cla/model/module/tm/HtmTemporalMemory.hpp"

namespace cla {

/**
 * TemporalMemoryGenerator implementation in C++.
 *
 * @b Description
 * The TemporalMemoryGenerator is the generator class for the TemporalMemory modules.
 * This class hides the pointer type of the TemporalMemory module.
 */
template <typename TemporalMemoryType>
struct TemporalMemoryGenerator {

	/**
	 * Generate the TemporalMemory module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the TemporalMemory.
	 * @return PTemporalMemory The pointer of the TemporalMemory module.
	 */
	template <typename... Args>
	static PTemporalMemory generate(const Args&... args) {
		return std::make_shared<TemporalMemoryType>(args...);
	}
};

} // namespace cla

#endif // TEMPORAL_MEMORY_GENERATOR_HPP