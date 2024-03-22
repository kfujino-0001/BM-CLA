// CallbackGenerator.hpp

/** 
 * @file
 * Definitions for the CallbackGenerator class in C++
 */

#ifndef CALLBACK_GENERATOR_HPP
#define CALLBACK_GENERATOR_HPP

#include "cla/model/core/CoreCallback.hpp"
#include "cla/model/module/callback/EvalCallback.hpp"
#include "cla/model/module/callback/SaveModelLogCallback.hpp"
#include "cla/model/module/callback/SaveLayerLogCallback.hpp"
#include "cla/model/module/callback/SaveLayerStateCallback.hpp"
#include "cla/model/module/callback/SaveLayerSynsCallback.hpp"
// #include "cla/model/module/callback/SaveNumSynsCallback.hpp"
// #include "cla/model/module/callback/SaveActiveSegmentsCallback.hpp"
#include "cla/model/module/callback/CompositeCallback.hpp"

namespace cla {

/**
 * CallbackGenerator implementation in C++.
 *
 * @b Description
 * The CallbackGenerator is the generator class for the Callback modules.
 * This class hides the pointer type of the Callback module.
 */
template <typename CallbackType>
struct CallbackGenerator {

	/**
	 * Generate the Callback module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Callback.
	 * @return PCallback The pointer of the Callback module.
	 */
	template <typename... Args>
	static PCallback generate(const Args&... args) {
		return std::make_shared<CallbackType>(args...);
	}
};

template<>
struct CallbackGenerator<CompositeCallback> {

	/**
	 * Generate the Callback module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Callback.
	 * @return PCallback The pointer of the Callback module.
	 */
	static PCallback generate(const std::vector<PCallback>& callbacks) {
		return std::make_shared<CompositeCallback>(callbacks);
	}
};

} // namespace cla

#endif // CALLBACK_GENERATOR_HPP