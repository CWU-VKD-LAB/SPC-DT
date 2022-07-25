//#include "stdafx.h"
//#pragma once
//
//enum EdgeType {Horizontal, Vertical};
//
//struct Edge {
//	std::vector<Zone*> zonesSide1;
//	std::vector<Zone*> zonesSide2;
//	EdgeType type;
//	int id;
//	Edge();
//	Edge(Zone& z1, Zone& z2, int id, EdgeType type) {
//		zonesSide1.push_back(&z1);
//		zonesSide2.push_back(&z2);
//		this->id = id;
//		this->type = type;
//	}
//};