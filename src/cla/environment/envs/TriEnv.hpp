// TriEnv.hpp

/** 
 * @file
 * Definitions for the TriEnv class in C++
 */

#ifndef TRI_ENV_HPP
#define TRI_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * TriEnv implementation in C++
 * 
 * @b Description
 * The TriEnv is the environment of triangle wave.
 */
class TriEnv: public CoreEnv {

private:

	Step period_;
	Step phase_;
	Step refresh_;

public:

	inline static const std::string name = "TriEnv";

public:

	/**
	 * TriEnv constructor
	 * 
	 * @param period The period of one tri cycle.
	 * @param period The phase of tri wave.
	 * @param refresh The refresh frequency of tri wave.
	 * If the refresh is set to 1, The TriEnv changes its value every time.
	 * If the refresh is set to n, The TriEnv changes it once every n times.
	 */
	TriEnv(
		const Step period, 
		const int phase = 0,
		const Step refresh = 1u
	);

	/**
	 * TriEnv destructor
	 */
	~TriEnv() = default;

	/**
	 * Get the value of tri env.
	 * 
	 * @return The value of the tri env.
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

#endif // TRI_ENV_HPP