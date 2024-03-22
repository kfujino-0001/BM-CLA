// Wrappers.hpp

/** 
 * @file
 * Definitions for the Wrappers class in C++
 */

#ifndef WRAPPERS_HPP
#define WRAPPERS_HPP

#include "cla/environment/wrappers/InvertedWrapper.hpp"
#include "cla/environment/wrappers/NoisedWrapper.hpp"
#include "cla/environment/wrappers/CompositeWrapper.hpp"
#include "cla/environment/wrappers/SequentialWrapper.hpp"
#include "cla/environment/wrappers/ProbabilitySequentialWrapper.hpp"
#include "cla/environment/wrappers/SmoothSequentialWrapper.hpp"

namespace cla {

template<typename WrapperType>
struct Wrapper {
	template<typename... Args>
	static PEnv make(Args... args) {
		return std::make_shared<WrapperType>(args...);
	}
};

template<>
struct Wrapper<CompositeWrapper> {
	static PEnv make(const std::vector<PEnv>& envs) {
		return std::make_shared<CompositeWrapper>(envs);
	}
};

template<>
struct Wrapper<SequentialWrapper> {
	static PEnv make(const std::vector<EnvSection>& sections) {
		return std::make_shared<SequentialWrapper>(sections);
	}
};

template<>
struct Wrapper<ProbabilitySequentialWrapper> {
	static PEnv make(
		const std::vector<EnvSection>& sections, 
		const int seed = 0
	) {
		return std::make_shared<ProbabilitySequentialWrapper>(sections, seed);
	}
};

template<>
struct Wrapper<SmoothSequentialWrapper> {
	static PEnv make(
		const std::vector<EnvSection>& sections,
		const Step overlappedStep = 25u
	) {
		return std::make_shared<SmoothSequentialWrapper>(sections, overlappedStep);
	}
};

} // namespace cla

#endif // WRAPPERS_HPP