/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	More detailed descriptions are in BitmarketPrivate.h file.

	Almost every function is constructed in the same way so I've described only one of them.
*/

#include "BitmarketPrivate.h"

BitmarketPrivate::BitmarketPrivate() : BitmarketPrivate(std::string(), std::string())
{ }

BitmarketPrivate::BitmarketPrivate(std::string _public, std::string _private) : key_private(_private), key_public(_public)
{
	m_pythonNet.m_pyFile = "bitmarket_python.py";
	m_pythonNet.m_pyPath = "pythonScript";

	/*
	m_errorCodes[500] = "Invalid HTTP method (other than POST)";
	m_errorCodes[501] = "Invalid public key component";
	m_errorCodes[502] = "Invalid message hash";
	m_errorCodes[503] = "Invalid value of the tonce parameter";
	m_errorCodes[504] = "The key is not authoriced to use this API method";
	m_errorCodes[505] = "Invalid value of the method parameter";
	m_errorCodes[506] = "Too many commands in a given time interval";
	m_errorCodes[507] = "Invalid nonce value (only if an old request is sent again - replay attack)";
	m_errorCodes[508] = "Invalid parameter value for method";
	m_errorCodes[509] = "The account has been banned";
	m_errorCodes[510] = "The account is not name verified";
	m_errorCodes[511] = "API access is globaly temporarily disabled";
	m_errorCodes[400] = "Invalid value of the market parameter";
	m_errorCodes[401] = "Invalid value of the type parameter";
	m_errorCodes[402] = "Invalid value of the amount parameter";
	m_errorCodes[403] = "Invalid value of the rate parameter";
	m_errorCodes[404] = "The operation amount is invalid";
	m_errorCodes[405] = "Insufficient account balance to perform the operation";
	m_errorCodes[406] = "The user has no access to specified market offer";
	m_errorCodes[407] = "Invalid value of the currency parameter";
	m_errorCodes[408] = "Invalid value of the count parameter";
	m_errorCodes[409] = "Invalid value of the start parameter";
	m_errorCodes[410] = "Invalid value of the address parameter.";
	m_errorCodes[411] = "Invalid value of the id parameter.";
	m_errorCodes[412] = "Invalid value of the type parameter.";
	m_errorCodes[413] = "Invalid value of the rateLoss parameter.";
	m_errorCodes[414] = "Invalid value of the rateProfit parameter.";
	m_errorCodes[415] = "Cannot close margin because the position is not fully open";
	m_errorCodes[416] = "Cannot cancel margin because the position is fully open";
	m_errorCodes[417] = "Order cannot be fully satisfied and all or nothing was requested (no longer in use)";
	m_errorCodes[418] = "Operation cannot be performed";
	m_errorCodes[419] = "Recipient has been banned";
	m_errorCodes[420] = "Invalid Fiat/Crypto for tradingdesk";
	m_errorCodes[421] = "Amount is too high";
	m_errorCodes[422] = "Tradingdesk purchase quota exceeded";
	m_errorCodes[423] = "Tradingdesk invalid transaction id";
	m_errorCodes[300] = "Internal application error";
	m_errorCodes[301] = "Withdrawal of funds is blocked temporarily";
	m_errorCodes[302] = "Trading is blocked temporarily";
	m_errorCodes[303] = "Fast fiat withdrawal is unavailable now";
	m_errorCodes[304] = "Withdrawal service is unavailable now";
	*/
}

ptr_json BitmarketPrivate::info()
{
	std::unordered_map<std::string, std::string> arguments;

	return this->command("info", arguments);
}

ptr_json BitmarketPrivate::trade(std::string _market, std::string _type, double _amount, double _rate, bool _allOrNothing)
{
	// declare variable to store arguments
	std::unordered_map<std::string, std::string> arguments;

	// store every argument in variable
	// by using unordered_map we can easily store argument name and value
	arguments["market"]			= _market;
	arguments["type"]			= _type;
	arguments["amount"]			= std::to_string(_amount);
	arguments["rate"]			= std::to_string(_rate);
	arguments["allOrNothing"]	= _allOrNothing ? "1" : "0";

	// execute command and return it's return
	return this->command("trade", arguments);
}

ptr_json BitmarketPrivate::cancel(int _id)
{
	std::unordered_map<std::string, std::string> arguments;

	arguments["id"] = std::to_string(_id);

	return this->command("cancel", arguments);
}

ptr_json BitmarketPrivate::orders(std::string _market)
{
	std::unordered_map<std::string, std::string> arguments;

	arguments["market"] = _market;

	return this->command("orders", arguments);
}

ptr_json BitmarketPrivate::trades(std::string _market, int _count, int _start)
{
	std::unordered_map<std::string, std::string> arguments;

	arguments["market"] = _market;
	arguments["count"] = std::to_string(_count);
	arguments["start"] = std::to_string(_start);

	return this->command("trades", arguments);
}

ptr_json BitmarketPrivate::history(std::string _currency, int _count, int _start)
{
	std::unordered_map<std::string, std::string> arguments;

	arguments["currency"] = _currency;
	arguments["count"] = std::to_string(_count);
	arguments["start"] = std::to_string(_start);

	return this->command("history", arguments);
}

void BitmarketPrivate::pythonPath(std::string _scriptName, std::string _path)
{
	m_pythonNet.m_pyFile = _scriptName;
	m_pythonNet.m_pyPath = _path;
}

ptr_json BitmarketPrivate::command(std::string _method, std::unordered_map<std::string, std::string>& _arguments)
{
	// create data that is constant in every API request
	std::string post = "method=" + _method + "&tonce=" + std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	std::string headers = "API-Key=" + key_public + "&API-Hash=";

	// add arguments specified in _arguments variable
	for (auto& element : _arguments)
	{
		post += "&" + element.first + "=" + element.second;
	}

	// calculate header values
	headers += f_sha512(key_private, post);

	// obtain response
	std::string responseData = m_pythonNet.post("/api2/", post, headers);

	// if there was an error while calling python script then return nullptr
	if (responseData.empty())
		return nullptr;

	// generate return data
	return std::shared_ptr<nlohmann::json>(new nlohmann::json(nlohmann::json::parse(responseData)));
}

std::string BitmarketPrivate::f_sha512(std::string _key, std::string _data)
{
	// declare and define hash variable to store generated hash
	unsigned char *hash = new unsigned char[64];

	// use CHMAC_SHA512 class to calculate hash
	CHMAC_SHA512 hmac((const unsigned char*)_key.c_str(), _key.length());
	hmac.Write((const unsigned char*)_data.c_str(), _data.length());
	hmac.Finalize(hash);

	// create stringstream to transform generated 64-byte hash into HEX-string
	std::stringstream ss;
	ss << std::hex;

	for (int i = 0; i < 64; ++i)
	{
		// add extra '0' char if hexadecimal number is going to be 1-digit long
		if (hash[i] < 0x10)
			ss << '0';

		// cast char type to int so that stringstream does not try to use it's ASCII representation
		ss << static_cast<unsigned short>(hash[i]);
	}

	// free occupied memory and return generated HEX-string
	delete hash;
	return ss.str();
}
