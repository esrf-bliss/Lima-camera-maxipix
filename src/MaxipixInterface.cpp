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
#include "MaxipixInterface.h"

#define nullptr 0

using namespace std;
using namespace lima;
using namespace lima::Maxipix;

//-----------------------------------------------------
// @brief Detector Info Control
//-----------------------------------------------------
DetInfoCtrlObj::DetInfoCtrlObj(Camera& cam) :
		m_cam(cam) {
	DEB_CONSTRUCTOR();
}

DetInfoCtrlObj::~DetInfoCtrlObj() {
	DEB_DESTRUCTOR();
}

void DetInfoCtrlObj::getMaxImageSize(Size& size) {
	DEB_MEMBER_FUNCT();
	m_cam.getImageSize(size);
}

void DetInfoCtrlObj::getDetectorImageSize(Size& size) {
	DEB_MEMBER_FUNCT();
	m_cam.getImageSize(size);
}

void DetInfoCtrlObj::getDefImageType(ImageType& image_type) {
	DEB_MEMBER_FUNCT();
	image_type = Bpp16;
}

void DetInfoCtrlObj::getCurrImageType(ImageType& image_type) {
	DEB_MEMBER_FUNCT();
	image_type = Bpp16;
}

void DetInfoCtrlObj::setCurrImageType(ImageType image_type) {
	DEB_MEMBER_FUNCT();
	ImageType valid_image_type;
	getDefImageType(valid_image_type);
	if (image_type != valid_image_type)
		THROW_HW_ERROR(Error) << "Cannot change to "
				<< DEB_VAR2(image_type, valid_image_type);
}

void DetInfoCtrlObj::getPixelSize(double& x_size, double& y_size) {
	DEB_MEMBER_FUNCT();
	m_cam.getPixelSize(x_size, y_size);
}

void DetInfoCtrlObj::getDetectorType(std::string& type) {
	DEB_MEMBER_FUNCT();
	m_cam.getDetectorType(type);
}

void DetInfoCtrlObj::getDetectorModel(std::string& model) {
	DEB_MEMBER_FUNCT();
	m_cam.getDetectorModel(model);
}

void DetInfoCtrlObj::registerMaxImageSizeCallback(HwMaxImageSizeCallback& cb) {
	DEB_MEMBER_FUNCT();
	m_cam.registerMaxImageSizeCallback(cb);
}

void DetInfoCtrlObj::unregisterMaxImageSizeCallback(
		HwMaxImageSizeCallback& cb) {
	DEB_MEMBER_FUNCT();
	m_cam.unregisterMaxImageSizeCallback(cb);
}

//-----------------------------------------------------
// @brief Buffer Control
//-----------------------------------------------------
BufferCtrlObj::BufferCtrlObj(BufferCtrlMgr& buffer_mgr) :
		m_buffer_mgr(buffer_mgr) {
	DEB_CONSTRUCTOR();
}

BufferCtrlObj::~BufferCtrlObj() {
	DEB_DESTRUCTOR();
}

void BufferCtrlObj::setAllocParameters(const AllocParameters& alloc_params) {
	DEB_MEMBER_FUNCT();
	Espia::BufferMgr& espia_buffer_mgr =
		static_cast<Espia::BufferMgr&>(m_buffer_mgr.getAcqBufferMgr());
	espia_buffer_mgr.setAllocParameters(alloc_params);
}

void BufferCtrlObj::getAllocParameters(AllocParameters& alloc_params) {
	DEB_MEMBER_FUNCT();
	Espia::BufferMgr& espia_buffer_mgr =
		static_cast<Espia::BufferMgr&>(m_buffer_mgr.getAcqBufferMgr());
	espia_buffer_mgr.getAllocParameters(alloc_params);
}

void BufferCtrlObj::setFrameDim(const FrameDim& frame_dim) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.setFrameDim(frame_dim);
}

void BufferCtrlObj::getFrameDim(FrameDim& frame_dim) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getFrameDim(frame_dim);
}

void BufferCtrlObj::setNbBuffers(int nb_buffers) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.setNbBuffers(nb_buffers);
}

void BufferCtrlObj::getNbBuffers(int& nb_buffers) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getNbBuffers(nb_buffers);
}

void BufferCtrlObj::setNbConcatFrames(int nb_concat_frames) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.setNbConcatFrames(nb_concat_frames);
}

void BufferCtrlObj::getNbConcatFrames(int& nb_concat_frames) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getNbConcatFrames(nb_concat_frames);
}

void BufferCtrlObj::getMaxNbBuffers(int& max_nb_buffers) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getMaxNbBuffers(max_nb_buffers);

}

void *BufferCtrlObj::getBufferPtr(int buffer_nb, int concat_frame_nb) {
	DEB_MEMBER_FUNCT();
	return m_buffer_mgr.getBufferPtr(buffer_nb, concat_frame_nb);
}

void *BufferCtrlObj::getFramePtr(int acq_frame_nb) {
	DEB_MEMBER_FUNCT();
	return m_buffer_mgr.getFramePtr(acq_frame_nb);
}

void BufferCtrlObj::getStartTimestamp(Timestamp& start_ts) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getStartTimestamp(start_ts);
}

void BufferCtrlObj::getFrameInfo(int acq_frame_nb, HwFrameInfoType& info) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.getFrameInfo(acq_frame_nb, info);
}

void BufferCtrlObj::registerFrameCallback(HwFrameCallback& frame_cb) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.registerFrameCallback(frame_cb);
}

void BufferCtrlObj::unregisterFrameCallback(HwFrameCallback& frame_cb) {
	DEB_MEMBER_FUNCT();
	m_buffer_mgr.unregisterFrameCallback(frame_cb);
}

//-----------------------------------------------------
// @brief Sync Control
//-----------------------------------------------------
SyncCtrlObj::SyncCtrlObj(Camera& cam) :
		HwSyncCtrlObj(), m_cam(cam) {
	DEB_CONSTRUCTOR();
}

SyncCtrlObj::~SyncCtrlObj() {
	DEB_DESTRUCTOR();
}

bool SyncCtrlObj::checkTrigMode(TrigMode trig_mode) {
	return _checkTrigMode(trig_mode);
}

bool SyncCtrlObj::_checkTrigMode(TrigMode trig_mode, bool with_acq_mode) {
	DEB_MEMBER_FUNCT();

	// Get the Acquisition mode, some trigger mode
	// are not valid in Accumulation.

	AcqMode acqMode;
	getAcqMode(acqMode);
	return m_cam.checkTrigMode(trig_mode,
			(with_acq_mode && (acqMode == Accumulation)));

}

void SyncCtrlObj::setTrigMode(TrigMode trig_mode) {
	DEB_MEMBER_FUNCT();
	if (!_checkTrigMode(trig_mode, true)) {
		THROW_HW_ERROR(InvalidValue) << "Invalid in accumulation mode: " << DEB_VAR1(trig_mode);
	}
	m_cam.setTrigMode(trig_mode);
}

void SyncCtrlObj::getTrigMode(TrigMode& trig_mode) {
	DEB_MEMBER_FUNCT();
	m_cam.getTrigMode(trig_mode);
}

void SyncCtrlObj::setExpTime(double exp_time) {
	DEB_MEMBER_FUNCT();
	m_cam.setExpTime(exp_time);
}
void SyncCtrlObj::getExpTime(double& exp_time) {
	DEB_MEMBER_FUNCT();
	m_cam.getExpTime(exp_time);
}

void SyncCtrlObj::setLatTime(double lat_time) {
	DEB_MEMBER_FUNCT();
	m_cam.setLatTime(lat_time);
}
void SyncCtrlObj::getLatTime(double& lat_time) {
	DEB_MEMBER_FUNCT();
	m_cam.getLatTime(lat_time);
}

void SyncCtrlObj::setNbHwFrames(int nb_frames) {
	DEB_MEMBER_FUNCT();
	AcqMode acqMode;
	getAcqMode(acqMode);
	m_cam.setAcqMode(acqMode);
	m_cam.setNbHwFrames(nb_frames);
}
void SyncCtrlObj::getNbHwFrames(int& nb_frames) {
	DEB_MEMBER_FUNCT();
	m_cam.getNbHwFrames(nb_frames);
}

void SyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges) {
	DEB_MEMBER_FUNCT();
	m_cam.getValidRanges(valid_ranges);
}

/*******************************************************************
 * \brief ShutterCtrlObj constructor
 *******************************************************************/

ShutterCtrlObj::ShutterCtrlObj(Camera& cam) :
		m_cam(cam) {
	DEB_CONSTRUCTOR();
}

ShutterCtrlObj::~ShutterCtrlObj() {
	DEB_DESTRUCTOR();
}

bool ShutterCtrlObj::checkMode(ShutterMode shut_mode) const {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(shut_mode);

	bool valid_mode;
	switch (shut_mode) {
	case ShutterAutoFrame:
	case ShutterAutoSequence:
		valid_mode = true;
		break;
	default:
		// No Manual mode for Maxipix !
		valid_mode = false;
	}

	DEB_RETURN() << DEB_VAR1(valid_mode);
	return valid_mode;
}

void ShutterCtrlObj::getModeList(ShutterModeList& mode_list) const {
	DEB_MEMBER_FUNCT();
	mode_list.push_back(ShutterAutoFrame);
	mode_list.push_back(ShutterAutoSequence);
}

void ShutterCtrlObj::setMode(ShutterMode shutter_mode) {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(shutter_mode);

	if (!checkMode(shutter_mode))
		THROW_HW_ERROR(InvalidValue) << "Invalid " << DEB_VAR1(shutter_mode);

	m_cam.setShutterMode(shutter_mode);
}

void ShutterCtrlObj::getMode(ShutterMode& shut_mode) const {
	DEB_MEMBER_FUNCT();

	m_cam.getShutterMode(shut_mode);
}

void ShutterCtrlObj::setState(bool open) {
	DEB_MEMBER_FUNCT();

	THROW_HW_ERROR(NotSupported) << "No manual mode for Maxipix";
}

void ShutterCtrlObj::getState(bool& open) const {
	DEB_MEMBER_FUNCT();

	THROW_HW_ERROR(NotSupported) << "No manual mode for Maxipix";

}

void ShutterCtrlObj::setOpenTime(double shut_open_time) {
	DEB_MEMBER_FUNCT();
	m_cam.setShutterOpenTime(shut_open_time);
}

void ShutterCtrlObj::getOpenTime(double& shut_open_time) const {
	DEB_MEMBER_FUNCT();
	m_cam.getShutterOpenTime(shut_open_time);
	DEB_RETURN() << DEB_VAR1(shut_open_time);
}

void ShutterCtrlObj::setCloseTime(double shut_close_time) {
	DEB_MEMBER_FUNCT();
	m_cam.setShutterCloseTime(shut_close_time);
}

void ShutterCtrlObj::getCloseTime(double& shut_close_time) const {
	DEB_MEMBER_FUNCT();
	m_cam.getShutterCloseTime(shut_close_time);
}

/*******************************************************************
 * \brief ReconstructionCtrlObj constructor
 *******************************************************************/
ReconstructionCtrlObj::ReconstructionCtrlObj() :
		m_reconstruction_task(NULL) {
	DEB_CONSTRUCTOR();
}

ReconstructionCtrlObj::~ReconstructionCtrlObj() {
	DEB_DESTRUCTOR();
	if (m_reconstruction_task)
		m_reconstruction_task->unref();
}
void ReconstructionCtrlObj::setReconstructionTask(LinkTask* task) {
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(task);

	if (task)
		task->ref();
	if (m_reconstruction_task)
		m_reconstruction_task->unref();
	m_reconstruction_task = task;
	reconstructionChange(task);
}


class Interface::_ConfigThread : public Thread
{
  DEB_CLASS_NAMESPC(DebModCamera, "Interface", "_ConfigThread");
public:
  _ConfigThread(Interface& aHwInt);

protected:
  virtual void threadFunction();

private:
  Interface& m_hwint;
};


/*******************************************************************
 * \brief Hw Interface constructor
 *******************************************************************/

Interface::Interface(Camera& cam) :
  m_cam(cam), m_det_info(cam), m_sync(cam), m_shutter(cam),
		m_reconstructionCtrlObj(), m_config_flag(false)
{
	DEB_CONSTRUCTOR();

	HwDetInfoCtrlObj *det_info = &m_det_info;
	m_cap_list.push_back(HwCap(det_info));

	HwBufferCtrlObj *buffer = m_cam.getBufferCtrlObj();
	m_cap_list.push_back(HwCap(buffer));

	HwSyncCtrlObj *sync = &m_sync;
	m_cap_list.push_back(HwCap(sync));

	HwShutterCtrlObj *shutter = &m_shutter;
	m_cap_list.push_back(HwCap(shutter));

	HwReconstructionCtrlObj *reconstruction = &m_reconstructionCtrlObj;
	m_cap_list.push_back(HwCap(reconstruction));

	m_reconstructionTask = m_cam.getReconstructionTask();
	m_reconstructionCtrlObj.setReconstructionTask(m_reconstructionTask);
	reset(SoftReset);

	m_conf_thread = new _ConfigThread(*this);

}

Interface::~Interface() {
	DEB_DESTRUCTOR();
	delete m_conf_thread;
}

void Interface::getCapList(HwInterface::CapList &cap_list) const {
	DEB_MEMBER_FUNCT();
	cap_list = m_cap_list;
}

void Interface::reset(ResetLevel reset_level) {
	DEB_MEMBER_FUNCT();
	m_cam.reset(reset_level);
}

void Interface::prepareAcq() {
	DEB_MEMBER_FUNCT();
	m_cam.prepareAcq();
}

void Interface::startAcq() {
	DEB_MEMBER_FUNCT();
	AcqMode acqMode;
	m_sync.getAcqMode(acqMode);
	m_cam.setAcqMode(acqMode);
	m_cam.startAcq();
}

void Interface::stopAcq() {
	DEB_MEMBER_FUNCT();
	m_cam.stopAcq();
}

int Interface::getNbHwAcquiredFrames() {
	DEB_MEMBER_FUNCT();
	return m_cam.getNbHwAcquiredFrames();
}

void Interface::getStatus(StatusType& status) {
	DEB_MEMBER_FUNCT();
	static const DetStatus det_mask = (DetWaitForTrigger | DetExposure | DetReadout);
	status.det_mask = det_mask;
	if (!m_config_flag) {
		status.acq = m_cam.isAcqRunning() ? AcqRunning : AcqReady;
		m_cam.getStatus(status.det);
	} else {
		status.acq = AcqConfig;
		status.det = DetIdle;
	}

	DEB_RETURN() << DEB_VAR1(status);
}

void Interface::setConfigFlag(bool flag) {
	m_config_flag = flag;
}

void Interface::loadConfig(const std::string& name, bool reconstruction)
{
	DEB_MEMBER_FUNCT();

	m_config_name = name;
	m_reconstuct_flag = reconstruction;
	// switch acq status to AcqConfig, thread will switched back to false
	m_config_flag = true;
	m_conf_thread->start();
}

// Config thread
// (re)loading of a configuration takes a while so to stay in asynchronious mode
// the config loading is threaded and Lima status can be switched to AcqConfig state
// while the detector is reconfigured.
Interface::_ConfigThread::_ConfigThread(Interface& aHwInt) :
  m_hwint(aHwInt)
{
        pthread_attr_setscope(&m_thread_attr, PTHREAD_SCOPE_PROCESS);
}

//---------------------------
//- Interface::_ConfigThread::threadFunction()
//---------------------------
void Interface::_ConfigThread::threadFunction()
{
        DEB_MEMBER_FUNCT();
	DEB_ALWAYS() << "Ok, reconfiguring the detector in _ConfigThread";
	m_hwint.m_cam.loadConfig(m_hwint.m_config_name, m_hwint.m_reconstuct_flag);
	m_hwint.m_config_flag = false;
	DEB_ALWAYS() << "Ok, finished _ConfigThread thread";
	// join to get pthread ended and start again possible for next reconfig
	join();
}
