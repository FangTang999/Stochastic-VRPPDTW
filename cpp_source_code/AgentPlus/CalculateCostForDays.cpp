//#include "stdafx.h"
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include "PATH.h"
using namespace std;

//乘客起点发现需要虚拟车
int virtual_car_pstart(vector<ITEM>::size_type pos, vector<ITEM>& paths) { 
	int cost_d = 0;
	int end = paths[pos].to_node.nodeid+1;
	int link_type = paths[pos].link_type;
	for (pos = pos+1;pos < paths.size(); ++pos)
	{
		auto &v = paths[pos];
		if (v.link_type != 0)
		{
			if (v.link_type != link_type && v.link_type != -1 * link_type)
			{
				continue;
			}
		}
		cost_d += 2*v.travel_time;
		if (v.to_node.nodeid == end)
		{
			break;
		}
	}
	return cost_d; 
}
//乘客终点时发现需要虚拟车
int virtual_car_pend(vector<ITEM>::size_type pos, vector<ITEM>& paths, vector<ITEM>::size_type& posstart) {
	int start = paths[pos].to_node.nodeid - 1;
	int cost_d = 0;
	for (vector<ITEM>::size_type i = 0; i < pos; ++i)
	{
		if (paths[i].to_node.nodeid != start)
			continue;
		posstart = i + 1;
		cost_d = 2*virtual_car_pstart(posstart,paths);
		break;
	}
	return cost_d;
}
//void revert(int& cost, int& time, vector<ITEM> paths, vector<ITEM>::size_type pos) {
//	if (v.is_passenger()/*乘客*/) {
//		if (to_node_current_time_interval < v.p_start()) {// 车要等待
//			Cost_d += 0.5 * (v.p_start() - to_node_current_time_interval);
//			to_node_current_time_interval = v.p_start();
//		}
//		else if (v.p_start() <= to_node_current_time_interval &&
//			to_node_current_time_interval <= v.p_end()) {//人等车
//			Cost_d += 0.3 * (to_node_current_time_interval - v.p_start());
//		}
//		else if (to_node_current_time_interval > v.p_end()) {//超过时间窗
//			Cost_d -= v.travel_time;
//			to_node_current_time_interval -= v.travel_time;
//			Cost_d += virtual_car_pstart(i, paths);
//			not_reach.insert(v.to_node.nodeid);
//		}
//	}
//}
//第一天算出来的路径
//from -> to,我要不要从from到to
double CalculateCostForDays(vector<ITEM>& paths) {
	int to_node_current_time_interval = 1;
	int leaving_time_interval;
	leaving_time_interval = to_node_current_time_interval;
	float Cost_d = 1.00;
	set<int> not_reach;
	map<int, pair<double, int> > passagerstart;
	for (auto i = vector<ITEM>::size_type(0); i < paths.size();++i) {
		auto v = paths[i];
		if (not_reach.find(v.from_node.nodeid) != not_reach.end() || 
			not_reach.find(v.to_node.nodeid) != not_reach.end())
		{
			//当一个点被滤掉，两条边同时被滤掉
			continue;
		}
		//v.is_common_node()/*物理节点*/
		//v.is_car_start_end()) //车到终点仓库
		to_node_current_time_interval += v.travel_time;//假设走到目标点
		Cost_d += v.travel_time;//行驶费用

		if (v.is_passenger()/*乘客*/) {
			if (to_node_current_time_interval < v.p_start()) {// 车要等待
				float costtmp = 0.5 * (v.p_start() - to_node_current_time_interval);
				Cost_d += costtmp;
				passagerstart[paths[i].to_node.nodeid] = make_pair(costtmp, v.travel_time + v.p_start() - to_node_current_time_interval);
				to_node_current_time_interval = v.p_start();
				
			}
			else if (v.p_start() <= to_node_current_time_interval &&
				to_node_current_time_interval <= v.p_end()) {//人等车
				Cost_d += 0.3 * (to_node_current_time_interval - v.p_start());
				passagerstart[paths[i].to_node.nodeid] = make_pair(0.3 * (to_node_current_time_interval - v.p_start()), v.travel_time);

			}
			else if (to_node_current_time_interval > v.p_end()) {//超过时间窗
				Cost_d -= v.travel_time;
				to_node_current_time_interval -= v.travel_time;
				Cost_d += virtual_car_pstart(i,paths);
				not_reach.insert(v.to_node.nodeid);
				not_reach.insert(v.to_node.nodeid + 1);
			}
		}
		else if (v.is_passenger_reach()) {//人到站
			if (to_node_current_time_interval<v.p_start()){//车虽到站，但是人就在车上等着
				Cost_d += 0.5 * (v.p_start() - to_node_current_time_interval);
				to_node_current_time_interval = v.p_start();
			}
			//else if (v.p_start() <= to_node_current_time_interval &&
			//	to_node_current_time_interval <= v.p_end()) {
			//}
			else if (to_node_current_time_interval > v.p_end()) {//超过时间窗
				Cost_d -= v.travel_time;
				to_node_current_time_interval -= v.travel_time;
				vector<ITEM>::size_type posstart = 0;
				Cost_d += virtual_car_pend(i,paths,posstart);
				not_reach.insert(v.to_node.nodeid);
				int startid = v.to_node.nodeid - 1;
				Cost_d -= passagerstart[startid].first;
				to_node_current_time_interval -= passagerstart[startid].second;
				Cost_d -= paths[posstart].travel_time;
				to_node_current_time_interval -= paths[posstart].travel_time;
			}
		}
	}
	Cost_d -= leaving_time_interval;
	return Cost_d;
}

void testmain(){
	double real = 19;
	vector<ITEM> paths;
	//11; 4; 2; 7; 2; 5; 9; 5; 6; 8; 6; 3; 10; 3; 1; 12;
	ITEM item;
	item.from_node.nodeid = 11;
	item.from_node.nodetype = 3;
	item.to_node.nodeid = 4;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 100;
	paths.push_back(item);
	//item

	
	item.from_node.nodeid = 4;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 2;
	item.to_node.nodetype = 0;
	item.travel_time = 2;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	
	item.from_node.nodeid = 2;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 7;
	item.to_node.nodetype = 1;
	item.travel_time = 1;
	item.to_node.p.start = 5;
	item.to_node.p.end = 7;
	item.link_type = 1;
	paths.push_back(item);

	
	item.from_node.nodeid = 7;
	item.from_node.nodetype = 1;
	item.to_node.nodeid = 2;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 1;
	paths.push_back(item);

	item.from_node.nodeid = 2;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 1;
	item.to_node.nodetype = 0;
	item.travel_time = 2;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	item.from_node.nodeid = 1;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 5;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	
	item.from_node.nodeid = 5;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 9;
	item.to_node.nodetype = 1;
	item.travel_time = 1;
    item.to_node.p.start = 8;
	item.to_node.p.end = 10;
	item.link_type = 2;
	paths.push_back(item);

	
	item.from_node.nodeid = 9;
	item.from_node.nodetype = 1;
	item.to_node.nodeid = 5;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 2;
	paths.push_back(item);

	item.from_node.nodeid = 5;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 6;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	
	item.from_node.nodeid = 6;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 8;
	item.to_node.nodetype = 2;
	item.travel_time = 1;
	item.to_node.p.start = 9;
	item.to_node.p.end = 13;
	item.link_type = -1;
	paths.push_back(item);

	
	item.from_node.nodeid = 8;
	item.from_node.nodetype = 2;
	item.to_node.nodeid = 6;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = -1;
	paths.push_back(item);
	
	item.from_node.nodeid = 6;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 3;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	
	item.from_node.nodeid = 3;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 10;
	item.to_node.nodetype = 2;
	item.travel_time = 1;
	item.to_node.p.start = 14;
	item.to_node.p.end = 19;
	item.link_type = -2;
	paths.push_back(item);

	
	item.from_node.nodeid = 10;
	item.from_node.nodetype = 2;
	item.to_node.nodeid = 3;
	item.to_node.nodetype = 0;
	item.travel_time = 1;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = -2;
	paths.push_back(item);

	
	item.from_node.nodeid = 3;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 1;
	item.to_node.nodetype = 0;
	item.travel_time = 2;
	//item.to_node.p.start = 1;
	//item.to_node.p.end = 1;
	item.link_type = 0;
	paths.push_back(item);

	
	item.from_node.nodeid = 1;
	item.from_node.nodetype = 0;
	item.to_node.nodeid = 12;
	item.to_node.nodetype = 3;
	item.travel_time = 1;
	item.to_node.p.start = 1;
	item.to_node.p.end = 30;
	item.link_type = 101;
	paths.push_back(item);

	double cost_d = CalculateCostForDays(paths);
	cout << cost_d << endl;
}