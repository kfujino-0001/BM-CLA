// SawEnv.hpp

/** 
 * @file
 * Definitions for the SawEnv class in C++
 */

#ifndef SAW_ENV_HPP
#define SAW_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * SawEnv implementation in C++
 * 
 * @b Description
 * The SawEnv is the environment of saw wave.
 */
class SawEnv : public CoreEnv {

private:

	Step period_;
	Step phase_;
	Step refresh_;

public:

	inline static const std::string name = "SawEnv";

public:

	/**
	 * SawEnv constructor
	 * 
	 * @param period The period of one saw cycle.
	 * @param phase The phase of saw wave.
	 * @param refresh The refresh frequency of saw wave.
	 * If the refresh is set to 1, The SawEnv changes its value every time.
	 * If the refresh is set to n, The SawEnv changes it once every n times.
	 */
	SawEnv(
		const Step period,
		const int phase = 0,
		const Step refresh = 1u
	);

	/**
	 * SawEnv destructor
	 */
	~SawEnv() = default;

	/**
	 * Get the value of saw env.
	 * 
	 * @return The value of the saw env.
	 */
	const Values getValues() const override;

	/**
	 * Get json config of the environment.
	 * 
	 * @return The json instance.
	 */
	const json getJsonConfig() const override;

};


} // namespace cla

#endif // SAW_ENV_HPP