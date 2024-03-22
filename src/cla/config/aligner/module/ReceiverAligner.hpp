// ReceiverAligner.hpp

/** 
 * @file
 * Definitions for the ReceiverAligner class in C++
 */

#ifndef RECEIVER_ALIGNER_HPP
#define RECEIVER_ALIGNER_HPP

#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "cla/model/generator/ReceiverGenerator.hpp"

namespace cla {

using json = nlohmann::json;


/**
 * ReceiverJsonAligner implementation in C++.
 * 
 * @b Description
 * The ReceiverJsonAligner is for the receiver config. The class aligner the 
 * receiver config of JSON based on the complemented modules.
 */
class ReceiverJsonAligner {

private:

	std::string type_;
	json config_;

	htm::UInt externalPredictiveInputs_;
	bool isExternalPredictiveInputsUpdated_;

private:

	/**
	 * Initialize the ReceiverJsnAligner by the active cell receiver.
	 * 
	 * @param type The name of the active cell receiver module.
	 * @param config The config of the active cell receiver module.
	 */
	void initializeByActiveCellReceiver_(
		const std::string& type,
		const json& config
	);

	/**
	 * Initialize the ReceiverJsonAligner by the volatile active cell receiver.
	 * 
	 * @param type The name of the volatile active cell receiver module.
	 * @param config The config of the volatile active cell receiver module.
	 */
	void initializeByVolatileActiveCellReceiver_(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the active cell receiver module.
	 */
	void alignActiveCellReceiverConfig_();

	/**
	 * Align the config of the volatile active cell receiver module.
	 */
	void alignVolatileActiveCellReceiverConfig_();


public:

	/**
	 * ReceiverJsonAligner constructor.
	 */
	ReceiverJsonAligner();

	/**
	 * ReceiverJsonAligner constructor with the config.
	 * 
	 * @param config The config of the receiver module on the JSON.
	 */
	ReceiverJsonAligner(const json& config);

	/**
	 * ReceiverJsonAligner constructor with the config.
	 * 
	 * @param type The name of the receiver module type.
	 * @param config The config of the receiver module on the JSON.
	 */
	ReceiverJsonAligner(
		const std::string& type,
		const json& config
	);

	/**
	 * ReceiverJsonAligner destructor.
	 */
	~ReceiverJsonAligner() = default;

	/**
	 * Initialize the ReceiverJsonAligner with the config.
	 * 
	 * @param config The config of the receiver module on the JSON.
	 */
	void initialize(const json& config);
	
	/**
	 * Initialize the ReceiverJsonAligner with the config.
	 * 
	 * @param type The name of the receiver module type.
	 * @param config The config of the receiver module on the JSON.
	 */
	void initialize(
		const std::string& type,
		const json& config
	);

	/**
	 * Align the config of the receiver module.
	 */
	void align();

	/**
	 * Get the receiver type aligned in ReceiverJsonAligner.
	 * 
	 * @return const std::string& 
	 */
	const std::string& getType() const;

	/**
	 * Get the config of the receiver module.
	 * 
	 * @return const json& The aligned config of the receiver module.
	 */
	const json& getConfig() const;


	/************************************************
	 * Setters.
	 ***********************************************/

	/**
	 * Set the external predictive inputs.
	 * 
	 * @param externalPredictiveInputs The external predictive inputs.
	 */
	void setExternalPredictiveInputs(const htm::UInt externalPredictiveInputs);

};

} // namespace cla

#endif // RECEIVER_ALIGNER_HPP