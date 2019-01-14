/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	BitmarketPrivate class defines methods capable of obtaining data from 
	private part of Bitmarket API. Functions descriptions are a copy-pasted
	part of Bitmarket API's description. If you need more detailed
	information visit <https://github.com/bitmarket-net/api>.
*/

#ifndef BITMARKETPRIVATE_H
#define BITMARKETPRIVATE_H

#include <sstream>			// string, stringstream
#include <chrono>			// to_time_t, now
#include <unordered_map>	// unordered_map
#include <memory>			// shared_ptr

// Class for handling external python script execution
#include "PythonNet.h"

// Modified methods to generate HMAC SHA512 hash
#include "crypto/hmac_sha512.h"

// Single file library to handle JSON data format
#include "nlohmann/json.hpp"

typedef std::shared_ptr<nlohmann::json> ptr_json;

class BitmarketPrivate
{
public:
	BitmarketPrivate();
	BitmarketPrivate(std::string _public, std::string _private);

	/**
		Obtains account information

		@return	Nlohmann's JSON data structure representing API's response:

			balances - object describing the funds available on the user account, with the following properties:
				available - object describing free funds available in each currency.
				blocked - object describing funds in active trade offers, in each currency.
			account - object describing the state of the user account, with the following properties:
				turnover - account turnover value.
				commissionMaker - commission value as market maker.
				commissionTaker - commission value as market taker.
			bank_deposit_fiat - object describing information to deposit fiat currency, one for each. Currently only PLN and EUR:
				PLN
					bank_name - name of the bank where the deposit is to be made.
					pay_to - name to whom deposit is to be made.
					acc_num - the account number where deposit is to be made.
					transfer_title - text to identify the deposit by the user.
				EUR - same as PLN with one additional data
					swift_code - the switch code to be used to transfer EURO

	*/
	ptr_json info();

	/**
		Submits an order

		@param _market - market where the trade must be made (for example, "BTCEUR")
		@param _type - order type: buy ("bid" or "buy") or sell ("ask" or "sell")
		@param _amount - order amount (in cryptocurrency)
		@param _rate - exchange rate 
		@param _allOrNothing - flag to specify whether the order should be fulfilled completely or not.
								- "1" - The order must be fulfilled completely and the entire amount 
										should be traded. If it is not possible, it results in error.
								- "0" - The order can be fulfilled partially. Rest of the amount will 
										be kept in orderbook so that it can be traded later. This is 
										the default behaviour if the parameter is not send.

		@return	Nlohmann's JSON data structure representing API's response:

			id - market order identifier.
			order - object describing the newly made order:
				id - market order identifier.
				market - market where the order has been made.
				amount - cryptocurrency amount.
				rate - exchange rate.
				fiat - fiat amount after exchange.
				type - order type ("buy" or "sell").
				time - order creation time.
			balances - account balances after the operation (identical to those returned by the info command).

	*/
	ptr_json trade(std::string _market, std::string _type, double _amount, double _rate, bool _allOrNothing);

	/**
		Calcels na order request

		@param _id - order identifier

		@return Nlohmann's JSON data structure representing API's response:

			balances - account balances after canceling the order.
	*/
	ptr_json cancel(int _id);

	/**
		Obtains list of user orders

		@param _market - the market from which the orderbook must be returned

		@return Nlohmann's JSON data structure representing API's response:

			buy - list of buy orders (in the format identical to that returned from the order method).
			sell - list of sell orders.
	*/
	ptr_json orders(std::string _market);

	/**
		Obtains list of user trades

		@param _market - market where the trades took place, for example "BTCEUR"
		@param _count - number of list elements, possible values: from 1 to 1000 (1000 is the default)
		@param _start - number of the first element, zero based (0 is the default)

		@return Nlohmann's JSON data structure representing API's response:

			total - total number of elements.
			start - number of the first list element.
			count - number of returned list elements.
			results - the list of trades, each object has the following parameters:
				id - trade identifier.
				type - trade type ("buy" or "sell").
				amountCrypto - cryptocurrency amount.
				currencyCrypto - crptocurrency code (for example "BTC").
				amountFiat - fiat amount.
				currencyFiat - fiat currency code (for example "EUR").
				rate - exchange rate.
				time - trade time.
	*/
	ptr_json trades(std::string _market, int _count, int _start);

	/**
		Obtains history of account operations

		@param _currency - currency of the operations to be fetched
		@param _count - number of list elements, possible values: from 1 to 1000 (1000 is the default)
		@param _start - number of the first element, zero based (0 is the default)

		@return Nlohmann's JSON data structure representing API's response:

			total - total number of elements.
			start - number of the first list element.
			count - number of returned list elements.
			results - the list of history entries, each object has the following parameters:
				id - operation identifier.
				amount - operation amount.
				currency - operation currency.
				rate - exchange rate (if the operation describes a trade on the market).
				commission - operation commission.
				time - operation time.
				type - operation type, such as:
					"deposit" - deposit of funds.
					"withdraw" - withdrawal of funds.
					"withdrawCancel" - withdrawal cancellation.
					"order" - order submission.
					"trade" - market trade.
					"cancel" - order cancellation.
	*/
	ptr_json history(std::string _currency, int _count, int _start);
	
	// TODO: other API methods

	/**
		Changes python file localization

		@param _scriptName - name of python script file
		@param _path - directory where the script file is located
	*/
	void pythonPath(std::string _scriptName, std::string _path);

	/**
		User's public API key generated on bitmarket website
	*/
	std::string key_public;

	/**
		User's private API key generated on bitmarket website
	*/
	std::string key_private;

	/**
		Sends custom request to Bitmarket API

		@param _method - method name
		@param _arguments - arguments included in this request

		@return  Nlohmann's JSON data structure representing API's response
	*/
	ptr_json command(std::string _method, std::unordered_map<std::string, std::string>& _arguments);

private:
	/**
		This function generates HMAC SHA512 of given data using given key.

		@param _key is a private key from bitmarket API
		@param _data is a data we want to send
		@return HMAC SHA512 of given input	
	*/
	std::string f_sha512(std::string _key, std::string _data);

	PythonNet m_pythonNet;

	// std::unordered_map<int, std::string> m_errorCodes;
};

#endif
