/*
	Copyright (c) 2019 Maciej Goncerz <https://github.com/Paurin1>.
	Licensed under the MIT License <http://opensource.org/licenses/MIT>.

	PublicApiDataStructures.h file contains definitions of data structures that are
	used to represent output of BitmarketPublic class functions.

	Structures self-define themselves. Variable names correspond to those used by Bitmarket API.
*/

#ifndef PUBLICAPIDATASTRUCTURES_H
#define PUBLICAPIDATASTRUCTURES_H

#include <vector>

struct s_ticker
{
	double ask;
	double bid;
	double last;
	double low;
	double high;
	double vwap;
	double volume;
};

struct s_order
{
	double exchangeRate;
	double amount;
};

struct s_orderBook
{
	std::vector<s_order> asks;
	std::vector<s_order> bids;
};

struct s_trade
{
	double amount;
	double price;
	long date;			// seconds since Unix epoch
	long tid;
	std::string type;
};

struct s_trades
{
	std::vector<s_trade> trades;
};

struct s_graphPoint
{
	long time;
	double open;
	double high;
	double low;
	double close;
	double vol;
};

struct s_graph
{
	std::vector<s_graphPoint> points;
	std::string m_market;
	std::string m_interval;
};

struct s_transfer
{
	char success;
	std::string currency;
	std::string formatted_time;
	double amount;
	long time;
};

#endif