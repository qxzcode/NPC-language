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
	
	struct tmp {
		NounPhrase curNP;
		NPPart nextNPPart = NPPart::PREDET;
		nounInst lastNoun = nounInst::NO_NOUN;
		void endNP() {
			if (nextNPPart!=NPPart::PREDET) {
				curNP.head = lastNoun;
				lastNoun = nounInst::NO_NOUN;
				nextNPPart = NPPart::PREDET;
			}
		}
		
		VerbPhrase curVP;
		VPPart lastVPPart = VPPart::BEGIN;
		void endVP() {
			//...
		}
	};tmp ps;
	for (std::string word : words) {
		std::cout << "WORD: " << word << "\n";
		PreDet pd = getPreDet(word);
		Det d = getDet(word);
		PostDet pod = getPostDet(word);
		
		// determiners
		if (pd!=PreDet::NONE) {
			std::cout << "\tpredeterminer\n";
			if (ps.nextNPPart<=NPPart::PREDET) {
				ps.curNP = NounPhrase(); // new, blank NounPhrase
				ps.curNP.preDet = pd;
				ps.nextNPPart = NPPart::DET;
			} else throw GrammarException("Unexpected predeterminer");
		} else if (d!=Det::NONE) {
			std::cout << "\tdeterminer\n";
			if (ps.nextNPPart<=NPPart::DET) {
				ps.curNP.det = d;
				ps.nextNPPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected central determiner");
		} else if (PossAdj pa = getPossAdj(word)) {
			std::cout << "\tpossessive adjective\n";
			if (ps.nextNPPart<=NPPart::DET) {
				ps.curNP.det = Det::POSS_ADJ;
				ps.curNP.pa = pa;
				ps.nextNPPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected possessive adjective (central determiner)");
		} else if (pod!=PostDet::NONE) {
			std::cout << "\tpostdeterminer\n";
			if (ps.nextNPPart<=NPPart::POSTDET) {
				ps.curNP.postDets.push_back(pod);
				ps.nextNPPart = NPPart::POSTDET;
			} else throw GrammarException("Unexpected postdeterminer");
		}
		// adjective (premodifier)
		else if (adj a = getAdj(word)) {
			std::cout << "\tadjective\n";
			ps.curNP.premods.emplace_back(a);
		}
		// noun (head or premodifier)
		else if (nounInst ni = getNoun(word)) {
			std::cout << "\tnoun (" << (ni.sing?"sing":"pl") << ")\n";
			if (ps.lastNoun) {
				ps.curNP.premods.emplace_back(ps.lastNoun);
			}
			ps.lastNoun = ni;
		}
		
		// verbs...
		else if (PerfAux pa = getPerfAux(word)) {
			std::cout << "\t" << (pa.present?"present":"past") << " perfect aux\n";
			if (ps.lastVPPart==VPPart::BEGIN) {
				ps.endNP();
				ps.curVP.tense = pa.present? PRESENT : PAST;
				ps.curVP.perfect = true;
				ps.lastVPPart = VPPart::PERFAUX;
			} else throw GrammarException("Unexpected perfect aux");
		}
		else if (verb v = ps.lastVPPart==VPPart::PERFAUX?getPastPart(word):verb{""}) {
			std::cout << "\tparticiple in perfect verb\n";
			ps.curVP.verb = v;
			ps.lastVPPart = v.isToBe()? VPPart::TOBE : VPPart::END;
		}
		
		// unrecognized word
		else {
			throw GrammarException("Unrecognized word: "+word);
		}
		
		//...
	}
}
