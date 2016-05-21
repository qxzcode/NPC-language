//
//  Language.cpp
//  NPC language
//
//  Created by Quinn on 7/28/15.
//  Copyright (c) 2015 Hexahedron Games. All rights reserved.
//

#include "Language.h"

#include <fstream>
#include <sstream>

const verb verb::TO_BE = {"be","is","being","was","been",false,false};

static bool isBegVow(std::string word) {
	char l = word[0];
	return l=='a' || l=='e' || l=='i' || l=='o' || l=='u';
}

Language::Language(std::string file) {
	std::ifstream in(file);
	if (!in.is_open()) {
		printf("Failed to open lang file: %s\n", file.c_str());
		return;
	}
	
	std::string line;
	while (getline(in, line)) {
		if (line.empty()) continue;
		std::istringstream ss(line);
		std::string tok;
		
		ss >> tok;
		if (tok == "n") {
			std::string s, p;
			ss >> s >> p;
			nouns.push_back(noun{s,p,isBegVow(s)});
			printf("Noun: %s/%s\n", s.c_str(), p.c_str());
		} else if (tok == "a") {
			std::string a, n;
			ss >> a >> n;
			adjs.push_back(adj{a,n,isBegVow(a)});
			printf("Adjective: %s (%s)\n", a.c_str(), n.c_str());
		} else if (tok == "v") {
			std::string t, b, th, i, p, pp;
			ss >> t >> b >> th >> i >> p >> pp;
			verbs.push_back(verb{b,th,i,p,pp,t=="t",isBegVow(pp)});
			printf("Verb: %s/%s/%s/%s\n", b.c_str(), i.c_str(), p.c_str(), pp.c_str());
		} else {
			printf("Invalid word type in line \"%s\"\n", line.c_str());
		}
	}
}



static std::string makeSubObj(subjObj so, subjObj sub = subjObj()) {
	if (so.isPronoun) {
		pronoun p = so.pronoun;
		if (sub.init && sub.pronoun==p) { // reflexive pronoun
			static const std::string pronoun[2][3] = {{"ourselves","yourselves","themselves"},{"myself","yourself",""}};
			static const std::string pronoun2[3] = {"itself","himself","herself"};
			if (p.sing && p.person==THIRD)
				return pronoun2[p.gender];
			else
				return pronoun[p.sing][p.person];
		}
		static const std::string pronoun[2][2][3] = {{{"us","you","them"},{"me","you",""}},{{"we","you","they"},{"I","you",""}}};
		static const std::string pronoun2[2][3] = {{"it","him","her"},{"it","he","she"}};
		if (p.sing && p.person==THIRD)
			return pronoun2[!sub.init][p.gender];
		else
			return pronoun[!sub.init][p.sing][p.person];
	} else {
		nounPhrase& np = so.nounPhrase;
		std::string mod;
		switch (np.mod) {
			case IND_ARTICLE:
				mod = np.sing? (np.noun.begVow?"an ":"a ") : "some ";
				break;
			case DEF_ARTICLE:
				mod = "the ";
				break;
			case DEM_CLOSE:
				mod = np.sing? "this " : "these ";
				break;
			case DEM_FAR:
				mod = np.sing? "that " : "those ";
				break;
			case N_POSSESSIVE:
				mod = "{TODO}";
				break;
			case PN_POSSESSIVE:
				static const std::string possPN[2][3] = {{"our","your","their"},{"my","your",""}};
				static const std::string possPN2[3] = {"its","his","her"};
				if (np.possPronoun.sing && np.possPronoun.person==THIRD)
					mod = possPN2[np.possPronoun.gender];
				else
					mod = possPN[np.possPronoun.sing][np.possPronoun.person];
				mod += ' ';
				break;
		}
		return mod + (np.sing?np.noun.sing:np.noun.pl);
	}
}

static std::string makeAuxToBe(bool sing, PERSON per, TENSE tense, bool perfect) {
	if (tense==FUTURE) {
		return "will";
	}
	if (perfect) {
		static const std::string form[2] = {"had","have"};
		return form[tense];
	} else {
		static const std::string form[2][2] = {{"were","was"},{"are","am"}};
		static const std::string form3rd[2] = {"was","is"};
		std::string str = sing&&per==THIRD? form3rd[tense] : form[tense][sing&&per==FIRST];
		return str;
	}
}

static std::string makeMainToBe(TENSE tense, TENSE_FORM tenseForm, bool perfect, bool positive) {
	if (perfect) {
		std::string str = tense==FUTURE? "have " : "";
		str += positive? "been" : "not been";
		if (tenseForm==PROGRESSIVE) str += " being";
		return str;
	} else {
		std::string str = positive? "" : "not ";
		if (tense==FUTURE) {
			str += tenseForm==PROGRESSIVE? "be being" : "be";
		} else {
			if (tenseForm==PROGRESSIVE)
				str += "being";
			else if (!positive)
				str = "not";
		}
		return str;
	}
}

static bool vpHasAux(verbPhrase vp, bool question) {
	return question || !vp.positive || vp.tenseForm==EMPHATIC || vp.perfect || vp.tense==FUTURE;
}

static std::string makeAuxVerb(verbPhrase vp, subjObj sub, bool question) {
	if (vp.tenseForm == PROGRESSIVE) {
		return makeAuxToBe(sub.sing, sub.person, vp.tense, vp.perfect);
	}
	if (!vpHasAux(vp, question))
		return "";
	static const std::string auxVerb[2][3][2] = {{{"did","did"},{"do","does"},{"will","will"}},{{"had","had"},{"have","has"},{"will","will"}}};
	return auxVerb[vp.perfect][vp.tense][sub.sing&&sub.person==THIRD];
}

static std::string makeMainVerb(verbPhrase vp, subjObj sub, bool question) {
	if (vp.tenseForm == PROGRESSIVE) {
		std::string str = makeMainToBe(vp.tense, SIMPLE, vp.perfect, vp.positive);
		if (!str.empty()) str += ' ';
		return str + vp.verb.ing;
	}
	std::string str = vp.positive? "" : "not ";
	if (vpHasAux(vp, question)) {
		str += vp.perfect? (vp.tense==FUTURE? "have "+vp.verb.pastPart : vp.verb.pastPart) : vp.verb.base;
	} else if (vp.tense==PAST) { // simple past
		str += vp.verb.past;
	} else { // simple present
		str += sub.sing&&sub.person==THIRD? vp.verb.thirdPers : vp.verb.base;
	}
	return str;
}

std::string Language::makeSentence(bool question, subjObj subject, verbPhrase verb) {
	if (question) {
		return makeAuxVerb(verb, subject, true) +' '+ makeSubObj(subject) +' '+ makeMainVerb(verb, subject, true);
	} else {
		std::string str = makeSubObj(subject) + ' ';
		std::string auxV = makeAuxVerb(verb, subject, false);
		if (!auxV.empty()) str += auxV + ' ';
		str += makeMainVerb(verb, subject, false);
		return str;
	}
}

std::string Language::makeSentence(bool question, subjObj subject, verbPhrase verb, subjObj object) {
	return makeSentence(question, subject, verb) +' '+ makeSubObj(object, subject);
}


