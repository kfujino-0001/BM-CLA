// LayerBuilder.hpp

/**
 * @file
 * Definitions for the LayerBuilder class in C++
 */

#ifndef LAYER_BUILDER_HPP
#define LAYER_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreLayer.hpp"


namespace cla {

using json = nlohmann::json;


/**
 * LayerJsonBuilder implementation in C++.
 *
 * @b Description
 * The LayerJsonBuilder is for the layer modules. This class creates the
 * layer module instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class LayerJsonBuilder {

private:

	/**
	 * Build the htm layer module instance.
	 * 
	 * @param config The config of the htm layer module on the JSON.
	 * @return PLayer The pointer of the htm layer module instance.
	 */
	static PLayer buildHtmLayer_(const json& config);

public:

	/**
	 * LayerJsonBuilder constructor.
	 */
	LayerJsonBuilder() = default;

	/**
	 * LayerJsonBuilder destructor.
	 */
	~LayerJsonBuilder() = default;

	/**
	 * Build the layer module instance.
	 * 
	 * @param config The config of the layer module on the JSON.
	 * @return PLayer The pointer of the layer module instance.
	 */
	static PLayer buildLayer(const json& config);

	/**
	 * Build the layer module instance.
	 * 
	 * @param type The name of the module type.
	 * @param config The config of the layer module on the JSON.
	 * @return PLayer The pointer of the layer module instance.
	 */
	static PLayer buildLayer(
		const std::string& type, 
		const json& config
	);
};

} // namespace cla

#endif // LAYER_BUILDER_HPP