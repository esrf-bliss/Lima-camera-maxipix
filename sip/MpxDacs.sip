//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2019
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


%MappedType std::map<std::string, int>
{
%TypeHeaderCode
#include <map>
#include <string>
#include "MpxDacs.h"
%End

%ConvertToTypeCode
PyObject *key, *value;
SIP_SSIZE_T pos;

if(sipIsErr == NULL)
{
  bool aReturnFlag = PyDict_Check(sipPy);

  pos = 0;
  while(aReturnFlag && PyDict_Next(sipPy, &pos, &key, &value))
     aReturnFlag = PyInt_Check(value) && sipCheckConvertFromPyStr(key);

  return aReturnFlag;
}

*sipCppPtr = new std::map<std::string, int>();
pos = 0;
while(PyDict_Next(sipPy, &pos, &key, &value))
{
  int anIndex = PyInt_AS_LONG(value);
  std::pair<std::string, int> item(sipConvertFromPyStr(key), anIndex);
  (*sipCppPtr)->insert(item);
}
return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
PyObject* aReturnDictPt = PyDict_New();

std::map<std::string, int>::iterator mIter, mEnd = sipCpp->end();
for(mIter = sipCpp->begin(); mIter != mEnd; ++mIter)
   {
    PyObject *aKeyPt = sipConvertToPyStr(mIter->first);
    PyObject *aValuePt = PyInt_FromLong(mIter->second);
    PyDict_SetItem(aReturnDictPt,aKeyPt,aValuePt);
    Py_DECREF(aKeyPt);Py_DECREF(aValuePt);
   }
return aReturnDictPt;
%End
};


%MappedType std::map<int, int>
{
%TypeHeaderCode
#include <map>
#include "MpxDacs.h"
%End

%ConvertToTypeCode
PyObject *key, *value;
SIP_SSIZE_T pos;

if(sipIsErr == NULL)
{
  bool aReturnFlag = PyDict_Check(sipPy);

  pos = 0;
  while(aReturnFlag && PyDict_Next(sipPy, &pos, &key, &value))
     aReturnFlag = PyInt_Check(value) && PyInt_Check(key);

  return aReturnFlag;
}

*sipCppPtr = new std::map<int, int>();
pos = 0;
while(PyDict_Next(sipPy, &pos, &key, &value))
{
  int aValue = PyInt_AS_LONG(value);
  int aKey = PyInt_AS_LONG(key);
  std::pair<int, int> item(aKey, aValue);
  (*sipCppPtr)->insert(item);
}
return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
PyObject* aReturnDictPt = PyDict_New();

std::map<int, int>::iterator mIter, mEnd = sipCpp->end();
for(mIter = sipCpp->begin(); mIter != mEnd; ++mIter)
   {
    PyObject *aKeyPt = PyInt_FromLong(mIter->first);
    PyObject *aValuePt = PyInt_FromLong(mIter->second);
    PyDict_SetItem(aReturnDictPt,aKeyPt,aValuePt);
    Py_DECREF(aKeyPt);Py_DECREF(aValuePt);
   }
return aReturnDictPt;
%End
};

namespace Maxipix {

%TypeHeaderCode
#include <string>
#include <vector>
#include "MpxCommon.h"
#include "MpxDacs.h"
#include "PriamAcq.h"
#include "MpxVersion.h"
#include "lima/Constants.h"

using namespace lima;
%End

class MpxDacs {

public:
	MpxDacs(lima::Maxipix::Version version, int nchip);
	~MpxDacs();
	void reset();
	void setPriamPars(Maxipix::PriamAcq* priamAcq, std::vector<int>* priamPorts);
	void applyChipDacs(int chipid);
	void getFsrString(int chipid, std::string& fsrString /Out/);

	void setThlNoise(std::map<int,int>& noise);
	void getThlNoise(std::map<int,int>& noise /Out/);
	void setThlXray(std::map<int,int>& threshold);
	void getThlXray(std::map<int,int>& threshold /Out/);
	void setEnergyCalibration(double energy);
	void getEnergyCalibration(double& energy /Out/);
	void setEnergy(double energy);
	void getEnergy(double& energy /Out/);

	void setOneDac(int chipid, std::string name, int value);
	void setDacs(int chipid, std::map<std::string, int>& dacs /Out/);
	void getOneDac(int chipid, std::string name, int& value);
	void getDacs(int chipid, std::map<std::string, int>& dacs /Out/);
	std::vector<std::string> getListKeys();
};

};