//
//  stringUtil.h
//  NPC language
//
//  Created by Quinn on 5/6/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace stringUtil {
	
	inline std::string capitalize(std::string str) {
		str[0] = std::toupper(str[0]);
		return str;
	}
	
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
	
}