//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
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

namespace Maxipix
{

class Interface : HwInterface
{
%TypeHeaderCode

#include "lima/HwInterface.h"
#include "EspiaBufferMgr.h"
#include "MaxipixInterface.h"
#include "MaxipixCamera.h"
#include "PriamAcq.h"
%End
 public:
	Interface(Maxipix::Camera& cam /KeepReference/);
	virtual ~Interface();

	virtual void getCapList(std::vector<HwCap> &cap_list /Out/) const;

	virtual void reset(HwInterface::ResetLevel reset_level);
	virtual void prepareAcq();
	virtual void startAcq();
	virtual void stopAcq();
	virtual void getStatus(HwInterface::StatusType& status /Out/);
	virtual int getNbHwAcquiredFrames();

	void getFillMode(Maxipix::MaxipixReconstruction::Type& type /Out/);
	void setFillMode(Maxipix::MaxipixReconstruction::Type type);
	void setPath(const std::string& path);
	void loadConfig(const std::string& name, bool reconstruction=true);

	void setEnergy(double energy);
	void getEnergy(double& energy /Out/);

	Maxipix::PriamAcq* priamAcq();

 private:
	Interface(const Maxipix::Interface&);
};

}; // namespace Maxipix

