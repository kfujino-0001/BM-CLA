// SpatialPoolerBuilder.hpp

/**
 * @file
 * Definitions for the SpatialPoolerBuilder class in C++
 */

#ifndef SPATIAL_POOLER_BUILDER_HPP
#define SPATIAL_POOLER_BUILDER_HPP

#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "cla/model/core/CoreSpatialPooler.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * SpatialPoolerJsonBuilder implementation in C++.
 *
 * @b Description
 * The SpatialPoolerJsonBuilder is for the SpatialPooler modules. This
 * class creates the SpatialPooler module instance according to the JSON
 * config. The structure of this class is based on the builder pattern of
 * GOF.
 */
class SpatialPoolerJsonBuilder {

private:

	/**
	 * Build the HtmSpatialPooler module instance.
	 * 
	 * @param config The config of the HtmSpatialPooler module on the JSON.
	 * @return PSpatialPooler The pointer of the SpatialPooler instance. 
	 */
	static PSpatialPooler buildHtmSpatialPooler_(const json& config);

public:

	/**
	 * SpatialPoolerJsonBuilder constructor.
	 */
	SpatialPoolerJsonBuilder() = default;

	/**
	 * SpatialPoolerJsonBuilder destructor.
	 */
	~SpatialPoolerJsonBuilder() = default;

	/**
	 * Build the SpatialPooler module instance.
	 * 
	 * @param config The config of the SpatialPooler module on the JSON.
	 * @return PSpatialPooler The pointer of the SpatialPooler instance. 
	 */
	static PSpatialPooler buildSpatialPooler(const json& config);

	/**
	 * Build the SpatialPooler module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the SpatialPooler module on the JSON.
	 * @return PSpatialPooler The pointer of the SpatialPooler instance. 
	 */
	static PSpatialPooler buildSpatialPooler(
		const std::string& type,
		const json& config
	);
};


} // namespace cla

#endif // SPATIAL_POOLER_BUILDER_HPP