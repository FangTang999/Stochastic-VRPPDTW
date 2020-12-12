#include "stdafx.h"
#include "PATH.h"
#include <sstream>
using namespace std;
const string pathc = "output_agent.csv";
const string path_input_link = "input_link.csv";

CPATH::CPATH()
{
}

CPATH::~CPATH()
{
}

bool CPATH::readpath() {
	if (!m_parser.OpenCSVFile(pathc, true)) {
		return false;
	}
	int day = 1,dayl = 1;
	string path;
	string lr;
	const string ubc = "Upperbound:";
	
	while (m_parser.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
	{
		int *pday = &dayl;
		int lower_or_up = LOWER;
		if (m_parser.GetValueByFieldName("LR_iteration", lr) == false)
			continue;
		if(lr == "LR_iteration")
			continue;
		if (ubc == lr){
			lower_or_up = UP;
			pday = &day;
		}
		if(m_parser.GetValueByFieldName("path_node_seq", path)== false)
			continue;
		setitem(path,(*pday)++, lower_or_up);
		
	}
	m_parser.CloseCSVFile();
	return m_item.size() != 0;
}

bool CPATH::readpassenger()
{
	for (map<int, map<int, vector<ITEM> > >::iterator lowup = m_item.begin();
		lowup != m_item.end(); ++lowup) {
		for (map<int, vector<ITEM> >::iterator it = lowup->second.begin();
			it != lowup->second.end(); ++it) {
			m_tt.read_passenger(it->second);
		}
	}
	return true;
}

bool CPATH::read_link_type() {
	for (map<int, map<int, vector<ITEM> > >::iterator lowup = m_item.begin();
		lowup != m_item.end(); ++lowup) {
		for (map<int, vector<ITEM> >::iterator it = lowup->second.begin();
			it != lowup->second.end(); ++it) {
			m_tt.read_link_type(it->second);
		}
	}

	return true;
}

bool CPATH::update_trave_time(vector<ITEM>& path, string tt)
{
	return m_tt.read_travel_time(path, tt);
}

void CPATH::out()
{
	for (auto v : m_paths) {
		cout << v << endl;
	}
}

void CPATH::setitem(string & paths, int day, int low_or_up)
{
	stringstream s(paths);
	vector<ITEM> items;
	string tmp1;
	int nodeid(0), nodeidpre(0);
	bool igore_first = true;
	while (getline(s, tmp1, ';'))
	{
		ITEM item;
		nodeid = atoi(tmp1.c_str());
		if (igore_first)
		{
			nodeidpre = nodeid;
			igore_first = false;
			continue;
		}
		item.from_node.nodeid = nodeidpre;
		item.to_node.nodeid = nodeid;
		items.push_back(item);
		nodeidpre = nodeid;
	}
	m_item[day][low_or_up] = items;
}

void CPATH::clean_files()
{
	FILE* f = fopen(pathc.c_str(), "w");
	fclose(f);
	f = fopen("output_solution.csv", "w");
	fclose(f);
	f = fopen("output_VehicleMatrix.csv", "w");
	fclose(f);
}

vector<string> CPATH::read_travel_time_label()
{
	if (!m_parser.OpenCSVFile(path_input_link, true)) {
		return vector<string>();
	}
	vector<string> tt;
	const string ttc = "travel_time";
	for (auto v : m_parser.Headers) {
		if (string::npos != v.find(ttc, 0))
			tt.push_back(v);
	}
	m_parser.CloseCSVFile();
	return tt;
}

double CPATH::base_profile() {
	return m_tt.m_passengers_total_base_cost;
}