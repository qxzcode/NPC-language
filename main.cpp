//
//  main.cpp
//  NPC language
//
//  Created by Quinn on 7/28/15.
//  Copyright (c) 2015 Hexahedron Games. All rights reserved.
//

#include <iostream>

#include "Language.h"
#include "parser.h"
#include "stringUtil.h"
using namespace lang;

int main(int argc, const char * argv[]) {
	std::cout << "Program start\n\n";
	
	Language lang("/Users/quinn/Documents/C++/NPC language/NPC language/english.txt");
	std::cout << "\n" <<
		stringUtil::capitalize(lang.makeSentence(false,
						  pronoun{THIRD,true,NONE},
						  {lang.verbs[0],PRESENT,SIMPLE,false,true},
						  pronoun{THIRD,true,NONE}))
		<< "\n\n";
	
	parse::initWords(lang);
	try {
		parse::parseSentence("his few happy people berries");
	} catch (parse::GrammarException &ex) {
		std::cerr << ex.what() << std::endl;
	}
	std::cout << std::endl;
	
    return 0;
}

