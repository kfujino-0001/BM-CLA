// SaveLayerStateCallback.hpp

/** 
 * @file
 * Definitions for the SaveLayerStateCallback class in C++
 */

#ifndef SAVE_STATE_CALLBACK_HPP
#define SAVE_STATE_CALLBACK_HPP

#include "htm/types/Sdr.hpp"
#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/module/callback/SaveCallback.hpp"

namespace cla {

/**
 * SparseHistory implementation in C++.
 * 
 * @b Description
 * The SparseHistory is a data structure. It contains the sparse data
 * indexes. This class saves the data at the json file.
 * 
 * @param step
 * The step of the sparse history.
 * 
 * @param Sparse
 * The active columns whose type is SDR dense.
 */
struct SparseHistory {

	Step step;
	htm::SDR_sparse_t sparse;

public:

	/**
	 * SparseHistory constructor.
	 */
	SparseHistory() = default;

	/**
	 * SparseHistory constructor.
	 */
	SparseHistory(
		const Step step,
		const htm::SDR_sparse_t& sparse
	);

	/**
	 * SparseHistory destructor.
	 */
	~SparseHistory() = default;

public:

	friend std::ostream& operator<<(std::ostream& os, const SparseHistory& h);

};

std::ostream& operator<<(std::ostream& os, const SparseHistory& h);



/**
 * SaveSparseLog implementation in C++.
 * 
 * @b Description
 * SaveSparseLog it save class for layer state. This class is responsible
 * for the save process of a single layer.
 */
class SaveSparseLog : public CoreSaver {

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

	bool firstInput_;

	std::vector<SparseHistory> hists_;

public:

	/**
	 * SaveSparseLog constructor.
	 */
	SaveSparseLog() = default;

	/**
	 * SaveSparseLog constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveSparseLog(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveSparseLog destructor.
	 */
	~SaveSparseLog() = default;

	/**
	 * Initialize SaveSparseLog with the parameters.
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
	 * @param sparse The sparse data which is saved by this class.
	 */
	void add(
		const Step step,
		const htm::SDR_sparse_t& sparse
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
 * SaveLayerStateCallback implementation in C++.
 * 
 * @b Description
 * SaveLayerStateCallback is one of the Callback-series. This class saves
 * the data of layers to a json file.
 */
class SaveLayerStateCallback : public SaveCallback {

private:

	std::vector<std::string> filenames_;

	std::vector<SaveSparseLog> inputBitLogs_;
	std::vector<SaveSparseLog> activeColumnLogs_;
	std::vector<SaveSparseLog> activeCellLogs_;
	std::vector<SaveSparseLog> predictiveCellLogs_;
	std::vector<SaveSparseLog> predictiveColumnLogs_;

public:

	/**
	 * SaveLayerStateCallback constructor.
	 */
	SaveLayerStateCallback() = default;

	/**
	 * SaveLayerStateCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerStateCallback(
		const std::string& path,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerStateCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filenames The filenames where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerStateCallback(
		const std::string& path,
		const std::vector<std::string>& filenames,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerStateCallback destructor.
	 */
	~SaveLayerStateCallback() = default;

	/**
	 * Initialize SaveLayerStateCallback with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filenames The filenames where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::vector<std::string>& filenames,
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



/**
 * SaveLayerExStateCallback implementation in C++.
 * 
 * @b Description
 * 
 */
class SaveLayerExStateCallback : public SaveCallback {

private:

	std::vector<std::string> filenames_;

	std::vector<SaveSparseLog> externalActiveSdrLogs_;
	std::vector<SaveSparseLog> externalWinnerSdrLogs_;

public:

	/**
	 * SaveLayerExStateCallback constructor.
	 */
	SaveLayerExStateCallback() = default;

	/**
	 * SaveLayerExStateCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerExStateCallback(
		const std::string& path,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerExStateCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filenames The filenames where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerExStateCallback(
		const std::string& path,
		const std::vector<std::string>& filenames,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerExStateCallback destructor.
	 */
	~SaveLayerExStateCallback() = default;

	/**
	 * Initialize SaveLayerExStateCallback with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filenames The filenames where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	void initialize(
		const std::string& path,
		const std::vector<std::string>& filenames,
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

#endif // SAVE_STATE_CALLBACK_HPP