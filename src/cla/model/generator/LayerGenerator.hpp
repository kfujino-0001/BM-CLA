// LayerGenerator.hpp

/** 
 * @file
 * Definitions for the LayerGenerator class in C++
 */

#ifndef LAYER_GENERATOR_HPP
#define LAYER_GENERATOR_HPP

#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/module/layer/HtmLayer.hpp"

namespace cla {

/**
 * LayerGenerator implementation in C++.
 *
 * @b Description
 * The LayerGenerator is the generator class for the Layer modules.
 * This class hides the pointer type of the Layer module.
 */
template <typename LayerType>
struct LayerGenerator {

	/**
	 * Generate the Layer module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Layer.
	 * @return PLayer The pointer of the Layer module.
	 */
	template <typename... Args>
	static PLayer generate(Args... args) {
		return std::make_unique<LayerType>(std::move(args)...);
	}
};

} // namespace cla

#endif // LAYER_GENERATOR_HPP