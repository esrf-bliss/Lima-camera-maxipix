//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2015
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#ifndef MPXDACS_H
#define MPXDACS_H

#include <map>
#include "lima/Debug.h"
#include "lima/Exceptions.h"
#include "lima/Constants.h"
#include "lima/HwInterface.h"
#include "MpxVersion.h"
#include "MpxVersion.h"
#include "PriamAcq.h"

namespace lima {
namespace Maxipix {

class MpxFsrDef {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Maxipix");
public:
	static MpxFsrDef* getInstance(Version version);
	std::vector<std::string> listKeys(bool saved = true);
	std::vector<std::pair<int,int> >&  fsrValues(std::string key);
	std::map<std::string, int>& dacCode();
	std::map<std::string, int>& getDefault();

private:
	MpxFsrDef(Version version);
	MpxFsrDef(MpxFsrDef const&);      // Don't Implement
	void operator=(MpxFsrDef const&); // Don't implement
	static MpxFsrDef* m_instance;

//	Version& m_version;
	std::map<std::string, std::vector<std::pair<int,int> > > m_fsrKeys;
	std::map<std::string, int> m_dacCode;
	std::map<std::string, int> m_default;

	void initDUMMY();
	void initMPX2();
	void initMXR2();
	void initTPX1();
};

class MpxChipDacs {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Maxipix");
public:

	MpxChipDacs(Version version);
	~MpxChipDacs();
	void reset();
	int getOneDac(std::string& name);
	void setOneDac(std::string& name, int value);
	void getDacs(std::map<std::string, int>& dacs);
	void setDacs(std::map<std::string, int>& dacs);
	std::vector<std::string> getListKeys();
	int dacCode(std::string& code);
	int dacCode(int code);
	void getFsrString(std::string&);

private:
	MpxChipDacs(const MpxChipDacs&);
	MpxChipDacs& operator=(const MpxChipDacs&);

	Version m_version;
	std::map<std::string,int> m_data;

	void setValue(std::string& name, int value);
};

 typedef  std::map<std::string, int> DacCodeMapType;
 typedef  std::map<int, int> ThlNoiseMapType;
 
class MpxDacs {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "MpxDacs");
public:

	MpxDacs(Version version, int nchip);
	~MpxDacs();
	void reset();
	void setPriamPars(PriamAcq* priamAcq, std::vector<int>* priamPorts);
	void applyChipDacs(int chipid);
	void getFsrString(int chipid, std::string& fsrString);

	void setThlNoise(ThlNoiseMapType& noise);
	void getThlNoise(ThlNoiseMapType& noise);
	void setThlXray(ThlNoiseMapType& threshold);
	void getThlXray(ThlNoiseMapType& threshold);
	void setEnergyCalibration(double energy);
	void getEnergyCalibration(double& energy);
	void setEnergy(double energy);
	void getEnergy(double& energy);

	void setOneDac(int chipid, std::string name, int value);
	void setDacs(int chipid, DacCodeMapType& dacs);
	void getOneDac(int chipid, std::string name, int& value);
	void getDacs(int chipid, DacCodeMapType& dacs);
	std::vector<std::string> getListKeys();
	
private:
	Version& m_version;
	int m_nchip;
	PriamAcq* m_pacq;
	std::vector<int>* m_priamPorts;
	double m_lastEnergy;
	double m_energyCalib;
	ThlNoiseMapType m_thlNoise;
	ThlNoiseMapType m_thlXray;
	std::vector<MpxChipDacs*> m_chipDacs;

	std::pair<int,int> getChipIdx(int chipid);
};

} // namespace Maxipix
} // namespace lima

#endif // MPXDACS_H
