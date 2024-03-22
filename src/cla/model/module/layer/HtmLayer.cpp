// HtmLayer.cpp

/**
 * @file
 * Implementation of HtmLayer.cpp
 */

#include <numeric>
#include <sstream>

#include "cla/model/module/layer/HtmLayer.hpp"
#include "cla/utils/Checker.hpp"
#include "cla/utils/VectorHelpers.hpp"

namespace cla {

/************************************************
 * HtmLayer public functions.
 ***********************************************/

HtmLayer::HtmLayer(
	const std::vector<htm::UInt>& inputDimensions,
	const std::vector<htm::UInt>& columnDimensions,
	const htm::UInt nbCellsForColumn,
	const htm::UInt nbRegions,
	PAccepter accepter,
	PAdapter adapter,
	PSender sender,
	PReceiver receiver,
	const std::vector<PSpatialPooler>& sps,
	PTemporalMemory tm
) :
	status_(Status::READY),
	inputDimensions_(inputDimensions),
	columnDimensions_(columnDimensions),
	nbCellsForColumn_(nbCellsForColumn),
	nbRegions_(nbRegions),
	accepter_(std::move(accepter)),
	adapter_(std::move(adapter)),
	sender_(std::move(sender)),
	receiver_(std::move(receiver)),
	sps_(sps),
	tm_(std::move(tm)) {

	// Check the input dimensions.
	CLA_ASSERT(!inputDimensions_.empty());
	CLA_ASSERT(
		std::accumulate(
			inputDimensions_.begin(), inputDimensions_.end(), 1u,
			std::multiplies()
		)
		!= 0u
	);

	// Check the column dimensions.
	CLA_ASSERT(!columnDimensions_.empty());
	CLA_ASSERT(
		std::accumulate(
			columnDimensions_.begin(), columnDimensions_.end(), 1u,
			std::multiplies()
		)
		!= 0u
	);

	// Check the number of the cells and regions.
	CLA_ASSERT(nbCellsForColumn_ >= 0u);
	CLA_ASSERT(nbRegions_ >= 0u);

	cellDimensions_ 
		= std::vector<htm::UInt>(columnDimensions_);
	cellDimensions_.emplace_back(nbCellsForColumn_);

	// Check the instance of the module pointers.
	CLA_ASSERT(accepter_);
	CLA_ASSERT(adapter_);
	CLA_ASSERT(sender_);
	CLA_ASSERT(receiver_);

	CLA_ASSERT(static_cast<htm::UInt>(sps_.size()) == nbRegions_);
	for(std::size_t i = 0u; i < sps_.size(); ++i)
		CLA_ASSERT(sps_.at(i));

	CLA_ASSERT(tm_);

	// Generate the container instance.
	container_.initialize(
		inputDimensions_, 
		columnDimensions_, 
		cellDimensions_
	);

	proxy_ = ProxyFunc::make(container_, sps_, tm_);
}

void HtmLayer::restate() {
	status_ = Status::SLEEP;

	accepter_->restate();
	adapter_->restate();
	sender_->restate();
	receiver_->restate();

	for(std::size_t i = 0u; i < sps_.size(); ++i)
		sps_.at(i)->restate();

	tm_->restate();

	proxy_->setStatus(status_);
}

void HtmLayer::reset() {
	status_ = Status::READY;

	accepter_->reset();
	adapter_->reset();
	sender_->reset();
	receiver_->reset();

	for(std::size_t i = 0u; i < sps_.size(); ++i)
		sps_.at(i)->reset();

	tm_->reset();
	container_.reset();

	proxy_->setStatus(status_);
	proxy_.reset();
}

void HtmLayer::summary(std::ostream& os) const {

	// Get the summary string of the modules for connecting the upper
	// layer.
	std::ostringstream upperModuleOss;
	sender_->summary(upperModuleOss);
	upperModuleOss << " / ";
	receiver_->summary(upperModuleOss);
	const std::string upperModuleStr = upperModuleOss.str();

	// Get the summary string of the temporal memory module.
	std::ostringstream tmModuleOss;
	tm_->summary(tmModuleOss);
	const std::string tmModuleStr = tmModuleOss.str();

	// Get the summary string of the spatial pooler module.
	std::ostringstream spModuleOss;
	sps_.front()->summary(spModuleOss);
	spModuleOss << " * " << sps_.size();
	const std::string spModuleStr = spModuleOss.str();

	// Get the summary string of the modules for connecting the lower
	// layer.
	std::ostringstream lowerModuleOss;
	accepter_->summary(lowerModuleOss);
	lowerModuleOss << " / ";
	adapter_->summary(lowerModuleOss);
	const std::string lowerModuleStr = lowerModuleOss.str();

	// The definition for the summary string.
	const std::size_t lineSize = 57u;
	const std::string layerStr = "HtmLayer";
	const char hlineChar = '=';
	const char dlineChar = '-';
	const char vlineChar = '|';
	const char spaceChar = ' ';
	const std::string hlineStr = std::string(1u, hlineChar);
	const std::string vlineStr = std::string(2u, vlineChar);
	const std::string spaceStr = std::string(1u, spaceChar);

	const std::string subHeader = std::string(2u, hlineChar) + spaceStr + layerStr + spaceStr;
	const std::string header 
		= subHeader + std::string(lineSize - subHeader.size(), hlineChar);
	const std::string footer = std::string(lineSize, hlineChar);
	const std::string delimiter 
		= vlineStr + std::string(lineSize - vlineStr.size() * 2u, dlineChar) + vlineStr;

	const auto& getSpace 
		= [&](const std::string& str) {
			return std::string(lineSize - str.size() - vlineStr.size() * 2u - spaceStr.size(), spaceChar);
		};


	os	<< spaceStr << header << std::endl
		<< spaceStr << vlineStr << spaceStr << upperModuleStr << getSpace(upperModuleStr) << vlineStr << std::endl
		<< spaceStr << delimiter << std::endl
		<< spaceStr << vlineStr << spaceStr << tmModuleStr << getSpace(tmModuleStr) << vlineStr << std::endl
		<< spaceStr << vlineStr << spaceStr << spModuleStr << getSpace(spModuleStr) << vlineStr << std::endl
		<< spaceStr << delimiter << std::endl
		<< spaceStr << vlineStr << spaceStr << lowerModuleStr << getSpace(lowerModuleStr) << vlineStr << std::endl
		<< spaceStr << footer << std::endl;

}

void HtmLayer::detail(std::ostream& os) const {
	os << " == HtmLayer ============================================="
	   << std::endl
	   << std::endl;

	os << "\tinput dimensions\t\t= {" << inputDimensions_ << " }"
	   << std::endl
	   << "\tcolumn dimensions\t\t= {" << columnDimensions_ << " }"
	   << std::endl
	   << "\tnum cells for column\t\t= " << nbCellsForColumn_ << std::endl
	   << "\tnum regions\t\t\t= " << nbRegions_ << std::endl
	   << std::endl
	   << std::endl;


	sender_->detail(os);
	receiver_->detail(os);

	tm_->detail(os);
	sps_.front()->detail(os);

	adapter_->detail(os);
	accepter_->detail(os);

	os << " =========================================================" << std::endl;
}

const bool HtmLayer::forward(
	const htm::SDR& inputSDR,
	const bool learn,
	htm::SDR& activeSDR
) {
	// The case that inputSDR is not accepted.
	if(!accepter_->isAccept(inputSDR)) return false;


	// The case that inputSDR is accepted.
	// update status.
	status_ = Status::RUN;
	container_.increment();

	proxy_->setStatus(status_);
	proxy_->reset();

	// convert an input bits pattern to the active bits pattern.
	adapter_->adapt(inputSDR, container_.activeBits);

	const auto& subActiveBits
		= htm::PSDR(container_.activeBits, nbRegions_).split();
	htm::PSDR activeColumns(columnDimensions_, nbRegions_);
	std::vector<htm::SDR> subActiveColumns = activeColumns.split();

	CLA_ASSERT(sps_.size() == subActiveBits.size());

	// convert an active bits pattern to the active columns pattern.
	for(std::size_t i = 0u; i < sps_.size(); ++i) {
		sps_.at(i)->compute(
			subActiveBits.at(i), learn, subActiveColumns.at(i)
		);
	}
	
	activeColumns.concatenate(subActiveColumns);

	container_.activeColumns = activeColumns;


	// convert an active columns pattern to the active cells pattern.
	tm_->compute(
		container_.activeColumns, learn, container_.activeCells,
		container_.winnerCells
	);

	// select an active pattern send to the upper layer.
	sender_->send(proxy_, activeSDR);

	return true;
}

const PLayerProxy& HtmLayer::backward(const bool learn) {
	// activate segments by internal active cells.
	tm_->activate(learn, container_.activeSegments);

	// Set the sdr container of this layer.
	return getLayerProxy();
}

const PLayerProxy& HtmLayer::backward(
	const PLayerProxy& upperLayer,
	const bool learn
) {

	// Receive and unzip the active and winner sdrs.
	receiver_->receive(
		upperLayer, container_.externalActiveSDR, container_.externalWinnerSDR
	);

	// activate segments by internal active cells.
	tm_->activate(
		container_.externalActiveSDR, container_.externalWinnerSDR,
		learn, container_.activeSegments
	);

	// Set the sdr container of this layer.
	return getLayerProxy();
}


/************************************************
 * HtmLayer public functions (getters).
 ***********************************************/

const PAccepter& HtmLayer::getAccepter() const {
	return accepter_;
}

const PAdapter& HtmLayer::getAdapter() const {
	return adapter_;
}

const std::vector<PSpatialPooler>& HtmLayer::getSPs() const {
	return sps_;
}

const PTemporalMemory& HtmLayer::getTM() const {
	return tm_;
}

const PSender& HtmLayer::getSender() const {
	return sender_;
}

const PReceiver& HtmLayer::getReceiver() const {
	return receiver_;
}

const PLayerProxy& HtmLayer::getLayerProxy() const {
	return proxy_;
}

const Status& HtmLayer::getStatus() const {
	return status_;
}

const std::vector<htm::UInt>& HtmLayer::getInputDimensions() const {
	return inputDimensions_;
}

const std::vector<htm::UInt>& HtmLayer::getColumnDimensions() const {
	return columnDimensions_;
}

const std::vector<htm::UInt>& HtmLayer::getCellDimensions() const {
	return cellDimensions_;
}



} // namespace cla