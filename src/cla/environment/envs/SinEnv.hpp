// SinEnv.hpp

/** 
 * @file
 * Definitions for the SinEnv class in C++
 */

#ifndef SIN_ENV_HPP
#define SIN_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * SinEnv implementation in C++
 * 
 * @b Description
 * The SinEnv is the environment of sin wave.
 */
class SinEnv : public CoreEnv {

private:

	Step period_;
	Step phase_;
	Step refresh_;

public:

	inline static const std::string name = "SinEnv";

public:

	/**
	 * SinEnv constructor
	 * 
	 * @param period The period of one sin cycle.
	 * @param phase The phase of sin wave.
	 * @param refresh The refresh frequency of sin wave.
	 * If the refresh is set to 1, The SinEnv changes its value every time.
	 * If the refresh is set to n, The SinEnv changes it once every n times.
	 * 
	 */
	SinEnv(
		const Step period,
		const int phase = 0,
		const Step refresh = 1u
	);

	/**
	 * SinEnv destructor
	 */
	~SinEnv() = default;

	/**
	 * Get the value of sin env.
	 * 
	 * @return The value of the sin env.
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

#endif // SIN_ENV_HPP