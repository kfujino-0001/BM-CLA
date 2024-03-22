// SaveLayerLogCallback.hpp

/** 
 * @file
 * Definitions for the SaveLayerLogCallback class in C++
 */

#ifndef SAVE_LAYER_LOG_CALLBACK_HPP
#define SAVE_LAYER_LOG_CALLBACK_HPP

#include "cla/utils/Csv.hpp"
#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/module/callback/SaveCallback.hpp"
#include "cla/utils/Status.hpp"

namespace cla {

using Header = csv::Header;

/**
 * LayerHistory implementation in C++.
 * 
 * @b Description
 * The LayerHistory is a data structure. It contains the actual and
 * predictive values, 
 * 
 * @param step
 * The step in which this data is.
 * 
 * @param predictiveCellsNum
 * The number of predictive cells in a layer.
 * 
 * @param predictiveColumnsNum
 * The number of predictive columns in a layer.
 * 
 * @param averageNumPredCellsPerColumn
 * The average of the number of predictive cells in a predictive columns.
 * 
 * @param burstColumnsNum
 * The number of burst columns in a layer.
 * 
 * @param anomaly
 * Tha anomaly rate of temporal memory.
 * 
 * @param numTmSynapses
 * The number of temporal memory synapses.
 * 
 * @param numTmSegments
 * The number of temporal memory segments.
 * 
 * @param status
 * The layer status.
 */
struct LayerHistory {

public:

	inline static const Header header = {
		"step", 
		"predictive_cells_num", "predictive_columns_num",
		"average_num_predictive_cells_per_column",
		"burst_columns_num",
		"anomaly", "num_tm_synapses", "num_tm_segments", 
		"status"
	};

public:

	Step step;

	std::size_t predictiveCellsNum;
	std::size_t predictiveColumnsNum;
	double averageNumPredCellsPerColumn;

	std::size_t burstColumnsNum;

	double anomaly;

	std::size_t numTmSynapses;
	std::size_t numTmSegments;

	Status status;

public:

	/**
	 * LayerHistory constructor.
	 */
	LayerHistory() = default;

	/**
	 * LayerHistory constructor.
	 */
	LayerHistory(
		const Step step,
		const std::size_t predictiveCellsNum,
		const std::size_t predictiveColumnsNum,
		const double averageNumPredCellsPerColumn,
		const std::size_t burstColumnsNum,
		const double anomaly,
		const std::size_t numTmSynapses,
		const std::size_t numTmSegments,
		const Status status
	);

	/**
	 * LayerHistory description;.
	 */
	~LayerHistory() = default;

public:

	friend std::ostream& operator<<(std::ostream& os, const LayerHistory& h);

};

std::ostream& operator<<(std::ostream& os, const LayerHistory& h);



/**
 * SaveLayerLog implementation in C++.
 * 
 * @b Description
 * SaveLayerLog is saver class for layer logs. This class is responsible 
 * for the save process of a single layer.
 */
class SaveLayerLog : public CoreSaver {

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

	std::vector<LayerHistory> hists_;
	csv::Csv csv_;

public:

	/**
	 * SaveLayerLog constructor.
	 */
	SaveLayerLog() = default;

	/**
	 * SaveLayerLog constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerLog(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerLog destructor.
	 */
	~SaveLayerLog() = default;

	/**
	 * Initialize SaveLayerLog with the parameters.
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
	void close() override {}

};



/**
 * SaveLayerLogCallback implementation in C++.
 * 
 * @b Description
 * SaveModelLogCallback is one of the Callback-series. This class saves
 * the data of layers to a csv file.
 */
class SaveLayerLogCallback : public SaveCallback {

private:

	std::vector<SaveLayerLog> logs_;

public:

	/**
	 * SaveLayerLogCallback constructor.
	 */
	SaveLayerLogCallback() = default;

	/**
	 * SaveLayerLogCallback constructor with the parameters.
	 * 
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the 
	 * real memory. Adjusting this value reduces the time required to 
	 * access io and speeds up the process.
	 */
	SaveLayerLogCallback(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerLogCallback destructor.
	 */
	~SaveLayerLogCallback() = default;

	/**
	 * Initialize SaveLayerLogCallback with the parameters.
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

#endif // SAVE_LAYER_LOG_CALLBACK_HPP