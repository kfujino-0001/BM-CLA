// LayerProxy.hpp

/**
 * @file
 * Definitions for the LayerProxy class in C++
 */

#ifndef LAYER_PROXY_HPP
#define LAYER_PROXY_HPP

#include <memory>
#include <vector>

#include "cla/extension/types/PSdrExtension.hpp"
#include "cla/extension/types/Psdr.hpp"
#include "cla/model/core/CoreSpatialPooler.hpp"
#include "cla/model/core/CoreTemporalMemory.hpp"
#include "cla/model/module/helper/SDRContainer.hpp"
#include "cla/utils/Status.hpp"

namespace cla {

using SynapseConnection = std::tuple<htm::Synapse, htm::CellIdx, htm::CellIdx>;
using SynapsePermanence = std::pair<htm::Synapse, htm::Permanence>;

/**
 * LayerProxy implementation in C++.
 *
 * @b Description
 * The LayerProxy is a proxy class for the layer. This class contains the
 * multiple SDRs and the layer status. Additionally, This class calculates
 * some type of SDR according to external requests.
 */
class LayerProxy {

private:

	Status status_;

	const SDRContainer& container_;
	const std::vector<PSpatialPooler>& sps_;
	const PTemporalMemory& tm_;


private:

	mutable htm::SDR predictiveCells_;
	mutable htm::SDRex<htm::NumCells> predictiveColumns_;
	mutable htm::SDRex<htm::NumCells> predictiveBits_;

	mutable bool predictiveCellsValid_;
	mutable bool predictiveColumnsValid_;
	mutable bool predictiveBitsValid_;

	mutable htm::SDR burstColumns_;
	mutable bool burstColumnsValid_;

private:

	/************************************************
	 * Converter callbacks.
	 ***********************************************/

	const htm::SDR_sparse_t seg2cells_(const htm::ElemSparse seg) const;
	const htm::SDR_sparse_t cell2columns_(const htm::ElemSparse cell) const;
	const htm::SDR_sparse_t column2bits_(const htm::ElemSparse column) const;

	/************************************************
	 * Reducter callbacks.
	 ***********************************************/

	const htm::NumCells countCells_(
		const htm::NumCells value,
		const htm::ElemSparse inputIdx,
		const htm::ElemSparse outputIdx
	) const;

	const htm::NumCells sumCells_(
		const htm::NumCells value,
		const htm::NumCells sum,
		const htm::ElemSparse inputIdx,
		const htm::ElemSparse outputIdx
	) const;


	/************************************************
	 * Converter functions.
	 ***********************************************/

	template <typename ConvertFunc>
	void convert_(
		const htm::SDR_sparse_t& inputSparse,
		htm::SDR& outputSDR,
		const ConvertFunc& converter
	) const {
		outputSDR.zero();
		htm::SDR_dense_t outputDense(outputSDR.getDense().size());

		for(const auto inputIdx : inputSparse) {
			for(const auto outputIdx : converter(inputIdx)) {
				outputDense.at(outputIdx) = static_cast<htm::ElemDense>(1);
			}
		}

		outputSDR.setDense(outputDense);
	}

	template <typename ConvertFunc>
	void convert_(
		const htm::SDR& inputSDR,
		htm::SDR& outputSDR,
		const ConvertFunc& converter
	) const {
		outputSDR.zero();
		htm::SDR_dense_t outputDense(outputSDR.getDense().size());

		for(const auto inputIdx : inputSDR.getSparse()) {
			for(const auto outputIdx : converter(inputIdx)) {
				outputDense.at(outputIdx) = static_cast<htm::ElemDense>(1);
			}
		}

		outputSDR.setDense(outputDense);
	}

	template <
		typename ExDataType,
		typename ConvertFunc,
		typename ReductionFunc>
	void convert_(
		const htm::SDR& inputSDR,
		htm::SDRex<ExDataType>& outputSDR,
		const ConvertFunc& converter,
		const ReductionFunc& reductor
	) const {

		const std::size_t size = outputSDR.getDense().size();

		outputSDR.zero();
		htm::SDR_dense_t outputDense(size);
		std::vector<ExDataType> outputData(size, static_cast<ExDataType>(0));

		for(const auto inputIdx : inputSDR.getSparse()) {
			for(const auto outputIdx : converter(inputIdx)) {
				outputDense.at(outputIdx) = static_cast<htm::ElemDense>(1);
				outputData.at(outputIdx) = reductor(
					outputData.at(outputIdx), inputIdx, outputIdx
				);
			}
		}

		outputSDR.setDense(outputDense);
		outputSDR.setExDataDense(outputData);
	}

	template <
		typename ExDataType,
		typename ConvertFunc,
		typename ReductionFunc>
	void convert_(
		const htm::SDRex<ExDataType>& inputSDR,
		htm::SDRex<ExDataType>& outputSDR,
		const ConvertFunc& converter,
		const ReductionFunc& reductor
	) const {
		const auto& inputSparse = inputSDR.getSparse();
		const auto& inputData = inputSDR.getExDataDense();

		const std::size_t size = outputSDR.getDense().size();

		outputSDR.zero();
		htm::SDR_dense_t outputDense(size);
		std::vector<ExDataType> outputData(size, static_cast<ExDataType>(0));

		for(const auto inputIdx : inputSparse) {
			for(const auto outputIdx : converter(inputIdx)) {
				outputDense.at(outputIdx) = static_cast<htm::ElemDense>(1);
				outputData.at(outputIdx) = reductor(
					inputData.at(inputIdx), outputData.at(outputIdx),
					inputIdx, outputIdx
				);
			}
		}

		outputSDR.setDense(outputDense);
		outputSDR.setExDataDense(outputData);
	}


private:

	void segsToCells_(
		const htm::SDR_sparse_t& segs, 
		htm::SDR& cells
	) const;

	void cellsToColumns_(
		const htm::SDR& cells,
		htm::SDRex<htm::NumCells>& columns
	) const;

	void columnsToBits_(
		const htm::SDRex<htm::NumCells>& columns,
		htm::SDRex<htm::NumCells>& bits
	) const;

	void updateBurstColumns_(
		const htm::SDR& prePredictiveColumns,
		const htm::SDR& activeColumns,
		htm::SDR& burstColumns
	) const;

public:

	/**
	 * LayerProxy constructor.
	 */
	LayerProxy() = default;

	/**
	 * LayerProxy constructor with the parameters.
	 *
	 * @param container The original sdr container on this layer.
	 * @param sps The pointers of the spatial poolers on this layer.
	 * @param tm The pointer of the temporal memory on this layer.
	 */
	LayerProxy(
		const SDRContainer& container,
		const std::vector<PSpatialPooler>& sps,
		const PTemporalMemory& tm
	);

	/**
	 * LayerProxy destructor.
	 */
	~LayerProxy() = default;

	/**
	 * Reset the layer proxy.
	 */
	void reset();

	/**
	 * Set the status.
	 * 
	 * @param status The status of the layer.
	 */
	void setStatus(const Status status);

	/**
	 * Get the layer status.
	 *
	 * @return const Status& The layer status.
	 */
	const Status& getStatus() const;

	/**
	 * Get the active bits. This sdr is adapted by the layer adapter.
	 *
	 * @return const htm::PSDR& The partial sdr for the active bits.
	 */
	const htm::SDR& getActiveBits() const;

	/**
	 * Get the active columns.
	 *
	 * @return const htm::PSDR& The partial sdr for the active columns.
	 */
	const htm::SDR& getActiveColumns() const;

	/**
	 * Get the burst columns.
	 *
	 * @return const htm::PSDR& The partial sdr for the burst columns.
	 */
	const htm::SDR& getBurstColumns() const;

	/**
	 * Get the active cells.
	 *
	 * @return const htm::SDR& The sdr for the active cells.
	 */
	const htm::SDR& getActiveCells() const;

	/**
	 * Get the winner cells.
	 *
	 * @return const htm::SDR& The sdr for the winner cells.
	 */
	const htm::SDR& getWinnerCells() const;

	/**
	 * Get the external active sdrs.
	 * 
	 * @return const htm::SDR& The sdr for the external actives.
	 */
	const htm::SDR& getExternalActives() const;

	/**
	 * Get the external winner sdrs.
	 * 
	 * @return const htm::SDR& The sdr for the external winners.
	 */
	const htm::SDR& getExternalWinners() const;

	/**
	 * Get the active segments.
	 *
	 * @return const htm::SDR& The sdr for the active segments.
	 */
	const htm::SDR_sparse_t& getActiveSegments() const;

	/**
	 * Get the predictive cells.
	 *
	 * @return const htm::SDR& The sdr for the predictive cells.
	 */
	const htm::SDR& getPredictiveCells() const;

	/**
	 * Get the predictive columns.
	 *
	 * @return const htm::SDR& The sdr for the predictive columns.
	 */
	const htm::SDR& getPredictiveColumns() const;

	/**
	 * Get the predictive columns with the number of the predictive cells.
	 *
	 * @return const htm::SDRex<htm::NumCells>& The sdr for the predictive
	 * columns with the number of the predictive cells.
	 */
	const htm::SDRex<htm::NumCells>& getPredictiveColumnsWithNbPCells() const;

	/**
	 * Get the predictive bits.
	 *
	 * @return const htm::SDR& The sdr for the predictive bits.
	 */
	const htm::SDR& getPredictiveBits() const;

	/**
	 * Get tht predictive bits with the number of the predictive cells.
	 *
	 * @return const htm::SDRex<htm::NumCells>& The sdr for the predictive
	 * bits with the number of the predictive cells.
	 */
	const htm::SDRex<htm::NumCells>& getPredictiveBitsWithNbPCells() const;


	/************************************************
	 * Getters of temporal memory module information. 
	 ***********************************************/

	/**
	 * Get the number of the tm synapse.
	 * 
	 * @return const std::size_t The number of the tm synapses.
	 */
	const std::size_t getNbTmSynapses() const;
	
	/**
	 * Get the number of the tm segments.
	 * 
	 * @return const std::size_t The number of the tm segments.
	 */
	const std::size_t getNbTmSegments() const;
	
	/**
	 * Get the tm anomaly.
	 * 
	 * @return const double The tm anomaly.
	 */
	const htm::Real getTmAnomaly() const;

	/**
	 * Get the created synapses of the temporal memory module on an each step.
	 * 
	 * @return const std::vector<SynapseConnection>& The created synapses.
	 */
	const std::vector<SynapseConnection> getTmCreatedSynapses() const;

	/**
	 * Get the destroyed synapses of the temporal memory module on an each step.
	 * 
	 * @return const std::vector<htm::Synapse>& The destroyed synapses.
	 */
	const std::vector<htm::Synapse>& getTmDestroyedSynapses() const;

	/**
	 * Get the updated synapses of the temporal memory module on an each step.
	 * 
	 * @return const std::vector<SynapsePermanence>& The updated synapses.
	 */
	const std::vector<SynapsePermanence>& getTmUpdatedSynapses() const;

};

using PLayerProxy = std::shared_ptr<LayerProxy>;



/**
 * ProxyFunc implementation in C++.
 *
 * @b Description
 * The ProxyFunc is the maker class for the LayerProxy. This class is the
 * util functions.
 */
struct ProxyFunc {

public:

	template <typename... Args>
	static PLayerProxy make(const Args&... args) {
		return std::make_shared<LayerProxy>(args...);
	}
};

} // namespace cla

#endif // LAYER_PROXY_HPP