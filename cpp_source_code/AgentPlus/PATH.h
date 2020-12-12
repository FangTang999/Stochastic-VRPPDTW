#pragma once
#include "stdafx.h"
#include "CSVParser.h"
#include <string>
#include <iostream>
#include "TIME_TRAVEL.h"
using namespace std;
struct PASSENGER {
	int start, end;//ʱ�䴰
};
struct NODE {
	NODE() { nodetype = 0; }
	int nodeid, nodetype;
	PASSENGER p;
};

struct ITEM {
	ITEM() {
		link_type = 0; travel_time = 0;
	}
	int link_type;
	NODE from_node, to_node;
	int travel_time;
	int tntype() { return to_node.nodetype; }
	bool is_common_node() {
		return to_node.nodetype == 0;
	}
	bool is_passenger() {
		return to_node.nodetype == 1;
	}
	bool is_passenger_reach() {
		return to_node.nodetype == 2;
	}
	bool is_car_start_end() {
		return to_node.nodetype == 3;
	}
	int p_start() {
		return to_node.p.start;
	}
	int p_end() {
		return to_node.p.end;
	}
};
class CPATH
{
public:
	const int UP = 1;
	const int LOWER = 0;
	CPATH();
	~CPATH();
	bool readpath();
	bool readpassenger();
	bool read_link_type();
	bool update_trave_time(vector<ITEM>&path, string tt);
	void out();
	//lower or up(0, 1), day ,path
	map<int, map<int, vector<ITEM> > > m_item;
	void setitem(string& paths, int day, int lower_or_up);
	void clean_files();
	vector<string> read_travel_time_label();
	double base_profile();
private:
	CTIME_TRAVEL m_tt;
	vector<string> m_paths;
	CCSVParser m_parser;
};

double CalculateCostForDays(vector<ITEM>& paths);