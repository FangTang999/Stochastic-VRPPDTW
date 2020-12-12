#pragma once
#include "CSVParser.h"
struct ITEM;
class CTIME_TRAVEL
{
public:
	CTIME_TRAVEL();
	~CTIME_TRAVEL();
	bool read_travel_time(vector<ITEM>& item, string tt);
	bool read_link_type(vector<ITEM>& item);
	bool read_passenger(vector<ITEM>& item);
	pair<int, int> passenger_nodes(vector<ITEM>& item, int from_node_id, int to_node_id);
	CCSVParser m_parser;
	double m_passengers_total_base_cost;
};

