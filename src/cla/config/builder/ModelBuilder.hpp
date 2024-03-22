// ModelBuilder.hpp

/** 
 * @file
 * Definitions for the ModelBuilder class in C++
 */

#ifndef MODEL_BUILDER_HPP
#define MODEL_BUILDER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/core/CoreCLA.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * ModelJsonBuilder implementation in C++.
 * 
 * @b Description
 * The ModelJsonBuilder is for the models. This class creates the
 * model instance according to the JSON config. The structure of
 * this class is based on the builder pattern of GOF.
 */
class ModelJsonBuilder {

private:

	/**
	 * Build the mlcla model instance.
	 * 
	 * @param config The config of the mlcla model on the JSON.
	 * @return PCLA The pointer of the mlcla model instance.
	 */
	static PCLA buildMlclaModel_(const json& config);


public:

	/**
	 * ModelJsonBuilder constructor.
	 */
	ModelJsonBuilder() = default;

	/**
	 * ModelJsonBuilder destructor.
	 */
	~ModelJsonBuilder() = default;

	/**
	 * Build the cla model instance.
	 * 
	 * @param config The config of the cla model on the JSON.
	 * @return PCLA The pointer of the cla model instance.
	 */
	static PCLA buildModel(const json& config);

	/**
	 * Build the cla model instance.
	 * 
	 * @param type The name of the model type.
	 * @param config The config of the cla model on the JSON.
	 * @return PCLA The pointer of the cla model instance.
	 */
	static PCLA buildModel(
		const std::string& type, 
		const json& config
	);

};

} // namespace cla

#endif // MODEL_BUILDER_HPP