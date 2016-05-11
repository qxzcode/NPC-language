//
//  words.h
//  NPC language
//
//  Created by Quinn on 5/5/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>

#include "Language.h"

namespace lang { namespace parse {
	
	/// noun phrase parts
	enum struct NPPart {
		PREDET, DET, POSTDET, PREMOD, HEAD, POSTMOD
	};
	enum struct PreDet { // predeterminers
		NONE = 0,
		ALL, BOTH
		//...
	};
	enum struct Det { // central determiners
		NONE = 0,
		A, THE,
		THIS, THESE, THAT, THOSE,
		POSS_ADJ
		//...
	};
	enum struct PostDet { // postdeterminers
		NONE = 0,
		MANY, FEW
		//...
	};
	struct PossAdj {
		PERSON person;
		bool sing;
		GENDER gender;
		operator bool() const;
	};
	
	void initWords(Language& lang);
	PreDet getPreDet(std::string str);
	Det getDet(std::string str);
	PostDet getPostDet(std::string str);
	PossAdj getPossAdj(std::string str);
	adj getAdj(std::string str);
	struct nounInst {
		noun n; bool sing;
		bool operator==(const nounInst& rhs) const;
		bool operator!=(const nounInst& rhs) const;
		operator bool() const;
		static const nounInst NO_NOUN;
	};
	nounInst getNoun(std::string str);
	
} }
