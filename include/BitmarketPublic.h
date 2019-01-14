/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	BitmarketPublic class defines methods capable of obtaining data from
	public part of Bitmarket API. Functions descriptions are a copy-pasted
	part of Bitmarket API's description. If you need more detailed
	information visit <https://www.bitmarket.pl/docs.php?file=api_public.html>.
	Or <https://www.bitmarket.net/docs.php?file=api_public.html> for English.
*/

#ifndef BITMARKETPUBLIC_H
#define BITMARKETPUBLIC_H

#include <memory>	// shared_ptr

// Defines structures that are used to store public API's data
#include "PublicApiDataStructures.h"

// Class for handling external python script execution
#include "PythonNet.h"

// Nlohmann's json library https://github.com/nlohmann/json
#include "nlohmann/json.hpp"

class BitmarketPublic
{
public:
	/**
		Default and only constructor
	*/
	BitmarketPublic();

	/**
		Parses API's ticker.json file content into s_ticker struct

		@return smart pointer to an appropriate data structure
	*/
	std::shared_ptr<s_ticker>		ticker(std::string _market = "BTCPLN");

	/**
		Parses API's orderbook.json file content into s_orderBook struct

		@return smart pointer to an appropriate data structure
	*/
	std::shared_ptr<s_orderBook>	orderbook(std::string _market = "BTCPLN");

	/**
		Parses API's trades.json file content into s_trades struct

		@param _since when set then request will download 500 trades that follow this transaction id; if not then request downloads trades from last hour - container size may vary!
		@return smart pointer to an appropriate data structure
	*/
	std::shared_ptr<s_trades>		trades(int _since = -1, std::string _market = "BTCPLN");

	/**
		Parses json file that contains 90 data points from given interval and market into s_graph struct

		@param _market string choosen out of { BTCPLN, BTCEUR, LTCPLN, LTCBTC, LiteMineXBTC }
		@param _interval string choosen out of { 90m, 6h, 1d, 7d, 1m, 3m, 6m, 1y }
		@return smart pointer to an appropriate data structure
	*/
	std::shared_ptr<s_graph>		graphs(std::string interval, std::string _market = "BTCPLN");

	/**
		Parses API's ctransfer.json file content into s_transfer struct

		@param _tx unique transaction id of the transfer operation
		@param _from login name of the sender
		@param _to login name of the receiver
		@return smart pointer to an appropriate data structure
	*/
	std::shared_ptr<s_transfer>		ctransfer(std::string _tx, std::string _from, std::string _to);

private:
	PythonNet m_pythonNet;
};

#endif