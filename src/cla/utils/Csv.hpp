// Csv.hpp

/** 
 * @file
 * Definitions for the Csv class in C++
 */

#ifndef CSV_HPP
#define CSV_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <filesystem>
#include <typeinfo>
#include <initializer_list>
#include <algorithm>


#define CSV_CHECK(condition, msg) \
	if(!(condition)){\
		std::cerr << __FILE__ << ":" << __LINE__ << ": runtime error: " << msg << std::endl;\
		std::abort();}



namespace csv {

namespace fs = std::filesystem;
using Header = std::vector<std::string>;


namespace {

	/************************************************
	 * Csv helper prototype definitions.
	 ***********************************************/

	template<class Type> 
	const bool isVariable();
	
	template<class... Args> 
	void printArgs_(std::ostream& os, Args... args);

	template<class DataType>
	std::ostream& printLine_(std::ostream& os, const std::vector<DataType>& line);

	template<class... Args>
	std::ostream& printData_(std::ostream& os, const Args&... data);

	template<class DataType>
	std::ostream& printData_(std::ostream& os, const std::vector<DataType>& data);

	template<class DataType>
	std::ostream& operator<<(std::ostream& os, const std::vector<DataType>& data);

	template<class... Args>
	std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& data);


	std::istringstream splitLine_(const std::string& line, const char delim);

	std::vector<std::string> toStringVec_(std::istringstream& iss);


	/************************************************
	 * Csv write helper functions.
	 ***********************************************/

	template<class Type>
	const bool isVariable(){
		return 
			typeid(Type) == typeid(int) ||
			typeid(Type) == typeid(long) ||
			typeid(Type) == typeid(long long) ||
			typeid(Type) == typeid(unsigned) ||
			typeid(Type) == typeid(unsigned long) ||
			typeid(Type) == typeid(unsigned long long) ||
			typeid(Type) == typeid(float) ||
			typeid(Type) == typeid(double) ||
			typeid(Type) == typeid(long double) ||
			typeid(Type) == typeid(char) ||
			typeid(Type) == typeid(std::string);
	}

	template<class... Args>
	void printArgs_(std::ostream& os, Args... args) {
		std::ostringstream ss;
		using swallow = std::initializer_list<int>;

		const auto& add = [&](const auto& elem){ss << elem << ",";};
		(void)swallow{ (void( add(args) ), 0)... };

		std::string line = ss.str();
		line.erase(line.end() - 1);
		os << line;
	}

	template<class DataType>
	std::ostream& printLine_(std::ostream& os, const std::vector<DataType>& line) {
		if(line.empty()) return os;

		for(std::size_t i = 0u, size = line.size() - 1u; i < size; ++i)
			os << line.at(i) << ",";

		os << line.back();
		return os;
	}

	template<class... Args>
	std::ostream& printData_(std::ostream& os, const Args&... data) {
		printArgs_(os, data...);
		return os;
	}

	template<class DataType>
	std::ostream& printData_(std::ostream& os, const std::vector<DataType>& data) {
		if(data.empty()) return os;

		for(std::size_t i = 0u, size = data.size() - 1u; i < size; ++i)
			os << data.at(i) << std::endl;

		os << data.back();
		return os;
	}

	template<class DataType>
	std::ostream& operator<<(std::ostream& os, const std::vector<DataType>& data) {
		if(isVariable<DataType>()){
			return printLine_(os, data);
		} else {
			return printData_(os, data);
		}
	}

	template<class... Args>
	std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& data) {
		std::apply([&](auto... args){printArgs_(os, args...);}, data);
		return os;
	}



	/************************************************
	 * Csv read helper functions.
	 ***********************************************/

	std::istringstream splitLine_(const std::string& line, const char delim) {
		std::string str(line);
		std::replace(str.begin(), str.end(), delim, ' ');
		std::istringstream iss(str);

		return iss;
	}

	std::vector<std::string> toStringVec_(std::istringstream& iss) {
		std::vector<std::string> vec;
		std::string str;

		while(std::getline(iss, str)) {
			vec.emplace_back(str);
		}

		return vec;
	}

} // namespace for inner linkage



/**
 * Csv implementation in C++.
 * 
 * @b Description
 * The csv class is a read-write class of csv files. In this class,
 * io processing is performed when read or write functions are called.
 * The user can read from and write to a file at any time.
 * 
 * This class does not strongly restrict data type. Basically, any
 * data structure with operator<< and operator>> can be used.
 */
class Csv {

private:

	std::string filename_;
	bool isOverwrite_;

public:

	/**
	 * Csv constructor.
	 */
	Csv() = default;

	/**
	 * Csv constructor.
	 * 
	 * @param filename The file name of the handled csv file.
	 * @param isOverwrite The boolean value whether overwriting the csv
	 * file when write function is called.
	 */
	Csv(
		const std::string& filename,
		const bool isOverwrite = false
	) {
		initialize(filename, isOverwrite);
	}

	/**
	 * Csv destructor.
	 */
	~Csv() = default;

	/**
	 * Initialize csv.
	 * 
	 * @param filename The file name of the handled csv file.
	 * @param isOverwrite The boolean value whether overwriting the csv
	 * file when write function is called.
	 */
	void initialize(
		const std::string& filename,
		const bool isOverwrite = false
	) {
		filename_ = filename;
		isOverwrite_ = isOverwrite;

		CSV_CHECK(
			filename_.find(".csv") != std::string::npos,
			"The class does not support files other than csv."
		)
	}

	/**
	 * Write data to the csv file.
	 * 
	 * @param mode The save mode of csv file.
	 * @param data The saved data. Not only vector, but also tuples and
	 * classes that inherit from operator<< are supported.
	 */
	template<class Data>
	void write(
		std::ios_base::openmode mode,
		const Data& data
	) {
		CSV_CHECK(
			(!fs::exists(filename_) || isOverwrite_),
			"The file already exists. Rename the file or allow overwriting."
		)

		std::ofstream ofs(filename_, mode);
		CSV_CHECK(!ofs.fail(), "The file cannot be opened.");
		
		ofs << data << std::endl;

		ofs.close();
	}

	/**
	 * Write data to the csv file.
	 * 
	 * @param mode The save mode of csv file.
	 * @param args The parameter pack of saved variables.
	 */
	template<class... Args>
	void write(
		std::ios_base::openmode mode,
		const Args&... args
	) {
		CSV_CHECK(
			(!fs::exists(filename_) || isOverwrite_),
			"The file already exists. Rename the file or allow overwriting."
		)

		std::ofstream ofs(filename_, mode);
		CSV_CHECK(!ofs.fail(), "The file cannot be opened.");

		printArgs_(ofs, args...);
		ofs << std::endl;

		ofs.close();
	}

	/**
	 * 
	 */
	template<class... Args>
	const Header read(
		std::vector<std::tuple<Args...>>& data,
		const bool isExistHeader = true,
		const std::size_t maxSize = 0u
	) {
		CSV_CHECK(
			(fs::exists(filename_)),
			"The file does not exist. Make sure the file name is correct."
		)

		std::ifstream ifs(filename_);
		std::string line;
		Header header;

		CSV_CHECK(!ifs.fail(), "The file cannot be opened.")

		// initialize data.
		data.clear();
		if(maxSize != 0u) data.reserve(maxSize);

		for(std::size_t i = 0u; std::getline(ifs, line); ++i) {
			if(line != "\n") {
				std::istringstream iss = splitLine_(line, ',');

				// get header
				if(i == 0u && isExistHeader) {
					header = toStringVec_(iss);
					continue;
				}
				
				// TODO build converting process from iss to tuple
			}

			if(data.size() == maxSize) break;
		}

		ifs.close();
		return header;
	}

	/**
	 * Read the csv file with data structures.
	 * 
	 * @param data The data vector. This function inserts data 
	 * from a csv file into the data vector and returns it.
	 * This data vector must have operator>> override.
	 * @param isExistHeader The boolean value whether to consider
	 * a header files.
	 * @param maxSize The max size of reading data. This function
	 * reads the data in the upper maxSize rows of the csv file.
	 * If maxSize = 0, this function reads all data in the csv file.
	 */
	template<class DataType>
	const Header read(
		std::vector<DataType>& data,
		const bool isExistHeader = true,
		const std::size_t maxSize = 0u
	) {
		CSV_CHECK(
			(fs::exists(filename_)),
			"The file does not exist. Make sure the file name is correct."
		)

		std::ifstream ifs(filename_);
		std::string line;
		Header header;

		CSV_CHECK(!ifs.fail(), "The file cannot be opened.")

		// initialize data.
		data.clear();
		if(maxSize != 0u) data.reserve(maxSize);

		for(std::size_t i = 0u; std::getline(ifs, line); ++i) {
			if(line != "\n") {
				std::istringstream iss = splitLine_(line, ',');

				// get header
				if(i == 0u && isExistHeader) {
					header = toStringVec_(iss);
					continue;
				}
				
				DataType dataLine;
				iss >> dataLine;

				// check if all data from istringstream has been converted
				// to DataType variables.
				CSV_CHECK(iss.eof(), "Not all row data is read.")

				data.emplace_back(dataLine);
			}

			if(data.size() == maxSize) break;
		}

		ifs.close();
		return header;
	}

};

} // namespace csv

#endif // CSV_HPP