// AdapterGenerator.hpp

/** 
 * @file
 * Definitions for the AdapterGenerator class in C++
 */

#ifndef ADAPTER_GENERATOR_HPP
#define ADAPTER_GENERATOR_HPP

#include "cla/model/core/CoreAdapter.hpp"
#include "cla/model/module/adapter/DirectAdapter.hpp"

namespace cla {

/**
 * AdapterGenerator implementation in C++.
 *
 * @b Description
 * The AdapterGenerator is the generator class for the Adapter modules.
 * This class hides the pointer type of the Adapter module.
 */
template <typename AdapterType>
struct AdapterGenerator {

	/**
	 * Generate the Adapter module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Adapter.
	 * @return PAdapter The pointer of the Adapter module.
	 */
	template <typename... Args>
	static PAdapter generate(const Args&... args) {
		return std::make_unique<AdapterType>(args...);
	}
};

} // namespace cla

#endif // ADAPTER_GENERATOR_HPP