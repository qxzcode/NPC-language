//
//  stringUtil.cpp
//  NPC language
//
//  Created by Quinn on 5/6/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#include "stringUtil.h"

#include <sstream>

std::vector<std::string> &stringUtil::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
std::vector<std::string> stringUtil::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
