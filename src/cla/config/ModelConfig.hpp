// ModelConfig.hpp

/**
 * @file
 * Definitions for the ModelConfig class in C++
 */

#ifndef MODEL_CONFIG_HPP
#define MODEL_CONFIG_HPP

#include <iostream>
#include <nlohmann/json.hpp>

#include "cla/config/aligner/ModelAligner.hpp"
#include "cla/config/builder/ModelBuilder.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * JsonConfig implementation in C++.
 *
 * @b Description
 * The JsonConfig is the supporter class for the json config. This class
 * has responsible for the aligning the config and building the model
 * instance.
 */
class JsonConfig {

private:

	ModelJsonAligner aligner_;
	json config_;

private:

	/**
	 * Initialize the JsonConfig.
	 * 
	 * @param config The config of the cla model on the JSON.
	 */
	void initialize_(const json& config);

public:

	/**
	 * JsonConfig constructor.
	 */
	JsonConfig() = default;

	/**
	 * JsonConfig constructor with the config.
	 *
	 * @param config The config of the cla model on the JSON.
	 */
	JsonConfig(const json& config);

	/**
	 * JsonConfig destructor.
	 */
	~JsonConfig() = default;

	/**
	 * Get the reference of the model aligner.
	 *
	 * @return ModelJsonAligner& The reference of the model aligner.
	 */
	ModelJsonAligner& getModel();

	/**
	 * Get the config of the model.
	 * 
	 * @return const json& The config.
	 */
	const json& getConfig() const;

	/**
	 * Align the config of the model.
	 */
	void align();

	/**
	 * Build the cla model instance.
	 *
	 * @return PCLA The pointer of the cla model instance.
	 */
	PCLA buildModel();


	friend std::ostream& operator<<(std::ostream& os, const JsonConfig& config);
	friend std::istream& operator>>(std::istream& is, JsonConfig& config);
};


std::ostream& operator<<(std::ostream& os, const JsonConfig& config);
std::istream& operator>>(std::istream& is, JsonConfig& config);

} // namespace cla

#endif // MODEL_CONFIG_HPP