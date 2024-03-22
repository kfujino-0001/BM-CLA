// Envs.hpp

/** 
 * @file
 * Definitions for the Envs class in C++
 */

#ifndef ENVS_HPP
#define ENVS_HPP

// artificial wave data.
#include "cla/environment/envs/ConstEnv.hpp"
#include "cla/environment/envs/SinEnv.hpp"
#include "cla/environment/envs/SawEnv.hpp"
#include "cla/environment/envs/TriEnv.hpp"
#include "cla/environment/envs/LogisticMapEnv.hpp"
#include "cla/environment/envs/CompositeWaveEnv.hpp"

// real world data.
#include "cla/environment/envs/RealDataEnv.hpp"

#include <memory>

namespace cla {

template<typename EnvType>
struct Env {
	template<typename... Args>
	static PEnv make(Args... args) {
		return std::make_shared<EnvType>(args...);
	}
};

} // namespace cla


#endif // ENVS_HPP