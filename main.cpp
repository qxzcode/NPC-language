//
//  main.cpp
//  NPC language
//
//  Created by Quinn on 7/28/15.
//  Copyright (c) 2015 Hexahedron Games. All rights reserved.
//

#include <iostream>

#include "Language.h"

int main(int argc, const char * argv[]) {
	std::cout << "Program start\n\n";
	
	Language lang("/Users/quinn/Documents/C++/NPC language/NPC language/english.txt");
	std::cout << "\n" <<
		capitalize(lang.makeSentence(false,
						  pronoun{THIRD,true,NONE},
						  {lang.verbs[0],PRESENT,SIMPLE,false,true},
						  pronoun{THIRD,true,NONE}))
		<< "\n";
	
    return 0;
}

