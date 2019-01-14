/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	More detailed descriptions are in BitmarketPublic.h file.

	Almost every function is constructed in the same way so I've described only one of them.
*/

#include "BitmarketPublic.h"

BitmarketPublic::BitmarketPublic()
{ 
	m_pythonNet.m_pyFile = "bitmarket_python.py";
	m_pythonNet.m_pyPath = "pythonScript";
}

std::shared_ptr<s_ticker> BitmarketPublic::ticker(std::string _market)
{
	// obtain appropriate data from Bitmarket API
	std::string _data = m_pythonNet.get("/json/" + _market + "/ticker.json");

	// check if PythonNet::get didn't encounter any error
	if(_data.empty())
		return nullptr;

	// TODO: implement better exceptions handling for this project
	try 
	{
		// parse obtained data into nlohmann json structure
		auto _jsonDocument = nlohmann::json::parse(_data);

		// declare pointer to a desired data structure
		std::shared_ptr<s_ticker> _retValue(new s_ticker);

		// fill data structure with appropriate data
		_retValue->ask		= _jsonDocument["ask"].get<double>();
		_retValue->bid		= _jsonDocument["bid"].get<double>();
		_retValue->last		= _jsonDocument["last"].get<double>();
		_retValue->low		= _jsonDocument["low"].get<double>();
		_retValue->high		= _jsonDocument["high"].get<double>();
		_retValue->vwap		= _jsonDocument["vwap"].get<double>();
		_retValue->volume	= _jsonDocument["volume"].get<double>();

		// return smart pointer
		return _retValue;
	}
	catch (...)
	{
		return nullptr;
	}
}

std::shared_ptr<s_orderBook> BitmarketPublic::orderbook(std::string _market)
{
	// obtain appropriate data from Bitmarket API
	std::string _data = m_pythonNet.get("/json/" + _market + "/orderbook.json");

	// check if PythonNet::get didn't encounter any error
	if (_data.empty())
		return nullptr;

	// TODO: implement better exceptions handling for this project
	try
	{
		// parse obtained data into nlohmann json structure
		auto _jsonDocument = nlohmann::json::parse(_data);

		// declare pointer to a desired data structure
		std::shared_ptr<s_orderBook> _retValue(new s_orderBook);

		// iterate through every element in 'asks' table
		for (auto _element : _jsonDocument["asks"])
		{
			_retValue->asks.push_back({ 
				_element[0].get<double>(), 
				_element[1].get<double>() 
			});
		}

		// iterate through every element in 'bids' table
		for (auto _element : _jsonDocument["bids"])
		{
			_retValue->bids.push_back({ 
				_element[0].get<double>(), 
				_element[1].get<double>() 
			});
		}

		// return smart pointer
		return _retValue;
	}
	catch (...)
	{
		return nullptr;
	}
}

std::shared_ptr<s_trades> BitmarketPublic::trades(int _since, std::string _market)
{
	// obtain appropriate data from Bitmarket API
	std::string _data = m_pythonNet.get("/json/" + _market + "/trades.json" + (_since < 0 ? "" : "?since=" + std::to_string(_since)));

	// check if PythonNet::get didn't encounter any error
	if (_data.empty())
		return nullptr;

	// TODO: implement better exceptions handling for this project
	try
	{
		// parse obtained data into nlohmann json structure
		auto _jsonDocument = nlohmann::json::parse(_data);

		// declare pointer to a desired data structure
		std::shared_ptr<s_trades> _retValue(new s_trades);

		// iterate through every element
		for (auto _element : _jsonDocument)
		{
			_retValue->trades.push_back({
				_element["amount"].get<double>(),
				_element["price"].get<double>(),
				_element["date"].get<long>(),
				_element["tid"].get<long>(),
				_element["type"].get<std::string>()
			});
		}

		// return smart pointer
		return _retValue;
	}
	catch (...)
	{
		return nullptr;
	}
}

std::shared_ptr<s_graph> BitmarketPublic::graphs(std::string _interval, std::string _market)
{
	// obtain appropriate data from Bitmarket API
	std::string _data = m_pythonNet.get("/graphs/" + _market + "/" + _interval + ".json");

	// check if PythonNet::get didn't encounter any error
	if (_data.empty())
		return nullptr;

	// TODO: implement better exceptions handling for this project
	try
	{
		// parse obtained data into nlohmann json structure
		auto _jsonDocument = nlohmann::json::parse(_data);

		// declare pointer to a desired data structure
		std::shared_ptr<s_graph> _retValue(new s_graph);

		// set market and interval information
		_retValue->m_interval = _interval;
		_retValue->m_market = _market;

		// iterate through every element
		for (auto _element : _jsonDocument)
		{
			_retValue->points.push_back({
				_element["time"].get<long>(),
				std::stod(_element["open"].get<std::string>()),
				std::stod(_element["high"].get<std::string>()),
				std::stod(_element["low"].get<std::string>()),
				std::stod(_element["close"].get<std::string>()),
				std::stod(_element["vol"].get<std::string>())
			});
		}

		// return smart pointer
		return _retValue;
	}
	catch (...)
	{
		return nullptr;
	}
}

std::shared_ptr<s_transfer> BitmarketPublic::ctransfer(std::string _tx, std::string _from, std::string _to)
{	
	// obtain appropriate data from Bitmarket API
	std::string _data = m_pythonNet.get("/json/ctransfer.json?tx=" + _tx + "&from=" + _from + "&to=" + _to);

	// check if PythonNet::get didn't encounter any error
	if (_data.empty())
		return nullptr;

	// TODO: implement better exceptions handling for this project
	try
	{
		// parse obtained data into nlohmann json structure
		auto _jsonDocument = nlohmann::json::parse(_data);

		// declare pointer to a desired data structure
		std::shared_ptr<s_transfer> _retValue(new s_transfer);

		// iterate through every element
		
		// TODO: implement this function!

		// return smart pointer
		return _retValue;
	}
	catch (...)
	{
		return nullptr;
	}
}



