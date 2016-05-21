//
//  words.cpp
//  NPC language
//
//  Created by Quinn on 5/5/16.
//  Copyright (c) 2016 Hexahedron Games. All rights reserved.
//

#include "words.h"

#include <map>

using namespace lang;
using namespace lang::parse;

PossAdj::operator bool() const {
	return person>=0;
}
PerfAux::operator bool() const {
	return present || !third;
}

bool nounInst::operator==(const nounInst& rhs) const {
	return (sing==rhs.sing) && (n.sing==rhs.n.sing);
}
bool nounInst::operator!=(const nounInst& rhs) const {
	return !(*this==rhs);
}
nounInst::operator bool() const {
	return *this!=NO_NOUN;
}
const nounInst nounInst::NO_NOUN{{"[no noun]","[no noun]",true},true};

std::map<std::string, PreDet> preDets;
std::map<std::string, Det> dets;
std::map<std::string, PostDet> postDets;
std::map<std::string, PossAdj> possAdjs;
std::map<std::string, adj> adjs;
std::map<std::string, nounInst> nouns;

std::map<std::string, verb> pastParts;
std::map<std::string, verb> presParts;
std::map<std::string, verb> bases;
std::map<std::string, verb> thirdSings;
std::map<std::string, verb> simplePasts;

void parse::initWords(Language& lang) {
	preDets["all"] = PreDet::ALL;
	preDets["both"] = PreDet::BOTH;
	dets["a"]=dets["an"] = Det::A;
	dets["the"] = Det::THE;
	dets["this"] = Det::THIS;
	dets["these"] = Det::THESE;
	dets["that"] = Det::THAT;
	dets["those"] = Det::THOSE;
	postDets["many"] = PostDet::MANY;
	postDets["few"] = PostDet::FEW;
	possAdjs["my"] = {FIRST,true};
	possAdjs["our"] = {FIRST,false};
	possAdjs["your"] = {SECOND,true};
	possAdjs["his"] = {THIRD,true,MALE};
	possAdjs["her"] = {THIRD,true,FEMALE};
	possAdjs["its"] = {THIRD,true,NONE};
	possAdjs["their"] = {THIRD,false};
	for (adj& a : lang.adjs) {
		adjs[a.adj] = a;
	}
	for (noun& n : lang.nouns) {
		nouns[n.sing] = {n,true};
		nouns[n.pl] = {n,false};
	}
	pastParts[verb::TO_BE.pastPart] = verb::TO_BE;
	presParts[verb::TO_BE.ing] = verb::TO_BE;
	for (verb& v : lang.verbs) {
		pastParts[v.pastPart] = v;
		presParts[v.ing] = v;
		bases[v.base] = v;
		thirdSings[v.thirdPers] = v;
		simplePasts[v.past] = v;
	}
}

PreDet parse::getPreDet(std::string str) {
	auto it = preDets.find(str);
	return it==preDets.end()? PreDet::NONE : it->second;
}
Det parse::getDet(std::string str) {
	auto it = dets.find(str);
	return it==dets.end()? Det::NONE : it->second;
}
PostDet parse::getPostDet(std::string str) {
	auto it = postDets.find(str);
	return it==postDets.end()? PostDet::NONE : it->second;
}
PossAdj parse::getPossAdj(std::string str) {
	auto it = possAdjs.find(str);
	return it==possAdjs.end()? PossAdj{PERSON(-1)} : it->second;
}
adj parse::getAdj(std::string str) {
	auto it = adjs.find(str);
	return it==adjs.end()? adj{""} : it->second;
}
nounInst parse::getNoun(std::string str) {
	auto it = nouns.find(str);
	return it==nouns.end()? nounInst::NO_NOUN : it->second;
}

PerfAux parse::getPerfAux(std::string str) {
	if (str=="had") return {false,false};
	if (str=="have") return {true,false};
	if (str=="has") return {true,true};
	return {false,true};
}
verb parse::getPastPart(std::string str) {
	auto it = pastParts.find(str);
	return it==pastParts.end()? verb{""} : it->second;
}
verb parse::getPresPart(std::string str) {
	auto it = presParts.find(str);
	return it==presParts.end()? verb{""} : it->second;
}
verb parse::getBase(std::string str) {
	auto it = bases.find(str);
	return it==bases.end()? verb{""} : it->second;
}
verb parse::getThirdSing(std::string str) {
	auto it = thirdSings.find(str);
	return it==thirdSings.end()? verb{""} : it->second;
}
verb parse::getSimplePast(std::string str) {
	auto it = simplePasts.find(str);
	return it==simplePasts.end()? verb{""} : it->second;
}
pnFlags parse::getPresentToBe(std::string str) {
	if (str=="are") return ALL ^ (SING1|SING3);
	if (str=="am") return SING1;
	if (str=="is") return SING3;
	return 0;
}
pnFlags parse::getPastToBe(std::string str) {
	if (str=="were") return ALL ^ (SING1|SING3);
	if (str=="was") return SING1|SING3;
	return 0;
}
