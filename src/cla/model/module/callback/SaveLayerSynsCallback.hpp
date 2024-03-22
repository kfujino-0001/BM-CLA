// SaveLayerSynsCallback.hpp

/** 
 * @file
 * Definitions for the SaveLayerSynsCallback class in C++
 */

#ifndef SAVE_SYNAPSE_CALLBACK_HPP
#define SAVE_SYNAPSE_CALLBACK_HPP

#include <iostream>
#include <string>
#include <tuple>

#include "htm/types/Sdr.hpp"
#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"

#include "cla/model/module/helper/LayerProxy.hpp"
#include "cla/model/module/callback/SaveCallback.hpp"

namespace cla {

/**
 * SynapseHistory implementation in C++.
 * 
 * @b Description
 * The SynapseHistory is a data structure. It contains the synapse data
 * indexes. This class saves the data at the json file.
 * 
 * @param step
 * The step of the synapse history.
 * 
 * @param createdSynapses
 * The indexes of created synapses
 * 
 * @param destroyedSynapses
 * The indexes of destroyed synapses
 * 
 * @param updateSynapses
 * The indexes of update synapses and each of the permanence.
 */
struct SynapseHistory {

	Step step;

	std::vector<SynapseConnection> createdSynapses;
	std::vector<htm::Synapse> destroyedSynapses;
	std::vector<SynapsePermanence> updatedSynapses;

public:

	/**
	 * SynapseHistory constructor.
	 */
	SynapseHistory() = default;

	/**
	 * SynapseHistory constructor.
	 */
	SynapseHistory(
		const Step step,
		const std::vector<SynapseConnection>& createdSynapses,
		const std::vector<htm::Synapse>& destroyedSynapse,
		const std::vector<SynapsePermanence>& updatedSynapses
	);

	/**
	 * SynapseHistory destructor.
	 */
	~SynapseHistory() = default;

public:

	friend std::ostream& operator<<(std::ostream& os, const SynapseHistory& h);

};

std::ostream& operator<<(std::ostream& os, const SynapseHistory& h);



/**
 * SaveSynapseLog implementation in C++.
 * 
 * @b Description
 * SaveSynapseLog is save class for layer synapses. This class is responsible
 * for the save process of a single layer.
 */
class SaveSynapseLog : public CoreSaver {

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

	bool firstInput_;

	std::vector<SynapseHistory> hists_;

public:

	/**
	 * SaveSynapseLog constructor.
	 */
	SaveSynapseLog() = default;

	/**
	 * SaveSynapseLog constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveSynapseLog(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveSynapseLog destructor.
	 */
	~SaveSynapseLog() = default;

	/**
	 * Initialize SaveSynapseLog with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * Reset the SaveCallback.
	 */
	void reset() override;

	/**
	 * Open the file where the data will be saved.
	 */
	void open() override;

	/**
	 * Add the data to container.
	 * 
	 * @param step The step this function called.
	 * @param layer A kind of layer. It needs to get the internal
	 * layer data.
	 */
	void add(
		const Step step,
		const PLayerProxy& layer
	);

	/**
	 * Save the data on the container.
	 */
	void save() override;

	/**
	 * Close the file where the data is saved.
	 */
	void close() override;

};



/**
 * SaveSynapseLogCallback implementation in C++.
 * 
 * @b Description
 * SaveSynapseLogCallback is one of the Callback-series. This class saves
 * the synapse data of layers to a json file.
 */
class SaveSynapseLogCallback : public SaveCallback {

private:

	std::vector<SaveSynapseLog> logs_;

public:

	/**
	 * SaveSynapseLogCallback constructor.
	 */
	SaveSynapseLogCallback() = default;

	/**
	 * SaveSynapseLogCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveSynapseLogCallback(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveSynapseLogCallback destructor.
	 */
	~SaveSynapseLogCallback() = default;

	/**
	 * Initialize SaveSynapseLogCallback with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * Reset the SaveCallback.
	 */
	void reset() override;

	/**
	 * Open the file where the data will be saved.
	 */
	void open() override;

	/**
	 * Add the data to container.
	 * 
	 * @param step The step this function called.
	 * @param inputs The input values from an environment in the step.
	 * @param nexts The input values form the environemnt in the next step.
	 * @param outputs The output values of CLA in the step.
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void add(
		const Step step,
		const Values& inputs,
		const Values& nexts,
		const Values& outputs,
		const CoreCLA* cla
	) override;

	/**
	 * Save the data on the container.
	 */
	void save() override;

	/**
	 * Close the file where the data is saved.
	 */
	void close() override;

	/**
	 * Called the start of the processing.
	 * 
	 * @param cla A kind of cla agents. It needs to get the internal
	 * data of cla. (ex: cla->getUnits();)
	 */
	void doStartProcessing(const CoreCLA* cla) override;


};

} // namespace cla

#endif // SAVE_SYNAPSE_CALLBACK_HPP