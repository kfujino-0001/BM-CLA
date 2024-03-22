// Evaluations.hpp

/** 
 * @file
 * Definitions for the Evaluations class in C++
 */

#ifndef EVALUATIONS_HPP
#define EVALUATIONS_HPP

namespace evaluations {

/**
 * Evaluations implementation in C++
 * 
 * @b Description
 * The Evaluations class calculates the evaluation indexes, RMSE
 * and MAE.
 */
class Evaluations {

private:

	double sumSquare_;
	double sumAbsolute_;
	unsigned int nbElem_;

public:

	/**
	 * Evaluations constructor
	 */
	Evaluations();

	/**
	 * Evaluations destructor
	 */
	~Evaluations() = default;

	/**
	 * Initialize Evaluations
	 */
	void initialize();

	/**
	 * Add a value to the error calculation.
	 * 
	 * @param error The difference between predicted value and actual value.
	 */
	template<typename ValueType>
	void add(const ValueType error) {
		const double e = static_cast<double>(error);
		sumSquare_ += error * error;
		sumAbsolute_ += std::abs(error);
		nbElem_++;
	}

	/**
	 * Add avalue to the error calculation.
	 * 
	 * @param predictiveValue Predicted value
	 * @param actualValue actual value
	 */
	template<typename ValueType>
	void add(
		const ValueType predictiveValue,
		const ValueType actualValue
	) {
		add(predictiveValue - actualValue);
	}

	/**
	 * Get Root Mean Square Error (RMSE)
	 * 
	 * @return RMSE
	 */
	double getRMSE()const;

	/**
	 * Get Mean Absolute Error (MAE)
	 * 
	 * @return MAE
	 */
	double getMAE()const;

};


} // namespace evaluations

#endif // EVALUATIONS_HPP