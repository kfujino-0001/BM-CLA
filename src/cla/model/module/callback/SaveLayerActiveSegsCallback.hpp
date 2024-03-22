// SaveLayerActiveSegsCallback.hpp

/**
 * @file
 * Definitions for the SaveLayerActiveSegsCallback class in C++
 */

#ifndef SAVE_ACTIVATE_SEGS_CALLBACK_HPP
#define SAVE_ACTIVATE_SEGS_CALLBACK_HPP

#include <tuple>
#include <vector>

#include "cla/extension/algorithms/TemporalMemoryExtension.hpp"
#include "htm/types/Sdr.hpp"

#include "cla/model/core/CoreLayer.hpp"
#include "cla/model/module/callback/SaveCallback.hpp"


namespace cla {

using NumSyns = htm::UInt;

/**
 * ActiveSegDataHistory implementation in C++.
 *
 * @b Description
 * The ActiveSegDataHistory is a data structure. It contains the
 * active segment data each step. This class saves the data in
 * the json file.
 *
 * @param step
 * The step of the synapse history.
 *
 * @param activeSegs
 * The data of active segments. For more detail, see the definition
 * of ActiveSegData.
 */
struct ActiveSegDataHistory {

public:

	/**
	 * ActiveSegData definition.
	 *
	 * elem[0]: Segment id.
	 * elem[1]: Cell id the segment belong to.
	 * elem[2]: The number of synapses connected to the inner active cells.
	 * elem[3]: The number of synapses connected to the outer active cells.
	 */
	struct ActiveSegData {
		htm::Segment segment;
		htm::CellIdx cell;
		NumSyns numSynsFromInner;
		NumSyns numSynsFromOuter;

		/**
		 * ActiveSegData constructor.
		 */
		ActiveSegData() = default;

		/**
		 * ActiveSegData constructor with the parameters.
		 *
		 * @param segment Segment id.
		 * @param cell  Cell id the segment belong to.
		 * @param numSynsFromInner The number of synapses connected to the
		 * inner active cells.
		 * @param numSynsFromOuter The number of synapses connected to the
		 * outer active cells.
		 */
		ActiveSegData(
			const htm::Segment segment,
			const htm::CellIdx cell,
			const NumSyns numSynsFromInner,
			const NumSyns numSynsFromOuter
		);
	};

public:

	Step step;
	std::vector<ActiveSegData> activeSegs;

public:

	/**
	 * ActiveSegDataHistory constructor.
	 */
	ActiveSegDataHistory() = default;

	/**
	 * ActiveSegDataHistory constructor with the parameters.
	 *
	 * @param step
	 * @param activeSegs
	 */
	ActiveSegDataHistory(
		const Step step,
		const std::vector<ActiveSegData>& activeSegs
	);

	/**
	 * ActiveSegDataHistory constructor with the parameters.
	 *
	 * @param step The time step.
	 * @param activeSegments The active segments of the layer.
	 * @param connections The connections which has synapse information.
	 * @param externalCellThreshold The threshold whether the cell is
	 * inside or outside of the layer.
	 */
	ActiveSegDataHistory(
		const Step step,
		const std::vector<htm::Segment>& activeSegments,
		const htm::Connections& connections,
		const htm::UInt externalCellThreshold
	);

	/**
	 * ActiveSegDataHistory constructor with the parameters.
	 *
	 * @param step The time step.
	 * @param activeSegments The active segments of the layer.
	 * @param connections The connections which has synapse information.
	 */
	ActiveSegDataHistory(
		const Step step,
		const std::vector<htm::Segment>& activeSegments,
		const htm::Connections& connections
	);

	/**
	 * ActiveSegDataHistory destructor.
	 */
	~ActiveSegDataHistory() = default;

public:

	friend std::ostream& operator<<(
		std::ostream& os,
		const ActiveSegDataHistory& h
	);
};

void to_json(json& j, const ActiveSegDataHistory::ActiveSegData& d);
std::ostream& operator<<(std::ostream& os, const ActiveSegDataHistory& h);



/**
 * SaveActiveSegmentsLog implementation in C++.
 *
 * @b Description
 * SaveActiveSegmentLog is save class for layer active segments. This class
 * is responsible for the save process for a single layer.
 */
class SaveActiveSegmentsLog : public CoreSaver {

private:

	std::string path_;
	std::string filename_;
	Step nbHoldingSteps_;

	bool firstInput_;

	std::vector<ActiveSegDataHistory> hists_;

public:

	/**
	 * SaveActiveSegmentsLog constructor.
	 */
	SaveActiveSegmentsLog() = default;

	/**
	 * SaveActiveSegmentsLog constructor with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 */
	SaveActiveSegmentsLog(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveActiveSegmentsLog destructor.
	 */
	~SaveActiveSegmentsLog() = default;

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
	void add(const Step step, const PLayer& layer);

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
 * SaveLayerActiveSegsCallback implementation in C++.
 *
 * @b Description
 * SaveLayerActiveSegsCallback is one of the Callback-series. This class saves
 * the active segments data of layers to a json file.
 */
class SaveLayerActiveSegsCallback : public SaveCallback {

private:

	std::vector<SaveActiveSegmentsLog> logs_;

public:

	/**
	 * SaveLayerActiveSegsCallback constructor.
	 */
	SaveLayerActiveSegsCallback() = default;

	/**
	 * SaveLayerActiveSegsCallback constructor with the parameters.
	 *
	 * @param path The file path where the data will be saved.
	 * @param filename The filename where the data will be saved.
	 * @param nbHoldingSteps The number of steps to hold the data in the
	 * real memory. Adjusting this value reduces the time required to
	 * access io and speeds up the process.
	 */
	SaveLayerActiveSegsCallback(
		const std::string& path,
		const std::string& filename,
		const Step nbHoldingSteps
	);

	/**
	 * SaveLayerActiveSegsCallback destructor.
	 */
	~SaveLayerActiveSegsCallback() = default;

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

#endif // SAVE_ACTIVATE_SEGS_CALLBACK_HPP