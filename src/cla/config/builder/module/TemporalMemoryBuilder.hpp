// TemporalMemoryBuilder.hpp

/**
 * @file
 * Definitions for the TemporalMemoryBuilder class in C++
 */

#ifndef TEMPORAL_MEMORY_BUILDER_HPP
#define TEMPORAL_MEMORY_BUILDER_HPP

#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "cla/model/core/CoreTemporalMemory.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * TemporalMemoryJsonBuilder implementation in C++.
 *
 * @b Description
 * The TemporalMemoryJsonBuilder is for the temporal memory modules. This
 * class creates the temporal memory module instance according to the JSON
 * config. The structure of this class is based on the builder pattern of
 * GOF.
 */
class TemporalMemoryJsonBuilder {

private:

	/**
	 * Build the htm temporal memory module instance.
	 *
	 * @param config The config of the htm temporal memory module on the
	 * JSON.
	 * @return PTemporalMemory The pointer of the htm temporal memory
	 * module instance.
	 */
	static PTemporalMemory buildHtmTemporalMemory_(const json& config);

public:

	/**
	 * TemporalMemoryJsonBuilder constructor.
	 */
	TemporalMemoryJsonBuilder() = default;

	/**
	 * TemporalMemoryJsonBuilder destructor.
	 */
	~TemporalMemoryJsonBuilder() = default;

	/**
	 * Build the temporal memory module instance.
	 *
	 * @param config The config of the temporal memory module on the JSON.
	 * @return PTemporalMemory The pointer of the io module instance.
	 */
	static PTemporalMemory buildTemporalMemory(const json& config);

	/**
	 * Build the temporal memory module instance.
	 *
	 * @param type The name of the module type.
	 * @param config The config of the temporal memory module on the JSON.
	 * @return PTemporalMemory The pointer of the io module instance.
	 */
	static PTemporalMemory buildTemporalMemory(
		const std::string& type,
		const json& config
	);
};

} // namespace cla

#endif // TEMPORAL_MEMORY_BUILDER_HPP