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
