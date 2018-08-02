//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2015
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software{ you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation{ either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY{ without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program{ if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#include <iostream>
#include <vector>
#include "lima/Debug.h"
#include "lima/Exceptions.h"
#include "lima/MiscUtils.h"
#include "MpxCommon.h"
#include "MpxDacs.h"

using namespace lima;
using namespace lima::Maxipix;

MpxFsrDef* MpxFsrDef::m_instance = NULL;
MpxFsrDef* MpxFsrDef::getInstance(Version version) {
	if (!m_instance)   // Only allow one instance of class to be generated.
		m_instance = new MpxFsrDef(version);

	return m_instance;
}

MpxFsrDef::MpxFsrDef(Version version) {
	m_fsrKeys = std::map<std::string,std::vector<std::pair<int,int> > >();
	m_dacCode = std::map<std::string, int>();
	m_default = std::map<std::string, int>();

	switch (version) {
	case MPX2: initMPX2(); break;
	case MXR2: initMXR2(); break;
	case TPX1: initTPX1(); break;
	case DUMMY: initDUMMY(); break;
	}
}

std::vector<std::string>  MpxFsrDef::listKeys(bool saved) {
	DEB_MEMBER_FUNCT();
	std::vector<std::string> keys;
	if (saved) {
		for (std::map<std::string,std::vector<std::pair<int,int> > >::iterator it=m_fsrKeys.begin(); it!=m_fsrKeys.end(); ++it) {
			if (it->first != "daccode" && it->first != "sensedac" && it->first != "dacsel"){
			  keys.push_back(it->first);
			}
		} 
	}else {
		for (std::map<std::string,std::vector<std::pair<int,int> > >::iterator it=m_fsrKeys.begin(); it!=m_fsrKeys.end(); ++it) {
			keys.push_back(it->first);
		}
	}
	return keys;
}

std::vector<std::pair<int,int> >&  MpxFsrDef::fsrValues(std::string key) {
	DEB_MEMBER_FUNCT();
	return m_fsrKeys[key];
}

std::map<std::string, int>& MpxFsrDef::dacCode() {
	DEB_MEMBER_FUNCT();
	return m_dacCode;
}

std::map<std::string, int>& MpxFsrDef::getDefault() {
	DEB_MEMBER_FUNCT();
	return m_default;
}

//#if __GNUC_MINOR__ < 4
void MpxFsrDef::initDUMMY() {
	DEB_MEMBER_FUNCT();
	std::vector<std::pair<int, int> > v1;
	v1.push_back(std::make_pair<int, int>(3,4));
	m_fsrKeys["testa"] = v1;
	std::vector<std::pair<int, int> > v2;
	v2.push_back(std::make_pair<int, int>(8,15));
	m_fsrKeys["testb"] = v2;
	std::vector<std::pair<int, int> > v3;
	v3.push_back(std::make_pair<int, int>(32,35));
	v3.push_back(std::make_pair<int, int>(44,47));
	m_fsrKeys["testc"] = v3;
}

void MpxFsrDef::initTPX1() {
	DEB_MEMBER_FUNCT();
	std::vector<std::pair<int, int> > v1;
	v1.push_back(std::make_pair<int, int>(4,11));
	m_fsrKeys["ikrum"] = v1;
	std::vector<std::pair<int, int> > v2;
	v2.push_back(std::make_pair<int, int>(12,19));
	m_fsrKeys["disc"] = v2;
	std::vector<std::pair<int, int> > v3;
	v3.push_back(std::make_pair<int, int>(20,27));
	m_fsrKeys["preamp"] = v3;
	std::vector<std::pair<int, int> > v4;
	v4.push_back(std::make_pair<int, int>(38,39));
	v4.push_back(std::make_pair<int, int>(41,42));
	m_fsrKeys["daccode"] = v4;
	std::vector<std::pair<int, int> > v5;
	v5.push_back(std::make_pair<int, int>(43,43));
	m_fsrKeys["sensedac"] = v5;
	std::vector<std::pair<int, int> > v6;
	v6.push_back(std::make_pair<int, int>(44,44));
	m_fsrKeys["dacsel"] = v6;
	std::vector<std::pair<int, int> > v7;
	v7.push_back(std::make_pair<int, int>(46,49));
	v7.push_back(std::make_pair<int, int>(56,59));
	m_fsrKeys["buffanaloga"] = v7;
	std::vector<std::pair<int, int> > v8;
	v8.push_back(std::make_pair<int, int>(60,67));
	m_fsrKeys["buffanalogb"] = v8;
	std::vector<std::pair<int, int> > v9;
	v9.push_back(std::make_pair<int, int>(86,93));
	m_fsrKeys["hist"] = v9;
	std::vector<std::pair<int, int> > v10;
	v10.push_back(std::make_pair<int, int>(100,113));
	m_fsrKeys["thl"] = v10;
	std::vector<std::pair<int, int> > v11;
	v11.push_back(std::make_pair<int, int>(120,127));
	m_fsrKeys["vcas"] = v11;
	std::vector<std::pair<int, int> > v12;
	v12.push_back(std::make_pair<int, int>(128,135));
	m_fsrKeys["fbk"] = v12;
	std::vector<std::pair<int, int> > v13;
	v13.push_back(std::make_pair<int, int>(136,143));
	m_fsrKeys["gnd"] = v13;
	std::vector<std::pair<int, int> > v14;
	v14.push_back(std::make_pair<int, int>(144,175));
	m_fsrKeys["ctpr"] = v14;
	std::vector<std::pair<int, int> > v15;
	v15.push_back(std::make_pair<int, int>(181,188));
	m_fsrKeys["ths"] = v15;
	std::vector<std::pair<int, int> > v16;
	v16.push_back(std::make_pair<int, int>(227,234));
	m_fsrKeys["biaslvds"] = v16;
	std::vector<std::pair<int, int> > v17;
	v17.push_back(std::make_pair<int, int>(235,242));
	m_fsrKeys["reflvds"] = v17;

	typedef std::pair<std::string, int> DefaultPair;
	static const DefaultPair DefaultCList[] = {
		DefaultPair("ikrum", 200),
		DefaultPair("disc", 250),
		DefaultPair("preamp", 128),
		DefaultPair("buffanaloga", 128),
		DefaultPair("buffanalogb", 128),
		DefaultPair("hist", 128),
		DefaultPair("vcas", 128),
		DefaultPair("fbk", 115),
		DefaultPair("gnd", 128),
		DefaultPair("ths", 180),
		DefaultPair("biaslvds", 128),
		DefaultPair("reflvds", 129),
		DefaultPair("thl", 6600),
	};
	std::map<std::string, int> m_default(C_LIST_ITERS(DefaultCList));

	typedef std::pair<std::string, int> DacCodePair;
	static const DacCodePair DacCodeCList[] = {
		DacCodePair("ikrum", 0xf),
		DacCodePair("disc", 0xb),
		DacCodePair("preamp", 0x7),
		DacCodePair("buffanaloga", 0x3),
		DacCodePair("buffanalogb", 0x4),
		DacCodePair("hist", 0x9),
		DacCodePair("thl", 0x6),
		DacCodePair("vcas", 0xc),
		DacCodePair("fbk", 0xa),
		DacCodePair("gnd", 0xd),
		DacCodePair("ths", 0x1),
		DacCodePair("biaslvds", 0x2),
		DacCodePair("reflvds", 0xe),
	};
	std::map<std::string, int> m_dacCode(C_LIST_ITERS(DacCodeCList));
}

void MpxFsrDef::initMPX2() {
	DEB_MEMBER_FUNCT();
	std::vector<std::pair<int, int> > v1;
	v1.push_back(std::make_pair<int, int>(3,10));
	m_fsrKeys["biasdelayn"] = v1;
	std::vector<std::pair<int, int> > v2;
	v2.push_back(std::make_pair<int, int>(11,18));
	m_fsrKeys["biasdisc"] = v2;
	std::vector<std::pair<int, int> > v3;
	v3.push_back(std::make_pair<int, int>(19,26));
	m_fsrKeys["biaspreamp"] = v3;
	std::vector<std::pair<int, int> > v4;
	v4.push_back(std::make_pair<int, int>(45,48));
	v4.push_back(std::make_pair<int, int>(55,58));
	m_fsrKeys["biassetdisc"] = v4;
	std::vector<std::pair<int, int> > v5;
	v5.push_back(std::make_pair<int, int>(59,66));
	m_fsrKeys["biasths"] = v5;
	std::vector<std::pair<int, int> > v6;
	v6.push_back(std::make_pair<int, int>(99,106));
	m_fsrKeys["biasikrum"] = v6;
	std::vector<std::pair<int, int> > v7;
	v7.push_back(std::make_pair<int, int>(107,114));
	m_fsrKeys["biasabuffer"] = v7;
	std::vector<std::pair<int, int> > v8;
	v8.push_back(std::make_pair<int, int>(115,122));
	m_fsrKeys["thh"] = v8;
	std::vector<std::pair<int, int> > v9;
	v9.push_back(std::make_pair<int, int>(123,130));
	m_fsrKeys["thl"] = v9;
	std::vector<std::pair<int, int> > v10;
	v10.push_back(std::make_pair<int, int>(131,138));
	m_fsrKeys["fbk"] = v10;
	std::vector<std::pair<int, int> > v11;
	v11.push_back(std::make_pair<int, int>(180,187));
	m_fsrKeys["gnd"] = v11;
	std::vector<std::pair<int, int> > v12;
	v12.push_back(std::make_pair<int, int>(226,233));
	m_fsrKeys["biaslvdstx"] = v12;
	std::vector<std::pair<int, int> > v13;
	v13.push_back(std::make_pair<int, int>(234,241));
	m_fsrKeys["reflvdstx"] = v13;
	std::vector<std::pair<int, int> > v14;
	v14.push_back(std::make_pair<int, int>(37,38));
	v14.push_back(std::make_pair<int, int>(40,40));
	v14.push_back(std::make_pair<int, int>(41,41));
	m_fsrKeys["daccode"] = v14;
	std::vector<std::pair<int, int> > v15;
	v15.push_back(std::make_pair<int, int>(42,42));
	m_fsrKeys["sensedac"] = v15;
	std::vector<std::pair<int, int> > v16;
	v16.push_back(std::make_pair<int, int>(43,43));
	m_fsrKeys["dacsel"] = v16;

	typedef std::pair<std::string, int> DefaultPair;
	static const DefaultPair DefaultCList[] = {
		DefaultPair("biasdelayn", 0),
		DefaultPair("biasdisc", 128),
		DefaultPair("biaspreamp", 60),
		DefaultPair("biassetdisc", 240),
		DefaultPair("biasths", 150),
		DefaultPair("biasikrum", 200),
		DefaultPair("biasabuffer", 255),
		DefaultPair("fbk", 180),
		DefaultPair("gnd", 128),
		DefaultPair("biaslvdstx", 128),
		DefaultPair("reflvdstx", 128),
		DefaultPair("thh", 0),
		DefaultPair("thl", 6600),
     };
	std::map<std::string, int> m_default(C_LIST_ITERS(DefaultCList));

	typedef std::pair<std::string, int> DacCodePair;
	static const DacCodePair DacCodeCList[] = {
		DacCodePair("biasdelayn", 0x1),
		DacCodePair("biasdisc", 0x2),
		DacCodePair("biaspreamp", 0x3),
		DacCodePair("biassetdisc", 0x4),
		DacCodePair("biasths", 0x5),
		DacCodePair("biasikrumhalf", 0x8),
		DacCodePair("biasikrum", 0x7),
		DacCodePair("biasabuffer", 0xe),
		DacCodePair("thh", 0xc),
		DacCodePair("thl", 0xb),
		DacCodePair("fbk", 0xa),
		DacCodePair("gnd", 0xd),
		DacCodePair("biaslvdstx", 0x6),
		DacCodePair("reflvdstx", 0x9),
     };
	std::map<std::string, int> m_dacCode(C_LIST_ITERS(DacCodeCList));
}

void MpxFsrDef::initMXR2() {
	DEB_MEMBER_FUNCT();
	std::vector<std::pair<int, int> > v1;
	v1.push_back(std::make_pair<int, int>(3,10));
	m_fsrKeys["ikrum"] = v1;
	std::vector<std::pair<int, int> > v2;
	v2.push_back(std::make_pair<int, int>(11,18));
	m_fsrKeys["disc"] = v2;
	std::vector<std::pair<int, int> > v3;
	v3.push_back(std::make_pair<int, int>(19,26));
	m_fsrKeys["preamp"] = v3;
	std::vector<std::pair<int, int> > v4;
	v4.push_back(std::make_pair<int, int>(37,38));
	v4.push_back(std::make_pair<int, int>(40,41));
	m_fsrKeys["daccode"] = v4;
	std::vector<std::pair<int, int> > v5;
	v5.push_back(std::make_pair<int, int>(42,42));
	m_fsrKeys["sensedac"] = v5;
	std::vector<std::pair<int, int> > v6;
	v6.push_back(std::make_pair<int, int>(43,43));
	m_fsrKeys["dacsel"] = v6;
	std::vector<std::pair<int, int> > v7;
	v7.push_back(std::make_pair<int, int>(45,48));
	v7.push_back(std::make_pair<int, int>(55,58));
	m_fsrKeys["buffanaloga"] = v7;
	std::vector<std::pair<int, int> > v8;
	v8.push_back(std::make_pair<int, int>(59,66));
	m_fsrKeys["buffanalogb"] = v8;
	std::vector<std::pair<int, int> > v9;
	v9.push_back(std::make_pair<int, int>(85,92));
	m_fsrKeys["delayn"] = v9;
	std::vector<std::pair<int, int> > v10;
	v10.push_back(std::make_pair<int, int>(99,112));
	m_fsrKeys["thl"] = v10;
	std::vector<std::pair<int, int> > v11;
	v11.push_back(std::make_pair<int, int>(113,126));
	m_fsrKeys["thh"] = v11;
	std::vector<std::pair<int, int> > v12;
	v12.push_back(std::make_pair<int, int>(127,134));
	m_fsrKeys["fbk"] = v12;
	std::vector<std::pair<int, int> > v13;
	v13.push_back(std::make_pair<int, int>(135,142));
	m_fsrKeys["gnd"] = v13;
	std::vector<std::pair<int, int> > v14;
	v14.push_back(std::make_pair<int, int>(143,174));
	m_fsrKeys["ctpr"] = v14;
	std::vector<std::pair<int, int> > v15;
	v15.push_back(std::make_pair<int, int>(180,187));
	m_fsrKeys["ths"] = v15;
	std::vector<std::pair<int, int> > v16;
	v16.push_back(std::make_pair<int, int>(226,233));
	m_fsrKeys["biaslvds"] = v16;
	std::vector<std::pair<int, int> > v17;
	v17.push_back(std::make_pair<int, int>(234,241));
	m_fsrKeys["reflvds"] = v17;

	typedef std::pair<std::string, int> DefaultPair;
	static const DefaultPair DefaultCList[] = {
		DefaultPair("ikrum", 200),
		DefaultPair("disc", 250),
		DefaultPair("preamp", 128),
		DefaultPair("buffanaloga", 128),
		DefaultPair("buffanalogb", 128),
		DefaultPair("delayn", 128),
		DefaultPair("fbk", 115),
		DefaultPair("gnd", 128),
		DefaultPair("ths", 180),
		DefaultPair("biaslvds", 128),
		DefaultPair("reflvds", 129),
		DefaultPair("thh", 0),
		DefaultPair("thl", 6600),
	};
	std::map<std::string, int> m_default(C_LIST_ITERS(DefaultCList));

	typedef std::pair<std::string, int> DacCodePair;
	static const DacCodePair DacCodeCList[] = {
		DacCodePair("ikrum", 0xf),
		DacCodePair("disc", 0xb),
		DacCodePair("preamp", 0x7),
		DacCodePair("buffanaloga", 0x3),
		DacCodePair("buffanalogb", 0x4),
		DacCodePair("delayn", 0x9),
		DacCodePair("thl", 0x6),
		DacCodePair("thh", 0xc),
		DacCodePair("fbk", 0xa),
		DacCodePair("gnd", 0xd),
		DacCodePair("ths", 0x1),
		DacCodePair("biaslvds", 0x2),
		DacCodePair("reflvds", 0xe),
	};
	std::map<std::string, int> m_dacCode(C_LIST_ITERS(DacCodeCList));
}
//#else
#if 0
	void MpxFsrDef::initDUMMY() {
	DEB_MEMBER_FUNCT();
	m_fsrKeys["testa"] = {{3,4}};
	m_fsrKeys["testb"] = {{8,15}};
	m_fsrKeys["testc"] = {{32,35},{44,47}};
}

void MpxFsrDef::initTPX1() {
	DEB_MEMBER_FUNCT();
	m_fsrKeys["ikrum"] = { {4,11}};
	m_fsrKeys["disc"] = { {12,19}};
	m_fsrKeys["preamp"] = { {20,27}};
	m_fsrKeys["daccode"] = { {38,39}, {41,42}};
	m_fsrKeys["sensedac"] = { {43,43}};
	m_fsrKeys["dacsel"] = { {44,44}};
	m_fsrKeys["buffanaloga"] = { {46,49}, {56,59}};
	m_fsrKeys["buffanalogb"] = { {60,67}};
	m_fsrKeys["hist"] = { {86,93}};
	m_fsrKeys["thl"] = { {100,113}};
	m_fsrKeys["vcas"] = { {120,127}};
	m_fsrKeys["fbk"] = { {128,135}};
	m_fsrKeys["gnd"] = { {136,143}};
	m_fsrKeys["ctpr"] = { {144,175}};
	m_fsrKeys["ths"] = { {181,188}};
	m_fsrKeys["biaslvds"] = { {227,234}};
	m_fsrKeys["reflvds"] = { {235,242}};

	m_default = {
		{	"ikrum", 200},
		{	"disc", 250},
		{	"preamp", 128},
		{	"buffanaloga", 128},
		{	"buffanalogb", 128},
		{	"hist", 128},
		{	"vcas", 128},
		{	"fbk", 115},
		{	"gnd", 128},
		{	"ths", 180},
		{	"biaslvds", 128},
		{	"reflvds", 129},
		{	"thl", 6600},
	};
	m_dacCode = {
		{	"ikrum", 0xf},
		{	"disc", 0xb},
		{	"preamp", 0x7},
		{	"buffanaloga", 0x3},
		{	"buffanalogb", 0x4},
		{	"hist", 0x9},
		{	"thl", 0x6},
		{	"vcas", 0xc},
		{	"fbk", 0xa},
		{	"gnd", 0xd},
		{	"ths", 0x1},
		{	"biaslvds", 0x2},
		{	"reflvds", 0xe},
	};
}

	void MpxFsrDef::initMPX2() {
	DEB_MEMBER_FUNCT();
	m_fsrKeys["biasdelayn"] = { {3,10}};
	m_fsrKeys["biasdisc"] = { {11,18}};
	m_fsrKeys["biaspreamp"] = { {19,26}};
	m_fsrKeys["biassetdisc"] = { {45,48}, {55,58}};
	m_fsrKeys["biasths"] = { {59,66}};
	m_fsrKeys["biasikrum"] = { {99,106}};
	m_fsrKeys["biasabuffer"] = { {107,114}};
	m_fsrKeys["thh"] = { {115,122}};
	m_fsrKeys["thl"] = { {123,130}};
	m_fsrKeys["fbk"] = { {131,138}};
	m_fsrKeys["gnd"] = { {180,187}};
	m_fsrKeys["biaslvdstx"] = { {226,233}};
	m_fsrKeys["reflvdstx"] = { {234,241}};
	m_fsrKeys["daccode"] = { {37,38}, {40,40}, {41,41}};
	m_fsrKeys["sensedac"] = { {42,42}};
	m_fsrKeys["dacsel"] = { {43,43}};

	m_default= {
		{	"biasdelayn", 0},
		{	"biasdisc", 128},
		{	"biaspreamp", 60},
		{	"biassetdisc", 240},
		{	"biasths", 150},
		{	"biasikrum", 200},
		{	"biasabuffer", 255},
		{	"fbk", 180},
		{	"gnd", 128},
		{	"biaslvdstx", 128},
		{	"reflvdstx", 128},
		{	"thh", 0},
		{	"thl", 6600},
	};
	m_dacCode= {
		{	"biasdelayn", 0x1},
		{	"biasdisc", 0x2},
		{	"biaspreamp", 0x3},
		{	"biassetdisc", 0x4},
		{	"biasths", 0x5},
		{	"biasikrumhalf", 0x8},
		{	"biasikrum", 0x7},
		{	"biasabuffer", 0xe},
		{	"thh", 0xc},
		{	"thl", 0xb},
		{	"fbk", 0xa},
		{	"gnd", 0xd},
		{	"biaslvdstx", 0x6},
		{	"reflvdstx", 0x9},
	};
}

	void MpxFsrDef::initMXR2() {
	DEB_MEMBER_FUNCT();
	m_fsrKeys["ikrum"] = { {3,10}};
	m_fsrKeys["disc"] = { {11,18}};
	m_fsrKeys["preamp"] = { {19,26}};
	m_fsrKeys["daccode"] = { {37,38}, {40,41}};
	m_fsrKeys["sensedac"] = { {42,42}};
	m_fsrKeys["dacsel"] = { {43,43}};
	m_fsrKeys["buffanaloga"] = { {45,48}, {55,58}};
	m_fsrKeys["buffanalogb"] = { {59,66}};
	m_fsrKeys["delayn"] = { {85, 92}};
	m_fsrKeys["thl"] = { {99, 112}};
	m_fsrKeys["thh"] = { {113, 126}};
	m_fsrKeys["fbk"] = { {127, 134}};
	m_fsrKeys["gnd"] = { {135, 142}};
	m_fsrKeys["ctpr"] = { {143, 174}};
	m_fsrKeys["ths"] = { {180, 187}};
	m_fsrKeys["biaslvds"] = { {226, 233}};
	m_fsrKeys["reflvds"] = { {234, 241}};

	m_default= {
		{	"ikrum", 200},
		{	"disc", 250},
		{	"preamp", 128},
		{	"buffanaloga", 128},
		{	"buffanalogb", 128},
		{	"delayn", 128},
		{	"fbk", 115},
		{	"gnd", 128},
		{	"ths", 180},
		{	"biaslvds", 128},
		{	"reflvds", 129},
		{	"thh", 0},
		{	"thl", 6600},
	};
	m_dacCode = {
		{	"ikrum", 0xf},
		{	"disc", 0xb},
		{	"preamp", 0x7},
		{	"buffanaloga", 0x3},
		{	"buffanalogb", 0x4},
		{	"delayn", 0x9},
		{	"thl", 0x6},
		{	"thh", 0xc},
		{	"fbk", 0xa},
		{	"gnd", 0xd},
		{	"ths", 0x1},
		{	"biaslvds", 0x2},
		{	"reflvds", 0xe},
	};
}
#endif

MpxChipDacs::MpxChipDacs(Version version) :
		m_version(version) {
	reset();
}

MpxChipDacs::~MpxChipDacs() {
}

void MpxChipDacs::reset() {
	DEB_MEMBER_FUNCT();
	std::map<std::string, int> vec = m_data;
	for (std::map<std::string, int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		m_data[it->first] = 0;
	}
	// update
	vec = MpxFsrDef::getInstance(m_version)->getDefault();
	for (std::map<std::string, int>::iterator it = vec.begin(); it != vec.end(); ++it) {
		m_data[it->first] = it->second;
	}
}

int MpxChipDacs::getOneDac(std::string& name) {
	DEB_MEMBER_FUNCT();
	std::string lower = name;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	std::map<std::string, int> vec = m_data;
	for (std::map<std::string, int>::iterator it = m_data.begin(); it != m_data.end(); ++it) {
		if (it->first == lower) {
			return it->second;
		}
	}
	return -1;
}

void MpxChipDacs::setOneDac(std::string& name, int value) {
	DEB_MEMBER_FUNCT();
	setValue(name, value);
}

void MpxChipDacs::getDacs(std::map<std::string, int>& dacs) {
	DEB_MEMBER_FUNCT();
	dacs = m_data;
}

void MpxChipDacs::setDacs(std::map<std::string, int>& dacs) {
	DEB_MEMBER_FUNCT();
	for (std::map<std::string, int>::iterator it = dacs.begin(); it != dacs.end(); ++it) {
		std::string name = it->first;
		int value = it->second;
		setValue(name, value);
	}
}

std::vector<std::string> MpxChipDacs::getListKeys() {
        return MpxFsrDef::getInstance(m_version)->listKeys(false);
}

void MpxChipDacs::setValue(std::string& name, int value) {
	DEB_MEMBER_FUNCT();
	std::string lower = name;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	std::vector<std::string> v = MpxFsrDef::getInstance(m_version)->listKeys(false);
	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
		if (*it == lower) {
			if (lower == "daccode") {
				m_data[lower] = dacCode(value);
			} else {
				m_data[lower] = value;
			}
			return;
		}
	}
	THROW_HW_ERROR(Error) << "<" << name << "> is not a valid dac parameter";
}

int MpxChipDacs::dacCode(std::string& code) {
	DEB_MEMBER_FUNCT();
	std::map<std::string, int> dacs = MpxFsrDef::getInstance(m_version)->dacCode();
	std::string name = code;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	for (std::map<std::string, int>::iterator it = dacs.begin(); it != dacs.end(); ++it) {
		if (name == it->first) {
			return dacs[name];
		}
	}
	THROW_HW_ERROR(Error) << "<" << code << "> is not a valid dac name";
}

int MpxChipDacs::dacCode(int code) {
	DEB_MEMBER_FUNCT();
	std::map<std::string, int> dacs = MpxFsrDef::getInstance(m_version)->dacCode();
	for (std::map<std::string, int>::iterator it = dacs.begin(); it != dacs.end(); ++it) {
		if (code == it->second) {
			return code;
		}
	}
	THROW_HW_ERROR(Error) << "<" << code << "> is not a valid dac code";
}

void MpxChipDacs::getFsrString(std::string& fsrString) {
	DEB_MEMBER_FUNCT();
	std::vector<std::string> fsrkeys = MpxFsrDef::getInstance(m_version)->listKeys();
	int fsrInt[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (std::map<std::string, int>::iterator it = m_data.begin(); it != m_data.end(); ++it) {
		int base = 0;
		std::string key = it->first;
		int val = it->second;
		std::vector<std::pair<int, int> > fsrValues = MpxFsrDef::getInstance(m_version)->fsrValues(key);
		for (std::vector<std::pair<int, int> >::iterator it = fsrValues.begin(); it != fsrValues.end(); ++it) {
			std::pair<int, int> p = *it;
			int lsb = p.first;
			int msb = p.second;
			for (int idx = 0; idx < msb - lsb + 1; idx++) {
				if (val & (1 << (base + idx))) {
					int index = 31 - (lsb + idx) / 8;
					int shift = ((lsb + idx) % 8);
					fsrInt[index] |= 1 << shift;
				}
			}
			base += msb - lsb + 1;
		}
	}
	std::stringstream ss;
	for (int idx = 0; idx < 32; idx++) {
		char c = fsrInt[idx];
		ss << c;
	}
	fsrString = ss.str();
	DEB_TRACE() << ss.str();
}

MpxDacs::MpxDacs(Version version, int nchip) :
		m_version(version), m_nchip(nchip) {
	m_priamPorts = NULL;
	m_thlNoise = std::map<int, int>();
	m_thlXray = std::map<int, int>();
	m_chipDacs = std::vector<MpxChipDacs*>();
	reset();
}

MpxDacs::~MpxDacs() {
	for (int idx = 0; idx < m_nchip; idx++) {
	  delete m_chipDacs[idx];
	}
}

void MpxDacs::reset() {
	DEB_MEMBER_FUNCT();
	m_chipDacs.clear();
	m_thlNoise.clear();
	m_thlXray.clear();
	m_energyCalib = 0;
	m_lastEnergy = 0;

	for (int idx = 0; idx < m_nchip; idx++) {
		m_chipDacs.push_back(new MpxChipDacs(m_version));
		m_thlNoise[idx] = 0;
		m_thlXray[idx] = 0;
	}
}

void MpxDacs::setPriamPars(PriamAcq* priamAcq, std::vector<int>* priamPorts) {
	DEB_MEMBER_FUNCT();
	m_pacq = priamAcq;
	m_priamPorts = priamPorts;
}

void MpxDacs::applyChipDacs(int chipid) {
	DEB_MEMBER_FUNCT();
	if (m_pacq == NULL || m_priamPorts == NULL)
		THROW_HW_ERROR(Error) << "Call first setPriamPars() first !";

	std::string sfsr;
	if (chipid == 0) {
		for (int idx = 0; idx < m_nchip; idx++) {
			getFsrString(idx + 1, sfsr);
			std::cout << "Loading Chip FSR #" << idx+1 << " ..." << std::endl;
			m_pacq->setChipFsr((*m_priamPorts)[idx], sfsr);
		}
	} else {
		int port = (*m_priamPorts)[chipid - 1];
		getFsrString(chipid, sfsr);
		std::cout << "Loading Chip FSR #" << chipid << " ..." << std::endl;
		m_pacq->setChipFsr(port, sfsr);
		// need to wait for FSR transfer at least for the last
		// chip otherwise an immediate exposure could make image
		// in bad shape
		sleep(100);
	}
}

void MpxDacs::getFsrString(int chipid, std::string& fsrString) {
	DEB_MEMBER_FUNCT();
	std::pair<int, int> p = getChipIdx(chipid);
	m_chipDacs[p.first]->getFsrString(fsrString);
}

void MpxDacs::setThlNoise(std::map<int, int>& values) {
	DEB_MEMBER_FUNCT();
	m_thlNoise = values;
}

void MpxDacs::getThlNoise(std::map<int, int>& noise) {
	DEB_MEMBER_FUNCT();
	noise = m_thlNoise;
}

void MpxDacs::setThlXray(std::map<int, int>& values) {
	DEB_MEMBER_FUNCT();
	m_thlXray = values;
}

void MpxDacs::getThlXray(std::map<int, int>& threshold) {
	DEB_MEMBER_FUNCT();
	threshold = m_thlXray;
}

void MpxDacs::setEnergyCalibration(double energy) {
	DEB_MEMBER_FUNCT();
	m_energyCalib = energy;
}

void MpxDacs::getEnergyCalibration(double& energy) {
	DEB_MEMBER_FUNCT();
	energy = m_energyCalib;
}

void MpxDacs::setEnergy(double energy) {
	DEB_MEMBER_FUNCT();
	for (int idx = 0; idx < m_nchip; idx++) {
		double val = m_thlNoise[idx] + ((m_thlXray[idx] - m_thlNoise[idx]) * energy / m_energyCalib);
		std::string name = "thl";
		m_chipDacs[idx]->setOneDac(name, int(val));
		DEB_TRACE() << " thl #" << idx << " = " << val;
	}
	m_lastEnergy = energy;
}

void MpxDacs::getEnergy(double& energy) {
	DEB_MEMBER_FUNCT();
	energy = m_lastEnergy;
}

void MpxDacs::setOneDac(int chipid, std::string name, int value) {
	DEB_MEMBER_FUNCT();
	std::pair<int, int> p = getChipIdx(chipid);
	for (int idx = p.first; idx < p.second; idx++) {
		m_chipDacs[idx]->setOneDac(name, value);
	}
}

void MpxDacs::setDacs(int chipid, std::map<std::string, int>& dacs) {
	DEB_MEMBER_FUNCT();
	std::pair<int, int> p = getChipIdx(chipid);
	for (int idx = p.first; idx < p.second; idx++) {
		m_chipDacs[idx]->setDacs(dacs);
	}
}

void MpxDacs::getOneDac(int chipid, std::string name, int& value) {
	DEB_MEMBER_FUNCT();
	std::vector<int> dacs;
	std::pair<int, int> p = getChipIdx(chipid);
	for (int idx = p.first; idx < p.second; idx++) {
		dacs.push_back(m_chipDacs[idx]->getOneDac(name));
	}
	int res = dacs[0];
	value = -1;
	if (chipid == 0 && m_nchip > 1) {
		for (int idx = 1; idx < m_nchip; idx++) {
			if (dacs[idx] != res) {
				return;
			}
		}
	}
	value = dacs[0];
}

void MpxDacs::getDacs(int chipid, std::map<std::string, int>& res) {
	DEB_MEMBER_FUNCT();
	std::pair<int, int> p = getChipIdx(chipid);
	std::vector<std::map<std::string, int> > dacs;
	for (int idx = p.first; idx < p.second; idx++) {
		std::map<std::string, int> chipDac;
		m_chipDacs[idx]->getDacs(chipDac);
		dacs.push_back(chipDac);
	}
	res = dacs[0];
	if (chipid == 0 && m_nchip > 1) {
		for (std::map<std::string, int>::iterator it = dacs[0].begin(); it != dacs[0].end(); ++it) {
			std::string key = it->first;
			int val = it->second;
			for (int idx = 1; idx < m_nchip; idx++) {
				if (dacs[idx][key] != val) {
					res.erase(key);
				}
			}
		}
	}
}

std::pair<int, int> MpxDacs::getChipIdx(int chipid) {
	DEB_MEMBER_FUNCT();
	if (chipid == 0) {
		return std::pair<int, int>(0, m_nchip);
	} else if (chipid < 1 || chipid > m_nchip) {
		THROW_HW_ERROR(Error) << "Invalid chipid <" << chipid << ">. Range is [1," << m_nchip << "]";
	}
	return std::pair<int, int>(chipid - 1, chipid);
}
