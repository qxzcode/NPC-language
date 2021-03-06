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
	
	/// verb phrase parts
	enum struct VPPart {
		BEGIN, PERFAUX, TOBE, END
	};
	struct PerfAux {
		bool present;
		bool third;
		operator bool() const;
	};
	typedef unsigned char pnFlags;
	constexpr pnFlags SING1=0x1,SING2=0x2,SING3=0x4,PL1=0x8,PL2=0x10,PL3=0x20,
					  ALL=0x3f;
	
	PerfAux getPerfAux(std::string str);
	verb getPastPart(std::string str);
	verb getPresPart(std::string str);
	verb getBase(std::string str);
	verb getThirdSing(std::string str);
	verb getSimplePast(std::string str);
	pnFlags getPresentToBe(std::string str);
	pnFlags getPastToBe(std::string str);
	
} }
