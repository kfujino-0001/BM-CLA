// ClaGenerator.hpp

/** 
 * @file
 * Definitions for the ClaGenerator class in C++
 */

#ifndef CLA_GENERATOR_HPP
#define CLA_GENERATOR_HPP

#include "cla/model/core/CoreCLA.hpp"
#include "cla/model/MultiLayerCLA.hpp"

namespace cla {

/**
 * ClaGenerator implementation in C++.
 *
 * @b Description
 * The ClaGenerator is the generator class for the Cla models.
 * This class hides the pointer type of the Cla model.
 */
template <typename ClaType>
struct ClaGenerator {

	/**
	 * Generate the Cla model instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Cla.
	 * @return PCLA The pointer of the Cla model.
	 */
	template <typename... Args>
	static PCLA generate(Args... args) {
		return std::make_unique<ClaType>(std::move(args)...);
	}
};

} // namespace cla

#endif // CLA_GENERATOR_HPP