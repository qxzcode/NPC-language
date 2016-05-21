//
//  Language.h
//  NPC language
//
//  Created by Quinn on 7/28/15.
//  Copyright (c) 2015 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

/// WORD TYPES (FOR STORED VOCAB) ///

struct noun {
	std::string sing, pl;
	bool begVow;
	operator bool() const {return !sing.empty();}
};

struct adj {
	std::string adj, noun;
	bool begVow;
	operator bool() const {return !adj.empty();}
};

struct verb {
	std::string base, thirdPers, ing, past, pastPart;
	bool trans;
	bool ppBegVow;
	operator bool() const {return !base.empty();}
	bool isToBe() const {return *this==TO_BE;}
	bool operator==(const verb &rhs) const {return base==rhs.base&&pastPart==rhs.pastPart;} // should be good enough
	static const verb TO_BE;
};


/// SENTENCE PARTS ///

enum TENSE {
	PAST=0, PRESENT=1, FUTURE=2
};
enum TENSE_FORM {
	SIMPLE=0, PROGRESSIVE=1, EMPHATIC=2
};
struct verbPhrase {
	verb verb;
	TENSE tense;
	TENSE_FORM tenseForm;
	bool perfect;
	bool positive;
};

enum PERSON {
	FIRST=0, SECOND=1, THIRD=2
};
enum GENDER {
	NONE=0, MALE=1, FEMALE=2
};
struct pronoun {
	PERSON person;
	bool sing;
	GENDER gender;
	
	bool operator==(const pronoun& p) const {
		return person==p.person && sing==p.sing && (!sing||person!=THIRD || gender==p.gender);
	}
};

enum NOUN_MOD {
	IND_ARTICLE, DEF_ARTICLE, DEM_CLOSE, DEM_FAR, N_POSSESSIVE, PN_POSSESSIVE
};
struct nounPhrase {
	noun noun;
	bool sing;
	NOUN_MOD mod;
	struct noun* possNoun;
	pronoun possPronoun;
};

struct subjObj {
	subjObj():init(false) {}
	subjObj(pronoun p):init(true),isPronoun(true),pronoun(p),sing(p.sing),person(p.person) {}
	subjObj(nounPhrase np):init(true),isPronoun(false),nounPhrase(np),sing(np.sing),person(THIRD) {}
	
	bool init;
	
	bool isPronoun;
	pronoun pronoun;
	nounPhrase nounPhrase;
	
	bool sing;
	PERSON person;
};


/// Language CLASS //

class Language {
public:
	Language(std::string file);
	
	std::string makeSentence(bool question, subjObj subject, verbPhrase verb);
	std::string makeSentence(bool question, subjObj subject, verbPhrase verb, subjObj object);
	
	std::vector<noun> nouns;
	std::vector<adj> adjs;
	std::vector<verb> verbs;
	
};