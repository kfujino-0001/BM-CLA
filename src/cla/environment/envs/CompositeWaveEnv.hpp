// CompositeWaveEnv.hpp

/** 
 * @file
 * Definitions for the CompositeWaveEnv class in C++
 */

#ifndef COMPOSITE_WAVE_ENV_HPP
#define COMPOSITE_WAVE_ENV_HPP

#include "cla/environment/core/CoreEnv.hpp"

namespace cla {

/**
 * CompositeWave implementation in C++.
 * 
 * @b Description
 * The CompositeWave is the environment of composite waveforms.
 * This class returns the values of composited sin waves.
 */
class CompositeWave : public CoreEnv {

private:

	Step period_;
	Step phase_;
	Step refresh_;

public:

	inline static const std::string name = "CompositeEnv";

public:

	/**
	 * @param period The period of one saw cycle.
	 * @param phase The phase of saw wave.
	 * @param refresh The refresh frequency of saw wave.
	 * If the refresh is set to 1, The SawEnv changes its value every time.
	 * If the refresh is set to n, The SawEnv changes it once every n times.
	 */
	CompositeWave(
		const Step period,
		const int phase = 0,
		const Step refresh = 1u
	);

	/**
	 * CompositeWave destructor.
	 */
	~CompositeWave() = default;

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

#endif // COMPOSITE_WAVE_ENV_HPP