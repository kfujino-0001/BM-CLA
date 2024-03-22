// ModelAligner.hpp

/**
 * @file
 * Definitions for the ModelAligner class in C++
 */

#ifndef MODEL_ALIGNER_HPP
#define MODEL_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/config/aligner/module/LayerAligner.hpp"
#include "cla/config/aligner/module/IoAligner.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * ModelJsonAligner implementation in C++.
 *
 * @b Description
 * The ModelJsonAligner is for the model config. This class aligns the
 * model config of JSON based on the complemented modules.
 */
class ModelJsonAligner {

private:

	std::string type_;
	json config_;

	std::vector<LayerJsonAligner> layers_;
	IoJsonAligner io_;

private:

	/**
	 * Initialize the ModelJsonAligner by the MLCLA.
	 * 
	 * @param type The name of the model type.
	 * @param config The config of the cla model on the JSON.
	 */
	void initializeByMLCLA_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the model.
	 */
	void alignMLCLAConfig_();

public:

	/**
	 * ModelJsonAligner constructor.
	 */
	ModelJsonAligner() = default;

	/**
	 * ModelJsonAligner constructor with the config.
	 * 
	 * @param config The config of the cla model on the JSON.
	 */
	ModelJsonAligner(const json& config);

	/**
	 * ModelJsonAligner constructor with the config.
	 * 
	 * @param type The name of the model type.
	 * @param config The config of the cla model on the JSON.
	 */
	ModelJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * ModelJsonAligner destructor.
	 */
	~ModelJsonAligner() = default;

	/**
	 * Initialize the ModelJsonAligner with the config.
	 * 
	 * @param config The config of the cla model on the JSON.
	 */
	void initialize(const json& config);

	/**
	 * Initialize the ModelJsonAligner with the config.
	 * 
	 * @param type The name of the model type.
	 * @param config The config of the cla model on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the model.
	 */
	void align();

	/**
	 * Get the model type aligned in ModelJsonAligner.
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the model.
	 * 
	 * @return const json& The aligned config of the model.
	 */
	const json& getConfig() const;

	/**
	 * Get the reference of the layers.
	 * 
	 * @return std::vector<LayerJsonAligner>& The reference of the layers.
	 */
	std::vector<LayerJsonAligner>& getLayers();

	/**
	 * Get the reference of the io.
	 * 
	 * @return IoJsonAligner& The reference of the io.
	 */
	IoJsonAligner& getIO();


	/************************************************
	 * Setters.
	 ***********************************************/

	/**
	 * Set the seed value. Seed values for all modules are unified 
	 * to this value.
	 * 
	 * @param seed The seed value.
	 */
	void setSeed(const int seed);

};

} // namespace cla

#endif // MODEL_ALIGNER_HPP