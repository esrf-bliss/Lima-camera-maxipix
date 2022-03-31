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

namespace Maxipix
{

class Camera {
%TypeHeaderCode
#include "MaxipixCamera.h"
#include "MpxVersion.h"
#include "PriamAcq.h"
%End

public:


	Camera(int espia_dev_nb, const std::string config_path, const std::string config_name, bool reconstruction);
	~Camera();

	void reset(HwInterface::ResetLevel reset_level);
	void prepareAcq();
	void startAcq();
	void stopAcq();
	int getNbHwAcquiredFrames();

	void getTrigMode(TrigMode &mode /Out/);
	void setTrigMode(TrigMode mode);
	bool checkTrigMode(TrigMode mode, bool accumulationMode);

	void setExpTime(double  exp_time);
    	void getExpTime(double& exp_time /Out/);
    	void setLatTime(double  lat_time);
    	void getLatTime(double& lat_time /Out/);

    	void setNbHwFrames(int  nb_frames);
    	void getNbHwFrames(int& nb_frames /Out/);
    	void getValidRanges(HwSyncCtrlObj::ValidRangesType& valid_ranges /Out/);

	void getImageSize(Size& size /Out/);
	void getPixelSize(double& x_size /Out/, double& y_size /Out/);
	void getImageType(ImageType& type /Out/);

	void getDetectorType(std::string& type /Out/);
	void getDetectorModel(std::string& model /Out/);

	void setShutterMode(ShutterMode shutter_mode);
	void getShutterMode(ShutterMode& shutter_mode /Out/) const;
	void setShutterOpenTime(double shutter_open_time);
	void getShutterOpenTime(double& shutter_open_time) const;
	void setShutterCloseTime(double shutter_close_time);
	void getShutterCloseTime(double& shutter_close_time) const;

	void getStatus(DetStatus& status /Out/);
	bool isAcqRunning();
	void setAcqMode(AcqMode mode);

	// Maxipix specific

	void getFillMode(Maxipix::MaxipixReconstruction::Type& type /Out/) const;
	void setFillMode(Maxipix::MaxipixReconstruction::Type type);
	void setPath(const std::string& path);
	void loadConfig(const std::string& name, bool reconstruction=true);

	void setEnergy(double energy);
	void getEnergy(double& energy /Out/);

	Maxipix::PriamAcq* priamAcq();
	Maxipix::MpxDacs*  dacs();

private:
	Camera(const Maxipix::Camera&);
};

};
