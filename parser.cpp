//
//  parser.cpp
//  NPC language
//
//  Created by Quinn on 5/5/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#include "parser.h"

#include <iostream>

#include "stringUtil.h"

using namespace lang;
using namespace lang::parse;

void parse::parseSentence(std::string sentence) {
	std::vector<std::string> words = stringUtil::split(sentence, ' ');
	
	NounPhrase curNP;
	NPPart nextPart = NPPart::PREDET;
	nounInst lastNoun = nounInst::NO_NOUN;
	for (std::string word : words) {
		std::cout << "WORD: " << word << "\n";
		PreDet pd = getPreDet(word);
		Det d = getDet(word);
		PostDet pod = getPostDet(word);
		
		// determiners
		if (pd!=PreDet::NONE) {
			std::cout << "\tpredeterminer\n";
			if (nextPart<=NPPart::PREDET) {
				curNP = NounPhrase(); // new, blank NounPhrase
				curNP.preDet = pd;
				nextPart = NPPart::DET;
			} else throw GrammarException("Unexpected predeterminer");
		} else if (d!=Det::NONE) {
			std::cout << "\tdeterminer\n";
			if (nextPart<=NPPart::DET) {
				curNP.det = d;
				nextPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected central determiner");
		} else if (PossAdj pa = getPossAdj(word)) {
			std::cout << "\tpossessive adjective\n";
			if (nextPart<=NPPart::DET) {
				curNP.det = Det::POSS_ADJ;
				curNP.pa = pa;
				nextPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected possessive adjective (central determiner)");
		} else if (pod!=PostDet::NONE) {
			std::cout << "\tpostdeterminer\n";
			if (nextPart<=NPPart::POSTDET) {
				curNP.postDets.push_back(pod);
				nextPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected postdeterminer");
		}
		// adjective (premodifier)
		else if (adj a = getAdj(word)) {
			std::cout << "\tadjective\n";
			curNP.premods.emplace_back(a);
		}
		// noun (head or premodifier)
		else if (nounInst ni = getNoun(word)) {
			std::cout << "\tnoun (" << (ni.sing?"sing":"pl") << ")\n";
			if (lastNoun) {
				curNP.premods.emplace_back(lastNoun);
			}
			lastNoun = ni;
		}
		
		// verbs...
		//...
		
		// unrecognized word
		else {
			throw GrammarException("Unrecognized word: "+word);
		}
		
		//...
	}
}
