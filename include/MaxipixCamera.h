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
#ifndef MAXIPIXCAMERA_H
#define MAXIPIXCAMERA_H

#include <iostream>
#include "lima/Debug.h"
#include "lima/Exceptions.h"
#include "lima/HwMaxImageSizeCallback.h"
#include "lima/HwSyncCtrlObj.h"
#include "lima/HwInterface.h"
#include "lima/HwBufferMgr.h"
#include "lima/SizeUtils.h"
#include "EspiaDev.h"
#include "EspiaSerialLine.h"
#include "EspiaAcq.h"
#include "EspiaBufferMgr.h"
#include "PriamSerial.h"
#include "PriamAcq.h"
#include "MaxipixBufferCtrlObj.h"
#include "MaxipixReconstruction.h"
#include "MpxDetConfig.h"
#include "MpxChipConfig.h"
#include "MpxVersion.h"

namespace lima {
namespace Maxipix {

static const double PixelSize = 55e-6;
static const int MaxChips = 5;

class PriamAcq;

class Camera : public HwMaxImageSizeCallbackGen {
DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Maxipix");
public:


	Camera(int espia_dev_nb, const std::string& config_path, const std::string& config_name, bool reconstruction = false);
	~Camera();

	void reset(HwInterface::ResetLevel reset_level);
	void prepareAcq();
	void startAcq();
	void stopAcq();
	int getNbHwAcquiredFrames();

	void getTrigMode(TrigMode &mode);
	void setTrigMode(TrigMode mode);
	bool checkTrigMode(TrigMode mode, bool accumulationMode);

	void setExpTime(double  exp_time);
	void getExpTime(double& exp_time);
	void setLatTime(double  lat_time);
	void getLatTime(double& lat_time);
	
	void setNbHwFrames(int  nb_frames);
	void getNbHwFrames(int& nb_frames);
	void getValidRanges(HwSyncCtrlObj::ValidRangesType& valid_ranges);

	void getImageSize(Size& size);
	void getPixelSize(double& x_size, double& y_size);
	void getImageType(ImageType& type);

	void getDetectorType(std::string& type);
	void getDetectorModel(std::string& model);

	void setShutterMode(ShutterMode shutter_mode);
	void getShutterMode(ShutterMode& shutter_mode) const;
	void setShutterOpenTime(double shutter_open_time);
	void getShutterOpenTime(double& shutter_open_time) const;
	void setShutterCloseTime(double shutter_close_time);
	void getShutterCloseTime(double& shutter_close_time) const;

	void getStatus(DetStatus& status);
	bool isAcqRunning();
	void setAcqMode(AcqMode mode) {m_acqMode = mode;}

	// Maxipix specific

	void setPath(const std::string& path);
	void loadConfig(const std::string& name, bool reconstruction = true);

	void getFillMode(MaxipixReconstruction::Type& type) const;
	void setFillMode(MaxipixReconstruction::Type type);

	void setEnergy(double energy) {m_mpxDacs->setEnergy(energy);m_mpxDacs->applyChipDacs(0); }
	void getEnergy(double& energy){m_mpxDacs->getEnergy(energy); }

	PriamAcq* priamAcq() {return &m_priamAcq; }
        MpxDacs* dacs() {return m_mpxDacs;}
	
	MaxipixReconstruction* getReconstructionTask(){return m_reconstructionTask;};


	
	// Buffer control object
	HwBufferCtrlObj* getBufferCtrlObj();

protected:
	virtual void setMaxImageSizeCallbackActive(bool cb_active);

private:
	void setVersion(Version version);
	void setNbChip(int xchip, int ychip);
	void setPixelGap(int xgap, int ygap);
	void setChipsLayout(const MaxipixReconstruction::Layout& layout);

	void loadDetConfig(const std::string& name, bool reconstruction);
	void setReconstructionActive(bool active);
	void loadChipConfig(const std::string& name);
	void applyPixelConfig(int chipid);
	MaxipixReconstruction* createReconstructionTask();


	class AcqEndCallback: public Espia::AcqEndCallback {
	DEB_CLASS_NAMESPC(DebModCamera, "Camera::AcqEndCallback",
			"Maxipix");

	public:
		AcqEndCallback(Camera& cam);
		virtual ~AcqEndCallback();

	protected:
		virtual void acqFinished(const HwFrameInfoType& /*finfo*/);
	private:
		Camera& m_cam;
	};

	Espia::Dev m_edev;
	Espia::SerialLine m_eser;
	Espia::Acq m_espiaAcq;
	Espia::BufferMgr m_ebuf;
	PriamSerial m_priamSerial;
	PriamAcq m_priamAcq;
	std::string m_cfgName;;
	int m_xchips;
	int m_ychips;
	int m_xgap;
	int m_ygap;
	ImageType m_type;
	Version m_version;
	bool m_mis_cb_act;
	bool m_reconstruction;
	MaxipixReconstruction::Layout m_layout;
	AcqEndCallback m_acq_end_cb;
	std::string m_cfgPath;
	int m_nchips;
	Size m_size;
	std::string m_cfgFilename;
	std::vector<int> m_priamPorts;
	bool m_prepare_flag;
	AcqMode m_acqMode;

	MaxipixReconstruction::PositionList m_positions;
	MaxipixReconstruction::Type m_reconstructType;
	MaxipixReconstruction* m_reconstructionTask;

	// Buffer management
	BufferCtrlMgr m_bufferCtrlMgr;
	BufferCtrlObj m_bufferCtrlObj;

	MpxPixelConfig* m_chipCfg;
	MpxDacs* m_mpxDacs;

	void init();
	void acqLoadConfig(const std::string& name, bool reconstruction);
	int getPriamPort(int chipid);
};

} // namespace Maxipix
} // namespace lima

#endif // MAXIPIXCAMERA_H
