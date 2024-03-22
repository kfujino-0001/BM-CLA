// LayerProxy.cpp

/**
 * @file
 * Implementation of LayerProxy.cpp
 */

#include "cla/model/module/helper/LayerProxy.hpp"

namespace cla {

/************************************************
 * LayerProxy converter callbacks.
 ***********************************************/

const htm::SDR_sparse_t LayerProxy::seg2cells_(const htm::ElemSparse seg
) const {
	return {tm_->cellForSegment(static_cast<htm::Segment>(seg))};
}

const htm::SDR_sparse_t LayerProxy::cell2columns_(
	const htm::ElemSparse cell
) const {
	return {tm_->columnForCell(static_cast<htm::CellIdx>(cell))};
}

const htm::SDR_sparse_t LayerProxy::column2bits_(
	const htm::ElemSparse column
) const {
	const auto& dimensions = container_.activeColumns.dimensions;
	const htm::UInt nbColumnsByRegion
		= htm::SDR(dimensions).size / static_cast<htm::UInt>(sps_.size());

	const htm::UInt regionIdx = column / nbColumnsByRegion;

	return sps_.at(regionIdx)->bitsForColumn(column);
}


/************************************************
 * LayerProxy reducter callbacks.
 ***********************************************/

const htm::NumCells LayerProxy::countCells_(
	const htm::NumCells value,
	const htm::ElemSparse inputIdx,
	const htm::ElemSparse outputIdx
) const {
	return value + static_cast<htm::NumCells>(1);
}

const htm::NumCells LayerProxy::sumCells_(
	const htm::NumCells value,
	const htm::NumCells sum,
	const htm::ElemSparse inputIdx,
	const htm::ElemSparse outputIdx
) const {
	return sum + value;
}


/************************************************
 * LayerProxy private functions.
 ***********************************************/

void LayerProxy::segsToCells_(
	const htm::SDR_sparse_t& segs, 
	htm::SDR& cells
) const {
	convert_(
		segs, cells, 
		[&](const htm::ElemSparse seg) {
			return seg2cells_(seg);
		}
	);
}

void LayerProxy::cellsToColumns_(
	const htm::SDR& cells,
	htm::SDRex<htm::NumCells>& columns
) const {
	convert_(
		cells, columns,
		[&](const htm::ElemSparse cell) {
			return  cell2columns_(cell);
		},
		[&](
			const htm::NumCells value,
			const htm::ElemSparse inputIdx,
			const htm::ElemSparse outputIdx
		) {
			return countCells_(value, inputIdx, outputIdx);
		}
	);
}

void LayerProxy::columnsToBits_(
	const htm::SDRex<htm::NumCells>& columns,
	htm::SDRex<htm::NumCells>& bits
) const {
	convert_(
		columns, bits,
		[&](const htm::ElemSparse column) {
			return column2bits_(column);
		}, 
		[&](
			const htm::NumCells value,
			const htm::NumCells sum,
			const htm::ElemSparse inputIdx,
			const htm::ElemSparse outputIdx
		){
			return sumCells_(value, sum, inputIdx, outputIdx);
		}
	);
}

void LayerProxy::updateBurstColumns_(
	const htm::SDR& prePredictiveColumns,
	const htm::SDR& activeColumns,
	htm::SDR& burstColumns
) const {
	const auto& activeSparse = activeColumns.getSparse();
	const auto& predictiveSparse = prePredictiveColumns.getSparse();

	htm::SDR_sparse_t sparse;

	std::set_difference(
		activeSparse.begin(), activeSparse.end(), predictiveSparse.begin(),
		predictiveSparse.end(), std::inserter(sparse, sparse.end())
	);

	std::sort(sparse.begin(), sparse.end());
	burstColumns.setSparse(sparse);
}



/************************************************
 * LayerProxy public functions.
 ***********************************************/

LayerProxy::LayerProxy(
	const SDRContainer& container,
	const std::vector<PSpatialPooler>& sps,
	const PTemporalMemory& tm
) :
	status_(Status::READY),
	container_(container),
	sps_(sps),
	tm_(tm)
{

	predictiveCells_.initialize(container_.activeCells.dimensions);
	predictiveColumns_.initialize(container_.activeColumns.dimensions);
	predictiveBits_.initialize(container_.activeBits.dimensions);
	burstColumns_.initialize(container_.activeColumns.dimensions);

	reset();
}

void LayerProxy::reset() {
	predictiveCells_.zero();
	predictiveColumns_.zero();
	predictiveBits_.zero();
	burstColumns_.zero();

	predictiveCellsValid_ = false;
	predictiveColumnsValid_ = false;
	predictiveBitsValid_ = false;
	burstColumnsValid_ = false;
}

void LayerProxy::setStatus(const Status status) {
	status_ = status;
}

const htm::SDR& LayerProxy::getBurstColumns() const {
	if(!burstColumnsValid_) {
		htm::SDR prePredictiveCells(container_.activeCells.dimensions);
		htm::SDRex<htm::NumCells> prePredictiveColumns(
			container_.activeColumns.dimensions
		);

		segsToCells_(container_.preActiveSegments, prePredictiveCells);
		cellsToColumns_(prePredictiveCells, prePredictiveColumns);

		updateBurstColumns_(
			prePredictiveColumns, getActiveColumns(), burstColumns_
		);
		burstColumnsValid_ = true;
	}

	return burstColumns_;
}

const htm::SDR& LayerProxy::getPredictiveCells() const {
	if(!predictiveCellsValid_) {
		segsToCells_(container_.activeSegments, predictiveCells_);
		predictiveBitsValid_ = true;
	}

	return predictiveCells_;
}

const htm::SDR& LayerProxy::getPredictiveColumns() const {
	return getPredictiveColumnsWithNbPCells();
}

const htm::SDRex<htm::NumCells>& LayerProxy::getPredictiveColumnsWithNbPCells() const {
	if(!predictiveColumnsValid_) {
		cellsToColumns_(getPredictiveCells(), predictiveColumns_);
		predictiveColumnsValid_ = true;
	}

	return predictiveColumns_;
}

const htm::SDR& LayerProxy::getPredictiveBits() const {
	return getPredictiveBitsWithNbPCells();
}

const htm::SDRex<htm::NumCells>& LayerProxy::getPredictiveBitsWithNbPCells() const {
	if(!predictiveBitsValid_) {
		columnsToBits_(
			getPredictiveColumnsWithNbPCells(), predictiveBits_
		);
		predictiveBitsValid_ = true;
	}

	return predictiveBits_;
}


/************************************************
 * TM module of the layer proxy functions. 
 ***********************************************/

const std::size_t LayerProxy::getNbTmSynapses() const {
	return tm_->getConnections().numSynapses();
}

const std::size_t LayerProxy::getNbTmSegments() const {
	return tm_->getConnections().numSegments();
}

const htm::Real LayerProxy::getTmAnomaly() const {
	return tm_->getAnomaly();
}

const std::vector<SynapseConnection> LayerProxy::getTmCreatedSynapses() const {
	const auto& synapses = tm_->getConnectionHandler().getCreatedSynapses();
	const auto& connections = tm_->getConnections();

	std::vector<SynapseConnection> createdSynapse(synapses.size());
	for(std::size_t i = 0u, size = synapses.size(); i < size; ++i) {
		const htm::ElemSparse synapse = synapses.at(i);
		const htm::SynapseData& synData
			= connections.dataForSynapse(synapse);

		createdSynapse.at(i) = {
			synapse, 
			connections.cellForSegment(synData.segment),
			synData.presynapticCell
		};
	}

	return createdSynapse;
}

const std::vector<htm::Synapse>& LayerProxy::getTmDestroyedSynapses() const {
	return tm_->getConnectionHandler().getDestroyedSynapses();
}

const std::vector<SynapsePermanence>& LayerProxy::getTmUpdatedSynapses() const {
	return tm_->getConnectionHandler().getUpdatePermanences();
}


/************************************************
 * LayerProxy getter functions.
 ***********************************************/

const Status& LayerProxy::getStatus() const {
	return status_;
}

const htm::SDR& LayerProxy::getActiveBits() const {
	return container_.activeBits;
}

const htm::SDR& LayerProxy::getActiveColumns() const {
	return container_.activeColumns;
}

const htm::SDR& LayerProxy::getActiveCells() const {
	return container_.activeCells;
}

const htm::SDR& LayerProxy::getWinnerCells() const {
	return container_.winnerCells;
}

const htm::SDR& LayerProxy::getExternalActives() const {
	return container_.externalActiveSDR;
}

const htm::SDR& LayerProxy::getExternalWinners() const {
	return container_.externalWinnerSDR;
}

const htm::SDR_sparse_t& LayerProxy::getActiveSegments() const {
	return container_.activeSegments;
}

} // namespace cla