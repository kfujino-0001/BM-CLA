// IoAligner.hpp

/** 
 * @file
 * Definitions for the IoAligner class in C++
 */

#ifndef IO_ALIGNER_HPP
#define IO_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/generator/IoGenerator.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * IoJsonAligner implementation in C++.
 * 
 * @b Description
 * The IoJsonAligner is for the io config. This class aligns the
 * io config of JSON based on the complemented modules.
 */
class IoJsonAligner {

private:

	std::string type_;
	json config_;

	Values mins_;
	Values maxs_;
	int seed_;

	bool isMinsUpdated_;
	bool isMaxsUpdated_;
	bool isSeedUpdated_;

private:

	/**
	 * Initialize the IoJsonAligner by the ScalarIO.
	 * 
	 * @param type The name of the io module type.
	 * @param config The config of the io module on the JSON.
	 */
	void initializeByScalarIO_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the io module.
	 */
	void alignScalarIOConfig_();

public:

	/**
	 * IoJsonAligner constructor.
	 */
	IoJsonAligner() = default;

	/**
	 * IoJsonAligner constructor with the config.
	 * 
	 * @param config The config of the io module on the JSON.
	 */
	IoJsonAligner(const json& config);

	/**
	 * IoJsonAligner constructor with the config.
	 * 
	 * @param type The name of the io module type.
	 * @param config The config of the io module on the JSON.
	 */
	IoJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * IoJsonAligner destructor.
	 */
	~IoJsonAligner() = default;

	/**
	 * Initialize the IoJsonAligner with the config.
	 * 
	 * @param config The config of the io module on the JSON.
	 */
	void initialize(const json& config);

	/**
	 * Initialize the IoJsonAligner with the config.
	 * 
	 * @param type The name of the io module type.
	 * @param config The config of the io module on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the io module.
	 */
	void align();

	/**
	 * Get the io type aligned in IoJsonAligner.
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the io module.
	 * 
	 * @return const json& The aligned config of the io module.
	 */
	const json& getConfig() const;


	/************************************************
	 * Setters
	 ***********************************************/

	/**
	 * Set the min values.
	 * 
	 * @param mins The min values.
	 */
	void setMins(const Values& mins);

	/**
	 * Set the max values.
	 * 
	 * @param maxs The max values.
	 */
	void setMaxs(const Values& maxs);

	/**
	 * Set the seed value.
	 * 
	 * @param seed The seed value. 
	 */
	void setSeed(const int seed);


	/************************************************
	 * Getters
	 ***********************************************/

	/**
	 * Get the io module dimensions.
	 * 
	 * @return const std::vector<htm::UInt>& the io module dimensions.
	 */
	const std::vector<htm::UInt> getDimensions() const;

};

} // namespace cla

#endif // IO_ALIGNER_HPP