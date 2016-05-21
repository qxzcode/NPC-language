//
//  parser.h
//  NPC language
//
//  Created by Quinn on 5/5/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "words.h"

namespace lang { namespace parse {
	
	class GrammarException: public std::runtime_error {
	public:
		GrammarException(std::string msg):runtime_error("Grammar error: "+msg) {}
	};
	
	struct Participle {
		verb verb;
		bool past; // passive past or present active (e.g. killed vs. killing)
	};
	struct Premodifier {
		Premodifier(nounInst ni):type(NOUN),noun(ni) {}
		Premodifier(adj a):type(ADJ),adj(a) {}
		enum {
			ADJ, NOUN, PART
		} type;
		adj adj;
		nounInst noun;
		Participle part;
	};
	struct NounPhrase {
		PreDet preDet = PreDet::NONE;
		Det det = Det::NONE;
		PossAdj pa;
		std::vector<PostDet> postDets;
		std::vector<Premodifier> premods;
		nounInst head = nounInst::NO_NOUN;
	};
	
	struct VerbPhrase {
		pnFlags pns = ALL;
		TENSE tense;
		TENSE_FORM tenseForm = SIMPLE;
		bool perfect = false;
		bool active = true;
		verb verb;
		std::string toString() const;
	};
	
	void parseSentence(std::string sentence);
	
} }