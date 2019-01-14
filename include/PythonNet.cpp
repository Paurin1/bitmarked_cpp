/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	PythonNet class provides methods to execute and obtain data from external 
	python script. 

	Before compiling - check "python_path" const variable in PythonNet.cpp
*/

#ifndef PYTHONNET_H
#define PYTHONNET_H

#include <string>		// string
#include <fstream>		// ifstream, istreambuf_iterator

class PythonNet
{
public:
	/*
		default empty constructor in case if needed

		recommended constructor is PythonNet(str, str)
	*/
	PythonNet();

	/*
		recommended constructor

		@param _pyPath is the path to directory containing correctly prepared bitmarket communication python script
		@param _pyFile is the name of previously mentioned script
	*/
	PythonNet(std::string _pyPath, std::string _pyFile);

	/*
		Uses python script to complete a GET request on a public part of bitmarket.net api

		@param _url specifies which API element should be called, i.e. _url = "/json/BTCPLN/ticker.json"
		@return if everything is correct the return is a std::string object containing JSON formatted API response; returns empty std::string on error
	*/
	std::string get(std::string _url);

	/*
		Uses python script to complete a POST request on a private part of bitmarket.net api

		@param _url specifies which API element should be called, i.e. _url = "/api2"
		@param _params is a std::string containing url-encoded parameters required by private API
		@param _headers is a std::string containing url-encoded headers required by private API
		@return if everything is correct the return is a std::string object containing JSON formatted API response; returns empty std::string on error
	*/
	std::string post(std::string _url, std::string _params, std::string _headers);

	/*
		stores path to directory containing correctly prepared bitmarket communication python script
	*/
	std::string m_pyPath;

	/*
		stores name of a correctly prepared bitmarket communication python script
	*/
	std::string m_pyFile;
};

#endif

