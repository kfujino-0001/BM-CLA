// SpatialPoolerGenerator.hpp

/** 
 * @file
 * Definitions for the SpatialPoolerGenerator class in C++
 */

#ifndef SPATIAL_POOLER_GENERATOR_HPP
#define SPATIAL_POOLER_GENERATOR_HPP

#include "cla/model/core/CoreSpatialPooler.hpp"
#include "cla/model/module/sp/HtmSpatialPooler.hpp"

namespace cla {

/**
 * SpatialPoolerGenerator implementation in C++.
 *
 * @b Description
 * The SpatialPoolerGenerator is the generator class for the SpatialPooler modules.
 * This class hides the pointer type of the SpatialPooler module.
 */
template <typename SpatialPoolerType>
struct SpatialPoolerGenerator {

	/**
	 * Generate the SpatialPooler module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the SpatialPooler.
	 * @return PSpatialPooler The pointer of the SpatialPooler module.
	 */
	template <typename... Args>
	static PSpatialPooler generate(const Args&... args) {
		return std::make_shared<SpatialPoolerType>(args...);
	}
};


} // namespace cla

#endif // SPATIAL_POOLER_GENERATOR_HPP