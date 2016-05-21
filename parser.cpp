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

std::string VerbPhrase::toString() const {
	static const std::string tenseNames[] = {"past","present","future"};
	static const std::string tfNames[] = {"simple","continuous","emphatic"};
	if (!this->verb) return "[null VP]";
	return "[to "+verb.base+": "+tfNames[tenseForm]+" "+tenseNames[tense]+(perfect?" perfect":"")+", "+(active?"active":"passive")+"]";
}

void parse::parseSentence(std::string sentence) {std::cout << getPastPart("eaten").pastPart << " " << (getPastPart("eaten")==verb::TO_BE) << "\n";
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
		else if (verb papv = getPastPart(word)) {
			switch (ps.lastVPPart) {
				case VPPart::PERFAUX:
					std::cout << "\tparticiple in perfect verb\n";
					ps.curVP.verb = papv;
					ps.lastVPPart = papv.isToBe()? VPPart::TOBE : VPPart::END;
					break;
				case VPPart::TOBE:
					if (papv.isToBe()) throw GrammarException("\"been\" as passive verb");
					std::cout << "\tparticiple in passive verb\n";
					ps.curVP.verb = papv;
					ps.curVP.active = false;
					ps.lastVPPart = VPPart::END;
					break;
				default:
					throw GrammarException("unexpected past participle");
			}
		}
		else if (verb prpv = getPresPart(word)) {
			switch (ps.lastVPPart) {
				case VPPart::TOBE:
					std::cout << "\tparticiple in continuous verb\n";
					ps.curVP.verb = prpv;
					ps.curVP.tenseForm = PROGRESSIVE;
					ps.lastVPPart = prpv.isToBe()? VPPart::TOBE : VPPart::END;
					break;
				default:
					throw GrammarException("unexpected present participle");
			}
		}
		else if (verb spv = getSimplePast(word)) {
			if (ps.lastVPPart==VPPart::BEGIN) {
				std::cout << "\tsimple past\n";
				ps.curVP.verb = spv;
				ps.curVP.tense = PAST;
				ps.lastVPPart = VPPart::END;
			} else throw GrammarException("Unexpected simple past");
		}
		else if (verb bv = getBase(word)) {
			if (ps.lastVPPart==VPPart::BEGIN) {
				std::cout << "\tsimple present (not 3rd sing)\n";
				ps.curVP.verb = bv;
				ps.curVP.tense = PRESENT;
				ps.curVP.pns ^= SING3; // all except third person singular
				ps.lastVPPart = VPPart::END;
			} else throw GrammarException("Unexpected simple present");
		}
		else if (verb tsv = getThirdSing(word)) {
			if (ps.lastVPPart==VPPart::BEGIN) {
				std::cout << "\tsimple present (3rd sing)\n";
				ps.curVP.verb = tsv;
				ps.curVP.tense = PRESENT;
				ps.curVP.pns = SING3; // only third person singular
				ps.lastVPPart = VPPart::END;
			} else throw GrammarException("Unexpected simple present");
		}
		else if (pnFlags prtb = getPresentToBe(word)) {
			if (ps.lastVPPart==VPPart::BEGIN) {
				std::cout << "\tpresent to be\n";
				ps.curVP.verb = verb::TO_BE;
				ps.curVP.tense = PRESENT;
				ps.curVP.pns = prtb;
				ps.lastVPPart = VPPart::TOBE;
			} else throw GrammarException("Unexpected present to be");
		}
		else if (pnFlags patb = getPastToBe(word)) {
			if (ps.lastVPPart==VPPart::BEGIN) {
				std::cout << "\tpast to be\n";
				ps.curVP.verb = verb::TO_BE;
				ps.curVP.tense = PAST;
				ps.curVP.pns = patb;
				ps.lastVPPart = VPPart::TOBE;
			} else throw GrammarException("Unexpected past to be");
		}
		
		// unrecognized word
		else {
			throw GrammarException("Unrecognized word: "+word);
		}
		
		//...
	}
	std::cout << "verb phrase: " << ps.curVP.toString() << "\n";
}
