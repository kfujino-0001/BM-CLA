// TemporalMemoryExtension.hpp

/** @file
 * Definitions for the Temporal Memory in C++
 */

#ifndef NTA_TEMPORAL_MEMORY_EXTENSION_HPP
#define NTA_TEMPORAL_MEMORY_EXTENSION_HPP

#include <htm/algorithms/Connections.hpp>
#include <htm/types/Types.hpp>
#include <htm/types/Sdr.hpp>
#include <htm/types/Serializable.hpp>
#include <htm/utils/Random.hpp>
#include <htm/algorithms/AnomalyLikelihood.hpp>

#include <vector>
#include <unordered_map>
#include <functional>

#include <nlohmann/json.hpp>

#include "cla/extension/types/SdrExtension.hpp"
#include "cla/extension/types/PsdrExtension.hpp"

#include "cla/extension/algorithms/AdjusterFunctions.hpp"

namespace htm {

// using namespace std;
// using namespace htm;

using json = nlohmann::json;

struct TemporalMemoryExtensionParameters;
using TMEParameters = TemporalMemoryExtensionParameters;


// prototype definition for cross reference.
class CoreSelector;
enum class SegmentSelectorMode;

#ifndef SEGMENT_SELECTOR_HPP
	using PSelector = std::shared_ptr<CoreSelector>;
	using SSMode = SegmentSelectorMode;
#endif


/**
 * SegmentDutyCycle implementation in C++.
 * 
 * @b Description
 * The SegmentDutyCycle is extended class for handling the
 * segment information such as creating, destroying. This 
 * has the segments' duty cycle which means the prediction
 * accuracy of the segment in the range of the cycle.
 * 
 * More information is in SpatialPooler.hpp.
 */
class SegmentDutyCycle : public ConnectionsEventHandler {

private:

	UInt32 cycle_;
	std::unordered_map<Segment, pair<UInt32, Real>> dutyCycle_;

public:

	/**
	 * SegmentDutyCycle constructor.
	 */
	SegmentDutyCycle(const UInt32 cycle = 1000u);

	/**
	 * SegmentDutyCycle destructor.
	 */
	~SegmentDutyCycle() = default;

	/**
	 * Initialize SegmentDutyCycle
	 */
	void initialize(const UInt32 cycle = 1000u);

	/**
	 * Reset SegmentDutyCycle
	 */
	void reset();

	/**
	 * Update duty cycle of the segment.
	 * 
	 * @param segment The segment index.
	 * @param correct The whether segment is correct.
	 * 
	 * @return The whether the segment should be destroyed.
	 */
	void updateDutyCycle(
		const Segment segment,
		const bool correct
	);

	/**
	 * Get the whether of existing segment.
	 * 
	 * @param segment The segment index.
	 * 
	 * @return The whether of existing segment.
	 */
	const bool isExistSegment(const Segment segment) const;

	/**
	 * Get the duty cycle info of segment.
	 * 
	 * @param segment The segment index.
	 * 
	 * @return The duty cycle data (acc duty value, num cycle) of segment.
	 */
	const std::pair<UInt32, Real>& getDutyCycle(const Segment segment) const;

	/**
	 * Set cycle which is used to calculate duty cycle.
	 * 
	 * @param cycle The cycle value.
	 */
	inline void setCycle(const Int32 cycle) {
		cycle_ = cycle;
	}

	/**
	 * get the cycle value which is used to calculate duty cycle.
	 */
	inline const Int32 getCycle() const {
		return cycle_;
	}

	/**
	 * Called after a segment is created.
	 */
	void onCreateSegment(Segment segment) override;

	/**
	 * Called before a segment is destroyed.
	 */
	void onDestroySegment(Segment segment) override;

	void onCreateSynapse(Synapse synapse) override {}
	void onDestroySynapse(Synapse synapse) override {}
	void onUpdateSynapsePermanence(Synapse synapse, Permanence permanence) override {}

};



/**
 * TemporalMemoryConnectionsHandler implementation in C++.
 * 
 * @b Description
 * The TemporalMemorySynapseSegmentHandle is extended class for handling
 * the segment and synapse information such as creating, destroying, update.
 */
class TemporalMemoryConnectionsHandler : public ConnectionsEventHandler {

private:

	std::vector<Segment> createdSegments_;
	std::vector<Segment> destroyedSegments_;
	std::vector<Synapse> createdSynapses_;
	std::vector<Synapse> destroyedSynapses_;
	std::vector<std::pair<Synapse, Permanence>> updatePermanences_;

public:

	/**
	 * TemporalMemoryConnectionsHandler constructor.
	 */
	TemporalMemoryConnectionsHandler();

	/**
	 * TemporalMemoryConnectionsHandler destructor.
	 */
	~TemporalMemoryConnectionsHandler() = default;

	/**
	 * Initialize TemporalMemoryConnectionsHandler.
	 */
	inline void initialize() {
		reset();
	}

	/**
	 * Reset TemporalMemoryConnectionsHandler.
	 */
	void reset();

	/**
	 * Called after a segment is created.
	 */
	void onCreateSegment(Segment segment) override;

	/**
	 * Called before a segment is destroyed.
	 */
	void onDestroySegment(Segment segment) override;

	/**
	 * Called after a synapse is created.
	 */
	void onCreateSynapse(Synapse synapse) override;

	/**
 	 * Called before a synapse is destroyed.
	 */
	void onDestroySynapse(Synapse synapse) override;

	/**
	 * Called after a synapse's permanence crosses the connected threshold.
	 */
	void onUpdateSynapsePermanence(
		Synapse synapse,
		Permanence permanence
	) override;

	/**
	 * Get created segments in a step.
	 * 
	 * @return created segments.
	 */
	inline const std::vector<Segment>& getCreatedSegments() const {
		return createdSegments_;
	}

	/**
	 * Get destroyed segments in a step.
	 * 
	 * @return destroyed segments.
	 */
	inline const std::vector<Segment>& getDestroyedSegments() const {
		return destroyedSegments_;
	}

	/**
	 * Get created synapses in a step
	 * 
	 * @return created synapses
	 */
	inline const std::vector<Synapse>& getCreatedSynapses() const {
		return createdSynapses_;
	}

	/**
	 * Get destroyed synapses in a step
	 * 
	 * @return destroyed synapses
	 */
	inline const std::vector<Synapse>& getDestroyedSynapses() const {
		return destroyedSynapses_;
	}

	/**
	 * Get update permanence of the synapses in a step
	 * 
	 * @return the pair value (Synapse, Permanence)
	 */
	inline const std::vector<std::pair<Synapse, Permanence>>& getUpdatePermanences() const {
		return updatePermanences_;
	}

};

using TMConnectionsHandler = TemporalMemoryConnectionsHandler;



/**
 * Temporal Memory implementation in C++.
 *
 * Example usage:
 *
 *     SpatialPooler sp(inputDimensions, columnDimensions, <parameters>);
 *     TemporalMemoryExtension tm(columnDimensions, <parameters>);
 *
 *     while (true) {
 *        <get input vector, streaming spatiotemporal information>
 *        sp.compute(inputVector, learn, activeColumns)
 *        tm.compute(activeColumns, learn)
 *        <do something with the tm, e.g. classify tm.getActiveCells()>
 *     }
 */
class TemporalMemoryExtension : public htm::Serializable{

public:

	enum class ANMode { DISABLED = 0, RAW = 1, LIKELIHOOD = 2, LOGLIKELIHOOD = 3};
	
public:

	/**
	 * TemporalMemoryExtension constructor.
	 */
	TemporalMemoryExtension() = default;

	/**
	 * Initialize the temporal memory (TM) using the given parameters.
	 *
	 * @param columnDimensions
	 * Dimensions of the column space
	 *
	 * @param cellsPerColumn
	 * Number of cells per column
	 *
	 * @param activationThreshold
	 * If the number of active connected synapses on a segment is at least
	 * this threshold, the segment is said to be active.
	 *
	 * @param initialPermanence
	 * Initial permanence of a new synapse.
	 *
	 * @param connectedPermanence
	 * If the permanence value for a synapse is greater than this value, it
	 * is said to be connected.
	 *
	 * @param minThreshold
	 * If the number of potential synapses active on a segment is at least
	 * this threshold, it is said to be "matching" and is eligible for
	 * learning.
	 *
	 * @param maxNewSynapseCount
	 * The maximum number of synapses added to a segment during learning.
	 *
	 * @param permanenceIncrement
	 * Amount by which permanences of synapses are incremented during
	 * learning.
	 *
	 * @param permanenceDecrement
	 * Amount by which permanences of synapses are decremented during
	 * learning.
	 *
	 * @param predictedSegmentDecrement
	 * Amount by which segments are punished for incorrect predictions.
	 *
	 * Note: A good value is just a bit larger than
	 * (the column-level sparsity * permanenceIncrement). So, if column-level
	 * sparsity is 2% and permanenceIncrement is 0.01, this parameter should be
	 * something like 4% * 0.01 = 0.0004).
	 *
	 * @param seed
	 * Seed for the random number generator.
	 *
	 * @param maxSegmentsPerCell
	 * The maximum number of segments per cell.
	 * The value you can choose here is limited by the type SegmentIdx
	 * in Connections.hpp, change it if you need larger values. 
	 *
	 * @param maxSynapsesPerSegment
	 * The maximum number of synapses per segment.
	 * The value you can choose here is limited by the type SynapseIdx
	 * in Connections.hpp, change it there if you needed to use large values.
	 *
	 * @param checkInputs
	 * Whether to check that the activeColumns are sorted without
	 * duplicates. Disable this for a small speed boost.
	 * DEPRECATED: The SDR class now enforces these properties.
	 *
	 * @param externalPredictiveInputs
	 * Number of external predictive inputs.  These values are not related to this
	 * TM, they represent input from a different region.  This TM will form
	 * synapses with these inputs in addition to the cells which are part of this
	 * TemporalMemoryExtension.  If this is given (and greater than 0) then the active
	 * cells and winner cells of these external inputs must be given to methods
	 * TM.compute and TM.activateDendrites
	 *
	 * @param anomalyMode (optional, default `ANMode::RAW`)from enum ANMode, how is 
	 * `TM.anomaly` computed. Options ANMode {DISABLED, RAW, LIKELIHOOD, LOGLIKELIHOOD}
	 * 
	 */
	TemporalMemoryExtension(
		const vector<CellIdx>& columnDimensions,
		const CellIdx cellsPerColumn = 32u,
		const SynapseIdx activationThreshold = 13u,
		const Permanence initialPermanence = 0.21f,
		const Permanence connectedPermanence = 0.50f,
		const SynapseIdx minThreshold = 10u,
		const SynapseIdx maxNewSynapseCount = 20u,
		const Permanence permanenceIncrement = 0.10f,
		const Permanence permanenceDecrement = 0.10f,
		const Permanence predictedSegmentDecrement = 0.0f,
		const Int seed = 42,
		const SegmentIdx maxSegmentsPerCell = 255u,
		const SynapseIdx maxSynapsesPerSegment = 255u,
		const bool checkInputs = true,
		const UInt externalPredictiveInputs = 0u,
		const ANMode anomalyMode = ANMode::RAW
	);

	/**
	 * TemporalMemoryExtension constructor.
	 * 
	 * @param params The parameters of temporal memory extension.
	 */
	TemporalMemoryExtension(const TMEParameters& params);

	/**
	 * Initialize TemporalMemoryExtension.
	 */
	virtual void initialize(
		const vector<CellIdx>& columnDimensions = {2048u},
		const CellIdx cellsPerColumn = 32u,
		const SynapseIdx activationThreshold = 13u,
		const Permanence initialPermanence = 0.21f,
		const Permanence connectedPermanence = 0.50f,
		const SynapseIdx minThreshold = 10u,
		const SynapseIdx maxNewSynapseCount = 20u,
		const Permanence permanenceIncrement = 0.10f,
		const Permanence permanenceDecrement = 0.10f,
		const Permanence predictedSegmentDecrement = 0.0f,
		const Int seed = 42,
		const SegmentIdx maxSegmentsPerCell = 255u,
		const SynapseIdx maxSynapsesPerSegment = 255u,
		const bool checkInputs = true,
		const UInt externalPredictiveInputs = 0u,
		const ANMode anomalyMode = ANMode::RAW
	);

	/**
	 * Initialize TemporalMemoryExtension.
	 */
	void initialize(
		const htm::UInt numRegions = 1u,
		const std::vector<htm::CellIdx>& columnDimensions = {2048u},
		const htm::CellIdx cellsPerColumn = 32u,
		const htm::SynapseIdx activationThreshold = 13u,
		const htm::Permanence initialPermanence = 0.21f,
		const htm::Permanence connectedPermanence = 0.50f,
		const htm::SynapseIdx minThreshold = 10u,
		const htm::SynapseIdx maxNewSynapseCount = 20u,
		const htm::Permanence permanenceIncrement = 0.10f,
		const htm::Permanence permanenceDecrement = 0.10f,
		const htm::Permanence predictedSegmentDecrement = 0.0f,
		const htm::Int seed = 42,
		const htm::SegmentIdx maxSegmentsPerCell = 255u,
		const htm::SynapseIdx maxSynapsesPerSegment = 255u,
		const bool checkInputs = true,
		const bool exceptionHandling = false,
		const htm::UInt externalPredictiveInputs = 0u,
		const htm::Real synapseDestinationWeight = 0.5f,
		const htm::Real createSynWeight = 0.0f,
		const htm::Real destroySynWeight = 0.0f,
		const htm::Real activateWeight = 1.0f,
		const htm::UInt capacityOfNbActiveSegments = 0u,
		const htm::UInt capacityOfNbMatchingSegments = 0u,
		const SSMode innerSegmentSelectorMode = static_cast<SSMode>(0),
		const SSMode outerSegmentSelectorMode = static_cast<SSMode>(0),
		const TemporalMemoryExtension::ANMode anomalyMode
			= TemporalMemoryExtension::ANMode::RAW
	);

	/**
	 * Initialize TemporalMemoryExtension.
	 * 
	 * @param params The parameters of temporal memory extension.
	 */
	void initialize(const TMEParameters& params);

	/**
	 * TemporalMemoryExtension destructor.
	 */
	virtual ~TemporalMemoryExtension() = default;

	//----------------------------------------------------------------------
	//  Main functions
	//----------------------------------------------------------------------

	/**
	 * Get the version number of for the TM implementation.
	 *
	 * @returns Integer version number.
	 */
	virtual UInt version() const;

	/**
	 * Indicates the start of a new sequence.
	 * Resets sequence state of the TM.
	 */
	virtual void reset();

	/**
	 * Calculate the active cells, using the current active columns and
	 * dendrite segments. Grow and reinforce synapses.
	 *
	 * @param activeColumns
	 * A sorted list of active column indices.
	 *
	 * @param learn
	 * If true, reinforce / punish / grow synapses.
	 */
	void activateCells(
		const SDR &activeColumns, 
		const bool learn = true
	);

	/**
	 * Calculate dendrite segment activity, using the current active cells.  Call
	 * this method before calling getPredictiveCells, getActiveSegments, or
	 * getMatchingSegments.  In each time step, only the first call to this
	 * method has an effect, subsequent calls assume that the prior results are
	 * still valid.
	 *
	 * @param learn
	 * If true, segment activations will be recorded. This information is
	 * used during segment cleanup.
	 *
	 * @param externalPredictiveInputsActive
	 * (optional) SDR of active external predictive inputs.
	 *
	 * @param externalPredictiveInputsWinners
	 * (optional) SDR of winning external predictive inputs.  When learning, only these
	 * inputs are considered active.  
	 * externalPredictiveInputsWinners must be a subset of externalPredictiveInputsActive.  
	 *
	 * See TM::compute() for details of the parameters. 
	 *
	 */
	void activateDendrites(
		const bool learn,
		const SDR &externalPredictiveInputsActive, 
		const SDR &externalPredictiveInputsWinners
	);

	inline void activateDendrites(const bool learn = true) {
		const SDR externalPredictiveInputsActive(std::vector<UInt>{ externalPredictiveInputs });
		const SDR externalPredictiveInputsWinners(std::vector<UInt>{externalPredictiveInputs });
		activateDendrites(learn, externalPredictiveInputsActive, externalPredictiveInputsWinners);
	}

	/**
	 * Perform one time step of the Temporal Memory algorithm.
	 *
	 * This method calls activateDendrites, then calls activateCells. Using
	 * the TemporalMemoryExtension via its compute method ensures that you'll always
	 * be able to call getActiveCells at the end of the time step.
	 *
	 * Additionally, this method computes anomaly for `TM.anomaly&`, if you
	 * use other learning methods (activateCells(), activateDendrites()) your
	 * anomaly scores will be off. 
	 *
	 * @param activeColumns
	 * Sorted SDR of active columns.
	 *
	 * @param learn
	 * Whether or not learning is enabled.
	 *
	 * @param externalPredictiveInputsActive
	 * (optional) Vector of active external predictive inputs.  
	 * TM must be set up with the 'externalPredictiveInputs' constructor parameter for this use.
	 *
	 * @param externalPredictiveInputsWinners
	 * (optional) Vector of winning external predictive inputs.  When learning, only these
	 * inputs are considered active.  
	 * externalPredictiveInputsWinners must be a subset of externalPredictiveInputsActive.  
	 */
	virtual void compute(
		const SDR &activeColumns, 
		const bool learn,
		const SDR &externalPredictiveInputsActive, 
		const SDR &externalPredictiveInputsWinners
	);

	virtual void compute(
		const SDR &activeColumns, 
		const bool learn = true
	);

	// ==============================
	//  Helper functions
	// ==============================

	/**
	 * Create a segment on the specified cell. This method calls
	 * createSegment on the underlying connections, and it does some extra
	 * bookkeeping. Unit tests should call this method, and not
	 * connections.createSegment().
	 *
	 * @param cell
	 * Cell to add a segment to.
	 *
	 * @return Segment
	 * The created segment.
	 */
	Segment createSegment(const CellIdx& cell) {
		return connections_.createSegment(cell, maxSegmentsPerCell_); 
	}

	Synapse createSynapse(const Segment seg, CellIdx presyn, Permanence perm) {
		return connections_.createSynapse(seg, presyn, perm); 
	}

	void destroySegment(const Segment rm) {
		connections_.destroySegment(rm);
	}

	void destroySynapse(const Synapse syn) { 
		connections_.destroySynapse(syn);
	}

	/**
	 * Returns the indices of cells that belong to a mini-column.
	 *
	 * @param column Column index
	 *
	 * @return (vector<CellIdx>) Cell indices
	 */
	vector<CellIdx> cellsForColumn(const CellIdx column) const;

	/**
	 * Returns the number of cells in this layer.
	 *
	 * @return (size_t) Number of cells
	 */
	size_t numberOfCells(void) const {
		return connections.numCells();
	}

	/**
	 * Returns the indices of the active cells.
	 *
	 * @returns (std::vector<CellIdx>) Vector of indices of active cells.
	 */
	vector<CellIdx> getActiveCells() const; //TODO remove
	void getActiveCells(SDR &activeCells) const;

	/**
	 * @return SDR with indices of the predictive cells.
	 * SDR dimensions are {TM column dims x TM cells per column}
	 */
	SDR getPredictiveCells() const;

	SDR getPredictiveCells(const vector<Segment>& activeSegments) const;

	/**
	 * Returns the indices of the winner cells.
	 *
	 * @returns (std::vector<CellIdx>) Vector of indices of winner cells.
	 */
	vector<CellIdx> getWinnerCells() const; //TODO remove?
	void getWinnerCells(SDR &winnerCells) const;

	vector<Segment> getActiveSegments() const;
	vector<Segment> getMatchingSegments() const;

	/**
	 * Returns the dimensions of the columns in the region.
	 *
	 * @returns Integer number of column dimension
	 */
	vector<CellIdx> getColumnDimensions() const {
		return columnDimensions_;
	}

	/**
	 * Returns the dimensions of the cells in the region.
	 * 
	 * @returns Integer number of cells dimension.
	 */
	vector<CellIdx> getCellsDimensions() const;

	/**
	 * Returns the total number of columns.
	 *
	 * @returns Integer number of column numbers
	 */
	size_t numberOfColumns() const {
		return numColumns_; 
	}

	/**
	 * Returns the number of cells per column.
	 *
	 * @returns Integer number of cells per column
	 */
	size_t getCellsPerColumn() const {
		return cellsPerColumn_;
	}

	/**
	 * Returns the activation threshold.
	 *
	 * @returns Integer number of the activation threshold
	 */
	SynapseIdx getActivationThreshold() const;
	void setActivationThreshold(const SynapseIdx);

	/**
	 * Returns the initial permanence.
	 *
	 * @returns Initial permanence
	 */
	Permanence getInitialPermanence() const;
	void setInitialPermanence(const Permanence);

	/**
	 * Returns the connected permanance.
	 *
	 * @returns Returns the connected permanance
	 */
	Permanence getConnectedPermanence() const;

	/**
	 * Returns the minimum threshold.
	 *
	 * @returns Integer number of minimum threshold
	 */
	SynapseIdx getMinThreshold() const;
	void setMinThreshold(const SynapseIdx);

	/**
	 * Returns the maximum number of synapses that can be added to a segment
	 * in a single time step.
	 *
	 * @returns Integer number of maximum new synapse count
	 */
	SynapseIdx getMaxNewSynapseCount() const;
	void setMaxNewSynapseCount(const SynapseIdx);

	/**
	 * Get and set the checkInputs parameter.
	 */
	bool getCheckInputs() const;
	void setCheckInputs(bool);

	/**
	 * Returns the permanence increment.
	 *
	 * @returns Returns the Permanence increment
	 */
	Permanence getPermanenceIncrement() const;
	void setPermanenceIncrement(Permanence);

	/**
	 * Returns the permanence decrement.
	 *
	 * @returns Returns the Permanence decrement
	 */
	Permanence getPermanenceDecrement() const;
	void setPermanenceDecrement(Permanence);

	/**
	 * Returns the predicted Segment decrement.
	 *
	 * @returns Returns the segment decrement
	 */
	Permanence getPredictedSegmentDecrement() const;
	void setPredictedSegmentDecrement(Permanence);

	/**
	 * Returns the maxSegmentsPerCell.
	 *
	 * @returns Max segments per cell
	 */
	SegmentIdx getMaxSegmentsPerCell() const;

	/**
	 * Returns the maxSynapsesPerSegment.
	 *
	 * @returns Max synapses per segment
	 */
	SynapseIdx getMaxSynapsesPerSegment() const;

	/**
	 * Returns the exception handling.
	 * 
	 * @return true: Running exception handling.
	 * @return false: Sleep exception handling.
	 */
	bool getExceptionHandling() const;

	/**
	 * Returns the external predictive inputs.
	 * 
	 * @return UInt: external predictive inputs.
	 */
	UInt getExternalPredictiveInputs() const;

	/**
	 * Returns the synapse destination weight.
	 * 
	 * @return Real: The weight value for synapse destination.
	 */
	Real getSynapseDestinationWeight() const;

	/**
	 * Returns the anomaly mode of the temporal memory extension.
	 * 
	 * @return ANMode: The anomaly mode.
	 */
	ANMode getAnomalyMode() const;

	/**
	 * Returns the create synapse weight for the adaptive synapse adjustment.
	 * 
	 * @return Real: The create synapse weight.
	 */
	Real getCreateSynWeight() const;

	/**
	 * Returns the destroy synapse weight for the adaptive synapse adjustment.
	 * 
	 * @return Real: The destroy synapse weight.
	 */
	Real getDestroySynWeight() const;

	/**
	 * Returns the activate weight for the adaptive synapse adjustment.
	 * 
	 * @return Real: The activate weight.
	 */
	Real getActivateWeight() const;

	/**
	 * Returns the activate segments capacity for the adaptive decoding.
	 * 
	 * @return UInt: The activate segments capacity.
	 */
	UInt getActivateSegmentsCapacity() const;
	
	/**
	 * Returns the matching segments capacity for the adaptive decoding.
	 * 
	 * @return UInt: The matching segments capacity.
	 */
	UInt getMatchingSegmentsCapacity() const;
	
	/**
	 * Returns the inner segment selector mode.
	 * 
	 * @return SSMode: The inner segment selector mode.
	 */
	SSMode getInnerSegmentSelectorMode() const;
	
	/**
	 * Returns the outer segment selector mode.
	 * 
	 * @return SSMode: The outer segment selector mode.
	 */
	SSMode getOuterSegmentSelectorMode() const;


	// /**
	//  * Returns the TemporalMemorySynapseHandler
	//  * 
	//  * @returns The synapse handler
	//  */
	// inline const TMSynapseHandler& getSynapseHandler() const {
	// 	return handler_;
	// }

	/**
	 * Returns the TemporalMemoryConnectionsHandler.
	 * 
	 * @return The synapse segment handler.
	 */
	inline const TMConnectionsHandler& getSynapseHandler() const {
		return handler_;
	}

	/**
	 * Returns the TemporalMemory segment duty cycle handler.
	 * 
	 * @return The segment duty cycle handler.
	 */
	inline const SegmentDutyCycle& getSegmentDutyCycle() const {
		return segmentDutyCycle_;
	}

	

	/**
	 * Save (serialize) / Load (deserialize) the current state of the spatial pooler
	 * to the specified stream.
	 *
	 * @param Archive & ar   a Cereal container.
	 */
	// a container to hold the data for one sequence item during serialization
	struct container_ar {
		SegmentIdx idx;
		CellIdx cell;
		SynapseIdx syn;

		template<class Archive>
		void save_ar(Archive & ar) const {
			ar(
				CEREAL_NVP(idx),
				CEREAL_NVP(cell),
				CEREAL_NVP(syn)
			);
		}
		template<class Archive>
		void load_ar(Archive & ar) {
			ar(
				CEREAL_NVP(idx),
				CEREAL_NVP(cell),
				CEREAL_NVP(syn)
			);
		}
	};

	CerealAdapter;

	template<class Archive>
	void save_ar(Archive & ar) const {
		ar(CEREAL_NVP(numColumns_),
		CEREAL_NVP(cellsPerColumn_),
		CEREAL_NVP(activationThreshold_),
		CEREAL_NVP(initialPermanence_),
		CEREAL_NVP(connectedPermanence_),
		CEREAL_NVP(minThreshold_),
		CEREAL_NVP(maxNewSynapseCount_),
		CEREAL_NVP(checkInputs_),
		CEREAL_NVP(permanenceIncrement_),
		CEREAL_NVP(permanenceDecrement_),
		CEREAL_NVP(predictedSegmentDecrement_),
		CEREAL_NVP(externalPredictiveInputs_),
		CEREAL_NVP(maxSegmentsPerCell_),
		CEREAL_NVP(maxSynapsesPerSegment_),
		CEREAL_NVP(rng_),
		CEREAL_NVP(columnDimensions_),
		CEREAL_NVP(activeCells_),
		CEREAL_NVP(winnerCells_),
		CEREAL_NVP(segmentsValid_),
		CEREAL_NVP(tmAnomaly_.anomaly_),
		CEREAL_NVP(tmAnomaly_.mode_),
		CEREAL_NVP(tmAnomaly_.anomalyLikelihood_),
		CEREAL_NVP(connections_));
		
		size_t activeSize = activeSegmentsForInner_.size();
		ar(CEREAL_NVP(activeSize));

		if (activeSize > 0) {
		cereal::size_type numActiveSegments = activeSegmentsForInner_.size();
		ar( cereal::make_size_tag(numActiveSegments));
		for (Segment segment : activeSegmentsForInner_) {
			struct container_ar c;
			c.cell = connections.cellForSegment(segment);
			const vector<Segment> &segments = connections.segmentsForCell(c.cell);

			c.idx = (SegmentIdx)std::distance(
								segments.begin(), 
								std::find(segments.begin(), 
								segments.end(), segment));
			c.syn = numActiveConnectedSynapsesForSegment_[segment];
			ar(c); // to keep iteration counts correct, only serialize one item per iteration.
		}
		}

		size_t matchSize = matchingSegmentsForInner_.size();
		ar(CEREAL_NVP(matchSize));
		if (matchSize > 0) {
		cereal::size_type numMatchingSegments = matchingSegmentsForInner_.size();
		ar(cereal::make_size_tag(numMatchingSegments));
		for (Segment segment : matchingSegmentsForInner_) {
			struct container_ar c;
			c.cell = connections.cellForSegment(segment);
			const vector<Segment> &segments = connections.segmentsForCell(c.cell);

			c.idx = (SegmentIdx)std::distance(segments.begin(), std::find(segments.begin(), segments.end(), segment));
			c.syn = numActivePotentialSynapsesForSegment_[segment];
			ar(c);
		}
		}

	}

	template<class Archive>
	void load_ar(Archive & ar) {
		ar(
			CEREAL_NVP(numColumns_),
			CEREAL_NVP(cellsPerColumn_),
			CEREAL_NVP(activationThreshold_),
			CEREAL_NVP(initialPermanence_),
			CEREAL_NVP(connectedPermanence_),
			CEREAL_NVP(minThreshold_),
			CEREAL_NVP(maxNewSynapseCount_),
			CEREAL_NVP(checkInputs_),
			CEREAL_NVP(permanenceIncrement_),
			CEREAL_NVP(permanenceDecrement_),
			CEREAL_NVP(predictedSegmentDecrement_),
			CEREAL_NVP(externalPredictiveInputs_),
			CEREAL_NVP(maxSegmentsPerCell_),
			CEREAL_NVP(maxSynapsesPerSegment_),
			CEREAL_NVP(rng_),
			CEREAL_NVP(columnDimensions_),
			CEREAL_NVP(activeCells_),
			CEREAL_NVP(winnerCells_),
			CEREAL_NVP(segmentsValid_),
			CEREAL_NVP(tmAnomaly_.anomaly_),
			CEREAL_NVP(tmAnomaly_.mode_),
			CEREAL_NVP(tmAnomaly_.anomalyLikelihood_),
			CEREAL_NVP(connections_)
		);
			
		size_t activeSize;
		ar(CEREAL_NVP(activeSize));

		if (activeSize > 0) {
			numActiveConnectedSynapsesForSegment_.assign(connections.segmentFlatListLength(), 0);
			cereal::size_type numActiveSegments;
			ar(cereal::make_size_tag(numActiveSegments));
			activeSegmentsForInner_.resize(static_cast<size_t>(numActiveSegments));
			for (size_t i = 0; i < static_cast<size_t>(numActiveSegments); i++) {
				struct container_ar c;
				ar(c);  
				Segment segment = connections.getSegment(c.cell, c.idx);
				activeSegmentsForInner_[i] = segment;
				numActiveConnectedSynapsesForSegment_[segment] = c.syn;
			}
		}

		size_t matchSize;
		ar(CEREAL_NVP(matchSize));

		if (matchSize > 0) {
			numActivePotentialSynapsesForSegment_.assign(connections.segmentFlatListLength(), 0);
			cereal::size_type numMatchingSegments;
			ar(cereal::make_size_tag(numMatchingSegments));
			matchingSegmentsForInner_.resize(static_cast<size_t>(numMatchingSegments));
			for (size_t i = 0; i < static_cast<size_t>(numMatchingSegments); i++) {
				struct container_ar c;
				ar(c);
				Segment segment = connections.getSegment(c.cell, c.idx);
				matchingSegmentsForInner_[i] = segment;
				numActivePotentialSynapsesForSegment_[segment] = c.syn;
			}
		}
	}

	virtual bool operator==(const TemporalMemoryExtension &other) const;
	inline bool operator!=(const TemporalMemoryExtension &other) const {
		return ! (this->operator==(other));
	}

	//----------------------------------------------------------------------
	// Debugging helpers
	//----------------------------------------------------------------------

	/**
	 * Print diagnostic info
	 */
	friend std::ostream& operator<< (
		std::ostream& stream, 
		const TemporalMemoryExtension& self
	);

	/**
	 * Print the main TM creation parameters
	 */
	void printParameters(std::ostream& out=std::cout) const;

	/**
	 * Returns the index of the (mini-)column that a cell belongs to.
	 * 
	 * Mini columns are an organizational unit in TM, 
	 * each mini column consists for cellsPerColumns cells. 
	 * There's no topology between cells within a mini-column, cells
	 * are organized as a flat array 
	 * `col{i} = [cell{i*CPS}, cell{i*CPS +1}, ..., cell{i*CPS + CPS-1}], 
	 * where CPS stands for cellsPerColumn`
	 *
	 * @param cell Cell index
	 *
	 * @return (int) Column index
	 */
	UInt columnForCell(const CellIdx cell) const;

	/**
	 *  cellsToColumns
	 *  converts active cells to columnar representation, 
	 *  see columnForCell() for details.
	 *
	 *  @param const SDR& cells - input cells, size must be a multiple of cellsPerColumn; ie. 
	 *    all SDRs obtained from TM's get*Cells(SDR) are valid. 
	 *    The SDR cells dimensions must be: {TM.getColumnDimensions, TM.getCellsPerColumn()}
	 *
	 *  @return SDR cols - which is size of TM's getColumnDimensions()
	 *
	 */
	SDR cellsToColumns(const SDR& cells) const;

	/**
	 * Calculate the indices of cells that belong to a mini-column with
	 * the number of active cells.
	 */
	SDRex<NumCells> countCellsOfColumns(const SDR& cells) const ;

	// /**
	//  * Identify the active column using the prediction cell.
	//  * This function assumes that cells in the predictive
	//  * state are highly trusted by a large number of layers and uses 
	//  * them preferentially for active column selection.
	//  * 
	//  * @param predictiveCells The predictive cells.
	//  * @param maxActiveColumns The max number of active columns, which 
	//  * are inferred from predictive cells.
	//  * @param activeColumns The active columns.
	//  */
	// void identifyActiveColumns(
	// 	const SDR& predictiveCells,
	// 	const UInt maxActiveColumns,
	// 	SDR& activeColumns
	// ) const;

	/**
	 * Calculate the average value of the number of synapses on the
	 * predictive cells (active segment).
	 * 
	 * @return The average value.
	 */
	const double calAverageNumSynapsesOnActivateSegment() const;

	/**
	 * Calculate the average value of the number of active synapses
	 * on the predictive cells (active segment).
	 */
	const double calAverageNumActiveSynapsesOnActiveSegment() const;

private:

	void destroySynapsesByASA_(
		vector<Segment>::const_iterator columnActiveSegmentBegin,
		vector<Segment>::const_iterator columnActiveSegmentEnd
	);

	void punishPredictedColumn_(
		vector<Segment>::const_iterator columnMatchingSegmentsBegin, 
		vector<Segment>::const_iterator columnMatchingSegmentsEnd, 
		const SDR& prevActiveCells
	);

	void activatePredictedColumn_(
		vector<Segment>::const_iterator columnActiveSegmentsBegin,
		vector<Segment>::const_iterator columnActiveSegmentsEnd,
		const SDR &prevActiveCells,
		const vector<CellIdx> &prevWinnerCells,
		const bool learn
	);

	void burstColumn_(
		const UInt column,
		vector<Segment>::const_iterator columnMatchingSegmentsBegin,
		vector<Segment>::const_iterator columnMatchingSegmentsEnd,
		const SDR &prevActiveCells,
		const vector<CellIdx> &prevWinnerCells,
		const bool learn
	);

	void growSynapses_(
		const Segment& segment,
		const SynapseIdx nDesiredNewSynapses,
		const vector<CellIdx> &prevWinnerCells
	);

	const bool isExceptionHandling_(
		const SynapseIdx numActivePotentialSynapses,
		const SynapseIdx numWinnerPotentialSynapses
	) const;

	CellIdx getLeastUsedCell_(const CellIdx column);

	void calculateAnomalyScore_(const SDR &activeColumns);

protected:

	//all these could be const
	CellIdx numColumns_;
	vector<CellIdx> columnDimensions_;
	CellIdx cellsPerColumn_;
	SynapseIdx activationThreshold_;
	SynapseIdx minThreshold_;
	SynapseIdx maxNewSynapseCount_;
	bool checkInputs_;
	Permanence initialPermanence_;
	Permanence connectedPermanence_;
	Permanence permanenceIncrement_;
	Permanence permanenceDecrement_;
	Permanence predictedSegmentDecrement_;
	UInt externalPredictiveInputs_;
	SegmentIdx maxSegmentsPerCell_;
	SynapseIdx maxSynapsesPerSegment_;

	Real synapseDestinationWeight_;

	bool exceptionHandling_;

	// for adaptive synapse adjustment.
	Real createSynWeight_;
	Real destroySynWeight_;
	Real activateWeight_;

	// for continuous prediction.
	UInt activateSegmentsCapacity_;
	UInt matchingSegmentsCapacity_;
	SSMode innerSegmentSelectorMode_;
	SSMode outerSegmentSelectorMode_;

private:

	vector<CellIdx> activeCells_;
	vector<CellIdx> winnerCells_;
	bool segmentsValid_;
	vector<Segment> activeSegmentsForInner_;
	vector<Segment> activeSegmentsForOuter_;
	vector<Segment> matchingSegmentsForInner_;
	vector<SynapseIdx> numActiveConnectedSynapsesForSegment_;
	vector<SynapseIdx> numActivePotentialSynapsesForSegment_;

	vector<SynapseIdx> numWinnerConnectedSynapsesForSegment_; 
	vector<SynapseIdx> numWinnerPotentialSynapsesForSegment_;

	Random rng_;

	// select the pow adjuster.
	Adjuster<adjuster::Pow> adjuster_;

	PSelector innerSelector_;
	PSelector outerSelector_;

	/**
	 * holds logic and data for TM's anomaly
	 */
	struct anomaly_tm {
		protected:
		friend class TemporalMemoryExtension;
		Real anomaly_ = 0.5f; //default value
		ANMode mode_ = ANMode::RAW;
		AnomalyLikelihood anomalyLikelihood_; //TODO provide default/customizable params here
	};

	Connections connections_;

	// TMSynapseHandler handler_;
	TMConnectionsHandler handler_;
	SegmentDutyCycle segmentDutyCycle_;

public:
	const Connections& connections = connections_; //const view of Connections for the public

	const UInt &externalPredictiveInputs = externalPredictiveInputs_;

	anomaly_tm tmAnomaly_;
	/*
	*  anomaly score computed for the current inputs
	*  (auto-updates after each call to TM::compute())
	*
	*  @return a float value from computeRawAnomalyScore()
	*  from Anomaly.hpp
	*/
	const Real &anomaly = tmAnomaly_.anomaly_; //this is position dependant, the struct anomaly_tm must be defined before this use,
	// otherwise this surprisingly compiles, but a call to `tmAnomaly_.anomaly` segfaults!

};



/**
 * TemporalMemoryExtensionParameters implementation in C++.
 * 
 * @b Description
 * The TemporalMemoryExtensionExtensionParameters contains the parameter of
 * TemporalMemoryExtension and TemporalMemoryExtension. This class supports
 * json files.
 * 
 * @param exceptionHandling
 * The boolean value whether do exception handling or do not.
 * 
 * @param synapseDestinationWeight
 * The value that determines how much of synapses are distributed to
 * its own layer when it is placed between multiple layers.
 * This value means r in 
 * A Preliminary Study of Cortical Learning Algorithm for Action Decision
 * Using Forecast.
 * 
 * For any other params
 * See htm/algorithm/TemporalMemoryExtension.hpp
 */
struct TemporalMemoryExtensionParameters {

	// for normal temporal memory.
	std::vector<htm::CellIdx> columnDimensions;
	htm::CellIdx cellsPerColumn = 32u;
	htm::SynapseIdx activationThreshold = 13u;
	htm::Permanence initialPermanence = 0.21f;
	htm::Permanence connectedPermanence = 0.50f;
	htm::SynapseIdx minThreshold = 10u;
	htm::SynapseIdx maxNewSynapseCount = 20u;
	htm::Permanence permanenceIncrement = 0.10f;
	htm::Permanence permanenceDecrement = 0.10f;
	htm::Permanence predictedSegmentDecrement = 0.0f;
	htm::Int seed = 42;
	htm::SegmentIdx maxSegmentsPerCell = 255u;
	htm::SynapseIdx maxSynapsesPerSegment = 255u;
	bool checkInputs = true;
	bool exceptionHandling = false;
	htm::UInt externalPredictiveInputs = 0u;
	htm::Real synapseDestinationWeight = 0.5;
	TemporalMemoryExtension::ANMode anomalyMode
		= TemporalMemoryExtension::ANMode::RAW;

	// for adaptive synapse adjustment.
	htm::Real createSynWeight = 0.0f;
	htm::Real destroySynWeight = 0.0f;
	htm::Real activateWeight = 1.0f;

	// for adaptive prediction.
	htm::UInt activateSegmentsCapacity = 0u;
	htm::UInt matchingSegmentsCapacity = 0u;
	SSMode innerSegmentSelectorMode = static_cast<SSMode>(0u);
	SSMode outerSegmentSelectorMode = static_cast<SSMode>(0u);
	htm::UInt numRegions = 1u;
	
	/**
	 * TemporalMemoryExtensionParameters constructor
	 */
	TemporalMemoryExtensionParameters() = default;

};

std::ostream& operator<<(
	std::ostream& os, 
	const TemporalMemoryExtensionParameters& params
);

void to_json(json& j, const TemporalMemoryExtensionParameters& p);
void from_json(const json& j, TemporalMemoryExtensionParameters& p);

} // namespace cla

#endif // NTA_TEMPORAL_MEMORY_EXTENSION_HPP
