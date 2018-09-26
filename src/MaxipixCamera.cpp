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
#include <iostream>
#include <sstream>
#include "lima/Debug.h"
#include "lima/Exceptions.h"
#include "MpxCommon.h"
#include "MaxipixCamera.h"

using namespace lima;
using namespace lima::Maxipix;

Camera::Camera(int espia_dev_nb, const std::string& config_path,
	       const std::string& config_name, bool reconstruction) :
		m_edev(Espia::Dev(espia_dev_nb)),
		m_eser(Espia::SerialLine(m_edev)),
		m_espiaAcq(Espia::Acq(m_edev)),
		m_ebuf(Espia::BufferMgr(m_espiaAcq)),
		m_priamSerial(PriamSerial(m_eser)),
		m_priamAcq(PriamAcq(m_priamSerial)),
		m_cfgName(config_name),
		m_xchips(0),
		m_ychips(0),
		m_xgap(0),
		m_ygap(0),
		m_type(Bpp16),
		m_version(MXR2),
		m_mis_cb_act(false),
		m_reconstruction(reconstruction),
		m_layout(MaxipixReconstruction::L_NONE),
		m_acq_end_cb(*this),
		m_cfgPath(config_path),
		m_bufferCtrlMgr(m_ebuf),
		m_bufferCtrlObj(m_bufferCtrlMgr) {

	DEB_CONSTRUCTOR();
	m_reconstructionTask = NULL;
	m_reconstructType = MaxipixReconstruction::RAW;
	m_mpxDacs = NULL;
	m_chipCfg = NULL;
	init();
}

Camera::~Camera() {
	DEB_DESTRUCTOR();
	delete m_chipCfg;
	delete m_mpxDacs;
	delete m_reconstructionTask;
}

void Camera::reset(HwInterface::ResetLevel reset_level) {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(reset_level);

	stopAcq();

	if (reset_level == HwInterface::HardReset) {
		DEB_ALWAYS() << "Performing chip hard reset";
		m_priamAcq.resetAllChip();
	}
	m_priamAcq.resetAllFifo();
}

void Camera::prepareAcq() {
	DEB_MEMBER_FUNCT();
	m_prepare_flag = true;
}

void Camera::startAcq() {
	DEB_MEMBER_FUNCT();
	if (m_prepare_flag || m_acqMode == Accumulation) {
		m_bufferCtrlMgr.setStartTimestamp(Timestamp::now());
		m_espiaAcq.start();
		m_prepare_flag = false;
	}
	m_priamAcq.startAcq();
}

void Camera::stopAcq() {
	DEB_MEMBER_FUNCT();
	m_priamAcq.stopAcq();
	m_espiaAcq.stop();
	m_prepare_flag = false;
}

int Camera::getNbHwAcquiredFrames() {
	DEB_MEMBER_FUNCT();
	Espia::Acq::Status acq_status;
	m_espiaAcq.getStatus(acq_status);
	int nb_hw_acq_frames = acq_status.last_frame_nb + 1;
	DEB_RETURN() << DEB_VAR1(nb_hw_acq_frames);
	return nb_hw_acq_frames;
}

bool Camera::checkTrigMode(TrigMode trig_mode, bool accumulation_mode) {
	DEB_MEMBER_FUNCT();
	bool valid_mode;
	switch (trig_mode) {
	case IntTrig:
	case IntTrigMult:
	case ExtTrigSingle:
	case ExtTrigMult:
	case ExtGate:
		valid_mode = true;
		break;
	default:
		valid_mode = false;
	}
	return valid_mode;
}

void Camera::setTrigMode(TrigMode trig_mode) {
	DEB_MEMBER_FUNCT();
	m_priamAcq.setTriggerMode(trig_mode);
}

void Camera::getTrigMode(TrigMode& trig_mode) {
	DEB_MEMBER_FUNCT();
	m_priamAcq.getTriggerMode(trig_mode);
}

void Camera::setExpTime(double exp_time) {
	DEB_MEMBER_FUNCT();
	double set_time;
	m_priamAcq.setExposureTime(exp_time, set_time);
}
void Camera::getExpTime(double& exp_time) {
	DEB_MEMBER_FUNCT();
	m_priamAcq.getExposureTime(exp_time);
}

void Camera::setLatTime(double lat_time) {
	DEB_MEMBER_FUNCT();
	double set_time;
	m_priamAcq.setIntervalTime(lat_time, set_time);
}
void Camera::getLatTime(double& lat_time) {
	DEB_MEMBER_FUNCT();
	m_priamAcq.getIntervalTime(lat_time);
}

void Camera::setNbHwFrames(int nb_frames) {
	DEB_MEMBER_FUNCT();
	m_espiaAcq.setNbFrames(nb_frames);

	int priamNbFrames = nb_frames;
	TrigMode trig_mode;
	m_priamAcq.getTriggerMode(trig_mode);

	if (trig_mode == IntTrigMult && m_acqMode != Accumulation)
		priamNbFrames = 1;

	m_priamAcq.setNbFrames(priamNbFrames);
}

void Camera::getNbHwFrames(int& nb_frames) {
	DEB_MEMBER_FUNCT();
	m_espiaAcq.getNbFrames(nb_frames);
}

void Camera::getValidRanges(HwSyncCtrlObj::ValidRangesType& valid_ranges) {
	DEB_MEMBER_FUNCT();
	double tmin, tmax;
	m_priamAcq.getExposureTimeRange(tmin, tmax);
	valid_ranges.min_exp_time = tmin;
	valid_ranges.max_exp_time = tmax;
	m_priamAcq.getIntervalTimeRange(tmin, tmax);
	valid_ranges.min_lat_time = tmin;
	valid_ranges.max_lat_time = tmax;
	DEB_RETURN() << DEB_VAR2(valid_ranges.min_exp_time, valid_ranges.max_exp_time);
	DEB_RETURN() << DEB_VAR2(valid_ranges.min_lat_time, valid_ranges.max_lat_time);
}

void Camera::getImageSize(Size& size) {
	DEB_MEMBER_FUNCT();
	size = m_size;
}

void Camera::getPixelSize(double& x_size, double& y_size) {
	DEB_MEMBER_FUNCT();
	x_size = y_size = PixelSize;
}

void Camera::getImageType(ImageType& type) {
	DEB_MEMBER_FUNCT();
	type = m_type;
}

void Camera::getDetectorType(std::string& type) {
	DEB_MEMBER_FUNCT();
	std::ostringstream os;
	os << "MAXIPIX";
	type = os.str();
}

void Camera::getDetectorModel(std::string& type) {
	DEB_MEMBER_FUNCT();
	std::stringstream ss;

	DEB_TRACE() << DEB_VAR1(m_layout);

	switch (m_layout) {
	case MaxipixReconstruction::L_NONE:
		ss << m_xchips << "x" << 1 << "(flatten out)-";
		break;
	case MaxipixReconstruction::L_2x2:
	case MaxipixReconstruction::L_5x1:
		ss << m_xchips << "x" << m_ychips << "(gap:" << m_xgap << "x" << m_ygap << ")-";
		break;
	case MaxipixReconstruction::L_FREE:
		ss << m_nchips << "x" << 1 << " chip(s) with rotation (FREE layout)-";
		break;
	case MaxipixReconstruction::L_GENERAL:
		ss << m_nchips << " chip(s) with position(GENERAL layout)-";
		break;
	}
	std::string version = convert_2_string(m_version);
	ss << version;
	type = ss.str();
}

void Camera::setShutterMode(ShutterMode shut_mode) {
	DEB_MEMBER_FUNCT();
	PriamAcq::ShutterMode cam_mode;
	cam_mode = (shut_mode == ShutterAutoFrame) ? PriamAcq::FRAME : PriamAcq::SEQUENCE;
	m_priamAcq.setShutterMode(cam_mode);
}

void Camera::getShutterMode(ShutterMode& shut_mode) const {
	DEB_MEMBER_FUNCT();

	PriamAcq::ShutterMode cam_mode;
	m_priamAcq.getShutterMode(cam_mode);
	shut_mode = (cam_mode == PriamAcq::FRAME) ? ShutterAutoFrame : ShutterAutoSequence;
	DEB_RETURN() << DEB_VAR1(shut_mode);

}

void Camera::setShutterOpenTime(double shut_open_time) {
	DEB_MEMBER_FUNCT();
	double settime;

	m_priamAcq.setShutterTime(shut_open_time, settime);
}
void Camera::getShutterOpenTime(double& shut_open_time) const {
	DEB_MEMBER_FUNCT();
	m_priamAcq.getShutterTime(shut_open_time);
	DEB_RETURN() << DEB_VAR1(shut_open_time);
}

void Camera::setShutterCloseTime(double shut_close_time) {
	DEB_MEMBER_FUNCT();
	double settime;
	m_priamAcq.setShutterTime(shut_close_time, settime);
}

void Camera::getShutterCloseTime(double& shut_close_time) const {
	DEB_MEMBER_FUNCT();
	m_priamAcq.getShutterTime(shut_close_time);
}

void Camera::getStatus(DetStatus& status) {
	m_priamAcq.getStatus(status);
}

bool Camera::isAcqRunning() {
	Espia::Acq::Status acq_status;
	m_espiaAcq.getStatus(acq_status);
	return acq_status.running;
}

HwBufferCtrlObj* Camera::getBufferCtrlObj() {
	return &m_bufferCtrlObj;
}

///////////////////////////////////////////////////////////////////////////////////
// Maxipix specific
///////////////////////////////////////////////////////////////////////////////////

void Camera::init() {
	m_edev.resetLink();
	m_priamAcq.setTimeUnit(PriamAcq::UNIT_S);
	m_espiaAcq.registerAcqEndCallback(m_acq_end_cb);
	setNbChip(1, 1);
	if (!m_cfgPath.empty() && !m_cfgName.empty()) {
		loadConfig(m_cfgName, m_reconstruction);
	}
}
void Camera::setPath(const std::string& path) {
	m_cfgPath = path;
}
void Camera::setVersion(Version version) {
	DEB_MEMBER_FUNCT();
	m_version = version;
}

void Camera::setNbChip(int xchips, int ychips) {
	DEB_MEMBER_FUNCT();
	if ((xchips != m_xchips) || (ychips != m_ychips)) {
		m_xchips = xchips;
		m_ychips = ychips;
		m_nchips = m_xchips * m_ychips;
	}
}

void Camera::setPixelGap(int xgap, int ygap) {
	DEB_MEMBER_FUNCT();
	if ((xgap != m_xgap) || (ygap != m_ygap)) {
		m_xgap = xgap;
		m_ygap = ygap;
	}
}

void Camera::setChipsLayout(const MaxipixReconstruction::Layout& layout) {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(layout);
	m_layout = layout;
}

MaxipixReconstruction* Camera::createReconstructionTask() {
	DEB_MEMBER_FUNCT();
	MaxipixReconstruction *reconstruction = NULL;
	switch (m_layout) {
	case MaxipixReconstruction::L_NONE: // No reconstruction
		m_size = Size(m_xchips * 256, 256);
		break;
	case MaxipixReconstruction::L_FREE:
	case MaxipixReconstruction::L_GENERAL:
	        reconstruction = new MaxipixReconstruction(m_layout, m_reconstructType);
		reconstruction->setChipsPosition(m_positions);
		m_size = reconstruction->getImageSize();
		break;
	case MaxipixReconstruction::L_2x2:
	case MaxipixReconstruction::L_5x1:
	        reconstruction = new MaxipixReconstruction(m_layout, m_reconstructType);
		reconstruction->setXnYGapSpace(m_xgap, m_ygap);
		m_size = reconstruction->getImageSize();
		break;
	default:
		throw LIMA_HW_EXC(Error, "Unknown reconstruction model");
	}
	// Update Size to CtImage
	if (m_mis_cb_act) {
		maxImageSizeChanged(m_size, m_type);
	}
	return reconstruction;
}

void Camera::setMaxImageSizeCallbackActive(bool cb_active) {
	DEB_MEMBER_FUNCT();
	m_mis_cb_act = cb_active;
}

void Camera::getFillMode(MaxipixReconstruction::Type& reconstructType) const {
	reconstructType = m_reconstructType;
}

void Camera::setFillMode(MaxipixReconstruction::Type fillMode) {
	DEB_MEMBER_FUNCT();      
	if (m_reconstructionTask != NULL) {
	        m_reconstructType = fillMode;
		m_reconstructionTask->setType(fillMode);	       
	}
}

void Camera::loadConfig(const std::string& name, bool reconstruction) {
	DEB_MEMBER_FUNCT();
//	thread.start_new_thread(Camera._loadConfig,(self,m_hwInt,name,reconstruction))
	acqLoadConfig(name, reconstruction);
}

void Camera::acqLoadConfig(const std::string& name, bool reconstruction) {
	DEB_MEMBER_FUNCT();
	loadDetConfig(name, reconstruction);
	loadChipConfig(name);
	std::cout << "End of configuration, Maxipix is Ok !" << std::endl;
}

void Camera::loadDetConfig(const std::string& name, bool reconstruction) {
	DEB_MEMBER_FUNCT();
	double settime;
	float frequency;
	Polarity polarity;

	MpxDetConfig detConfig;
	detConfig.setPath(m_cfgPath);
	std::cout << "Loading Detector Config <" << name << "> ..." << std::endl;
	detConfig.loadConfig(name);
	detConfig.getFilename(m_cfgFilename);
	detConfig.getPriamPorts(m_priamPorts);
	detConfig.getDacs(m_mpxDacs);
	detConfig.getPositionList(m_positions);
	detConfig.getAsicType(m_version);
	detConfig.getNChips(m_nchips);
	detConfig.getXGap(m_xgap);
	detConfig.getYGap(m_ygap);
	detConfig.getXChips(m_xchips);
	detConfig.getYChips(m_ychips);
	detConfig.getLayout(m_layout);
	m_mpxDacs->setPriamPars(&m_priamAcq, &m_priamPorts);
	DEB_TRACE() << DEB_VAR1(m_layout);

	std::cout << "Setting PRIAM configuration ..." << std::endl;
	std::string fsrString;
	m_mpxDacs->getFsrString(1, fsrString);
	detConfig.getFrequency(frequency);
	detConfig.getPolarity(polarity);

	m_priamAcq.setup(m_version, polarity, frequency, fsrString);
	m_priamAcq.setParallelReadout(m_priamPorts);
	m_priamAcq.setImageMode(PriamAcq::NORMAL);
	m_priamAcq.setGateMode(PriamAcq::INACTIVE);
	m_priamAcq.setShutterMode(PriamAcq::SEQUENCE);
	m_priamAcq.setIntervalTime(0.0, settime);
	m_priamAcq.setShutterTime(0.0, settime);

	// Ask Dacs obj to apply the new FSR registers (DACS values)
	// with a startup energy
	double energy;
	detConfig.getEnergy(energy);
	m_mpxDacs->setEnergy(energy);
	m_mpxDacs->applyChipDacs(0);
	std::cout << "Startup energy threshold = " << energy << " KeV" << std::endl;

	// Reconstruction can be not apply if requested
	setReconstructionActive(reconstruction);
}

void Camera::setReconstructionActive(bool active) {
	DEB_MEMBER_FUNCT();
	// get the default reconstruction task (object) set from the read config parameters
	if (m_reconstructionTask) {
		delete m_reconstructionTask;
	}

	int xchips = m_xchips;
	int ychips = m_ychips;
	int nchips = m_nchips;
	int xgap, ygap;
	MaxipixReconstruction::Layout layout;

	// test first if the reconstruction is desactived then force to NONE
	if (!active) {
		layout = MaxipixReconstruction::L_NONE;
		// flatten detector
		xchips = nchips;
		ychips = 1;
		xgap = ygap = 0;
	} else {
		xgap = m_xgap;
		ygap = m_ygap;
		layout = m_layout;
	}
	setChipsLayout(layout);
	setVersion(m_version);
	setNbChip(xchips, ychips);
	setPixelGap(xgap, ygap);
	
	// must be called even if reconstruction is inactive or NONE,
	// it gets the image size updating by callback
	m_reconstructionTask = createReconstructionTask();
	std::string d_model;
	getDetectorModel(d_model);

	// now decide to active or not a reconstruction
	// accord to either the config file and/or the "active" flag
	if (active && m_reconstructionTask != NULL) {
	       std::cout << "Image reconstruction is switched ON, model:" << d_model << std::endl;
	} else {
	        // no reconstruction, tell the user why
		if (active && m_reconstructionTask == NULL) {
		  std::cout << "Image reconstruction is switched OFF (active=true, config=off), model: " << d_model <<std::endl;
		} else {
		  std::cout << "Image reconstruction is switched OFF (active=false), model: " << d_model <<std::endl;
		}
	}
}

void Camera::loadChipConfig(const std::string& name) {
	DEB_MEMBER_FUNCT();
	m_chipCfg = new MpxPixelConfig(m_version, m_nchips);
	m_chipCfg->setPath(m_cfgPath);
	m_chipCfg->loadConfig(name);
	applyPixelConfig(0);
}

void Camera::applyPixelConfig(int chipid) {
	DEB_MEMBER_FUNCT();
	std::string scfg;
	if (chipid == 0) {
		for (int idx = 0; idx < m_nchips; idx++) {
			m_chipCfg->getMpxString(idx + 1, scfg);
			std::cout << "Loading Chip Config #" << (idx + 1) << " ..." << std::endl;
			m_priamAcq.setChipCfg(m_priamPorts[idx], scfg);
		}
	} else {
		short port = getPriamPort(chipid);
		m_chipCfg->getMpxString(chipid, scfg);
		std::cout << "Loading Chip Config #" << (chipid) << " ..." << std::endl;
		m_priamAcq.setChipCfg(port, scfg);
	}
	// After chip(s) configuration a chip pixel value is needed and
	// can only be done by reading the chips, this can be done with
	// a dummy acquisition
	std::cout << "Resetting chip(s) pixels ..." << std::endl;
	double exptime, settime;
	int nbframes;
	m_priamAcq.getExposureTime(exptime);
	m_priamAcq.getNbFrames(nbframes);
	if (nbframes == 0) {
	  nbframes = 1;
	}
	m_priamAcq.setExposureTime(0.01, settime);
	m_priamAcq.setNbFrames(1);
	m_priamAcq.startAcq();
	sleep(1);
	DetStatus status;
	m_priamAcq.getStatus(status);
	if (status != DetIdle) {
	  m_priamAcq.stopAcq();
	  THROW_HW_ERROR(Error) << "Cannot reset chip(s) after config.";
	}
	m_priamAcq.stopAcq();
	m_priamAcq.setExposureTime(exptime, settime);
	m_priamAcq.setNbFrames(nbframes);
	
}

int Camera::getPriamPort(int chipid) {
	DEB_MEMBER_FUNCT();
	if (chipid <= 0 || chipid > m_nchips) {
		THROW_HW_ERROR(Error) << "<" << chipid << "> is not a valid chipID";
	}
	return m_priamPorts[chipid - 1];
}

/*******************************************************************
 * \brief AcqEndCallback constructor
 *******************************************************************/

Camera::AcqEndCallback::AcqEndCallback(Camera& cam) :
		m_cam(cam) {
	DEB_CONSTRUCTOR();
}

Camera::AcqEndCallback::~AcqEndCallback() {
	DEB_DESTRUCTOR();
}

void Camera::AcqEndCallback::acqFinished(const HwFrameInfoType& /*finfo*/) {
	DEB_MEMBER_FUNCT();
	m_cam.stopAcq();
}

