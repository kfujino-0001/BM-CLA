// IoGenerator.hpp

/** 
 * @file
 * Definitions for the IoGenerator class in C++
 */

#ifndef IO_GENERATOR_HPP
#define IO_GENERATOR_HPP

#include "cla/model/core/CoreIO.hpp"
#include "cla/model/module/io/ScalarIO.hpp"

namespace cla {

/**
 * IoGenerator implementation in C++.
 *
 * @b Description
 * The IoGenerator is the generator class for the Io modules.
 * This class hides the pointer type of the Io module.
 */
template <typename IoType>
struct IoGenerator {

	/**
	 * Generate the Io module instance.
	 *
	 * @tparam Args The parameter definitions.
	 * @param args The parameters to initialize the Io.
	 * @return PIo The pointer of the Io module.
	 */
	template <typename... Args>
	static PIO generate(const Args&... args) {
		return std::make_unique<IoType>(args...);
	}
};

} // namespace cla

#endif // IO_GENERATOR_HPP