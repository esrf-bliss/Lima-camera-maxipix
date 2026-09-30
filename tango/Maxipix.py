############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2009-2026
# European Synchrotron Radiation Facility
# BP 220, Grenoble 38043
# FRANCE
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
#=============================================================================
#
# file :        Maxipix.py
#
# description : Python source for the Maxipix and its commands.
#                The class is derived from Device. It represents the
#                CORBA servant object which will be accessed from the
#                network. All commands which can be executed on the
#                Pilatus are implemented in this file.
#
# project :     TANGO Device Server
#
# copyleft :    European Synchrotron Radiation Facility
#               BP 220, Grenoble 38043
#               FRANCE
#
#=============================================================================
#         (c) - Bliss - ESRF
#=============================================================================
#
import os

import PyTango
from tango import AttrWriteType, DevState
from tango.server import Device, attribute, command, device_property

from lima import core
from lima import maxipix as MaxipixModule
# import some useful helpers to create direct mapping between tango attributes
# and Lima APIs.
from lima.server import AttrHelper

class Maxipix(Device):

    core.DEB_CLASS(core.DebModule.DebModApplication, 'Maxipix')

    # ------------------------------------------------------------------
    #    Static properties
    # ------------------------------------------------------------------
    _MaxipixCamera = None
    """Reference to Lima maxipix camera binding"""
    _MaxipixInterface = None
    """Reference to Lima maxipix interface binding"""

    _SignalLevel = {
        'LOW_FALL': MaxipixModule.PriamAcq.SignalLevel.LOW_FALL,
        'HIGH_RISE': MaxipixModule.PriamAcq.SignalLevel.HIGH_RISE,
    }
    _ReadyMode = {
        'EXPOSURE': MaxipixModule.PriamAcq.ReadyMode.EXPOSURE,
        'EXPOSURE_READOUT': MaxipixModule.PriamAcq.ReadyMode.EXPOSURE_READOUT,
    }
    _GateMode = {
        'INACTIVE': MaxipixModule.PriamAcq.GateMode.INACTIVE,
        'ACTIVE': MaxipixModule.PriamAcq.GateMode.ACTIVE,
    }
    _FillMode = {
        'RAW': MaxipixModule.MaxipixReconstruction.Type.RAW,
        'ZERO': MaxipixModule.MaxipixReconstruction.Type.ZERO,
        'DISPATCH': MaxipixModule.MaxipixReconstruction.Type.DISPATCH,
        'MEAN': MaxipixModule.MaxipixReconstruction.Type.MEAN,
    }

    # ------------------------------------------------------------------
    #    Device properties (initial hardware configuration only - the
    #    live attributes of the same name below read/write the camera
    #    directly, they don't share storage with these)
    # ------------------------------------------------------------------
    espia_dev_nb = device_property(dtype=int, default_value=0)
    config_path = device_property(dtype=str, default_value="")
    config_name = device_property(dtype=str, default_value="")
    reconstruction_active = device_property(dtype=bool, default_value=True)
    fill_mode = device_property(dtype=str, default_value="")
    ready_level = device_property(dtype=str, default_value="")
    gate_level = device_property(dtype=str, default_value="")
    shutter_level = device_property(dtype=str, default_value="")
    trigger_level = device_property(dtype=str, default_value="")
    ready_mode = device_property(dtype=str, default_value="")
    gate_mode = device_property(dtype=str, default_value="")

    # ------------------------------------------------------------------
    #    Device initialization
    # ------------------------------------------------------------------
    @core.DEB_MEMBER_FUNCT
    def init_device(self):
        Device.init_device(self)

        self.set_state(DevState.ON)

        # Leagcy code used by AttrHelper.get_attr_string_value_list
        self.__SignalLevel = self._SignalLevel
        self.__ReadyLevel = self._SignalLevel
        self.__GateLevel = self._SignalLevel
        self.__TriggerLevel = self._SignalLevel
        self.__ShutterLevel = self._SignalLevel
        self.__ReadyMode = self._ReadyMode
        self.__GateMode = self._GateMode
        self.__FillMode = self._FillMode

        # Apply property to the attributes
        priam = self._MaxipixInterface.priamAcq()
        for prop_name, enum, setter in (
            ("fill_mode", self._FillMode, self._MaxipixInterface.setFillMode),
            ("ready_mode", self._ReadyMode, priam.setReadyMode),
            ("ready_level", self._SignalLevel, priam.setReadyLevel),
            ("gate_mode", self._GateMode, priam.setGateMode),
            ("gate_level", self._SignalLevel, priam.setGateLevel),
            ("shutter_level", self._SignalLevel, priam.setShutterLevel),
            ("trigger_level", self._SignalLevel, priam.setTriggerLevel),
        ):
            self.applyNewPropery(prop_name, enum, setter)

    @core.DEB_MEMBER_FUNCT
    def applyNewPropery(self, prop_name, enum, setter):
        key = getattr(self, prop_name)
        if not key:
            return  # property is not set
        deb.Always('Setting property ' + prop_name)
        val = AttrHelper.getDictValue(enum, key.upper())
        if val is None:
            deb.Error('Wrong value for property %s :%s' % (prop_name, val))
        else:
            setter(val)

    def __getConfigNameList(self):
        spath = os.path.normpath(self.config_path)
        if not os.path.isdir(spath):
            PyTango.Except.throw_exception('WrongData',
                                           'Invalid path: %s'%(self.config_path),
                                           'Maxipix Class')

        else:
            dirList = os.listdir(spath)
            fileDict={}
            fileList=[]
            for file in dirList:
                if file.endswith('.cfg'):
                    filePath = spath+'/'+file
                    fileStat = os.stat(filePath)
                    modifiedTime = fileStat.st_mtime
                    fileDict[modifiedTime]= os.path.splitext(file)[0]
        if fileDict:
            timeList = sorted(fileDict.keys())
            for mTime in timeList:
                fileList.append(fileDict[mTime])
        return fileList

    # ------------------------------------------------------------------
    #    Commands
    # ------------------------------------------------------------------
    @command(dtype_in=str, dtype_out=(str,))
    @core.DEB_MEMBER_FUNCT
    def getAttrStringValueList(self, attr_name):
        if attr_name.count('config_name'):
            return self.__getConfigNameList()
        else:
            return AttrHelper.get_attr_string_value_list(self, attr_name)

    # ------------------------------------------------------------------
    #    Attributes with custom read/write logic
    # ------------------------------------------------------------------
    def _config_name_fget(self):
        return self.config_name or ""

    def _config_name_fset(self, data):
        self._MaxipixInterface.loadConfig(data)
        self.config_name = data

    config_name_attr = attribute(
        name="config_name",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Configuration name",
        unit="N/A",
        doc="The configuration name; if changed the detector is reconfigured and reset",
        fget=_config_name_fget,
        fset=_config_name_fset,
    )

    def _config_path_fget(self):
        return self.config_path or ""

    def _config_path_fset(self, data):
        self._MaxipixInterface.setPath(data)
        self.config_path = data

    config_path_attr = attribute(
        name="config_path",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Configuration directory path",
        unit="N/A",
        doc="Path of the configuration directory",
        fget=_config_path_fget,
        fset=_config_path_fset,
    )

    def _espia_dev_nb_fget(self):
        return self.espia_dev_nb or 0

    espia_dev_nb_attr = attribute(
        name="espia_dev_nb",
        dtype=int,
        access=AttrWriteType.READ,
        label="Espia board number",
        unit="number",
        doc="The Espia board device number",
        fget=_espia_dev_nb_fget,
    )

    # ------------------------------------------------------------------
    #    Attributes fully dispatched by naming convention (no explicit
    #    logic in the legacy code either - see AttrHelper.make_fget_fset)
    # ------------------------------------------------------------------
    _energy_threshold_fget, _energy_threshold_fset = AttrHelper.make_fget_fset(
        "energy", lambda self: self._MaxipixInterface
    )
    energy_threshold = attribute(
        dtype=float,
        access=AttrWriteType.READ_WRITE,
        label="Energy threshold",
        unit="KeV",
        format="%5.2f",
        doc="Energy threshold (KeV)",
        fget=_energy_threshold_fget,
        fset=_energy_threshold_fset,
    )

    _fill_mode_fget, _fill_mode_fset = AttrHelper.make_fget_fset(
        "fill_mode", lambda self: self._MaxipixInterface, enum=_FillMode
    )
    fill_mode_attr = attribute(
        name="fill_mode",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Fill mode",
        unit="enum.",
        doc="The chip-gap filling mode: Raw (copied), Zero, Dispatch (interpolated) or Mean",
        fget=_fill_mode_fget,
        fset=_fill_mode_fset,
    )

    _ready_mode_fget, _ready_mode_fset = AttrHelper.make_fget_fset(
        "ready_mode", lambda self: self._MaxipixInterface.priamAcq(), enum=_ReadyMode
    )
    ready_mode_attr = attribute(
        name="ready_mode",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Ready output mode",
        unit="enum.",
        doc="Mode of the Ready output: Exposure or Exposure_Readout",
        fget=_ready_mode_fget,
        fset=_ready_mode_fset,
    )

    _ready_level_fget, _ready_level_fset = AttrHelper.make_fget_fset(
        "ready_level", lambda self: self._MaxipixInterface.priamAcq(), enum=_SignalLevel
    )
    ready_level_attr = attribute(
        name="ready_level",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Ready output level",
        unit="enum.",
        doc="The level logic of the Ready output: High_Rise or Low_Fall",
        fget=_ready_level_fget,
        fset=_ready_level_fset,
    )

    _shutter_level_fget, _shutter_level_fset = AttrHelper.make_fget_fset(
        "shutter_level", lambda self: self._MaxipixInterface.priamAcq(), enum=_SignalLevel
    )
    shutter_level_attr = attribute(
        name="shutter_level",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Shutter output level",
        unit="enum.",
        doc="The level logic of the Shutter output: High_Rise or Low_Fall",
        fget=_shutter_level_fget,
        fset=_shutter_level_fset,
    )

    _gate_mode_fget, _gate_mode_fset = AttrHelper.make_fget_fset(
        "gate_mode", lambda self: self._MaxipixInterface.priamAcq(), enum=_GateMode
    )
    gate_mode_attr = attribute(
        name="gate_mode",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="The Gate input mode",
        unit="enum.",
        doc="The gate mode: Inactive or Active",
        fget=_gate_mode_fget,
        fset=_gate_mode_fset,
    )

    _gate_level_fget, _gate_level_fset = AttrHelper.make_fget_fset(
        "gate_level", lambda self: self._MaxipixInterface.priamAcq(), enum=_SignalLevel
    )
    gate_level_attr = attribute(
        name="gate_level",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Gate input level",
        unit="enum.",
        doc="The Input gate level: High_Rise or Low_Fall",
        fget=_gate_level_fget,
        fset=_gate_level_fset,
    )

    _trigger_level_fget, _trigger_level_fset = AttrHelper.make_fget_fset(
        "trigger_level", lambda self: self._MaxipixInterface.priamAcq(), enum=_SignalLevel
    )
    trigger_level_attr = attribute(
        name="trigger_level",
        dtype=str,
        access=AttrWriteType.READ_WRITE,
        label="Trigger output level",
        unit="enum.",
        doc="The output Trigger level: High_Rise or Low_Fall",
        fget=_trigger_level_fget,
        fset=_trigger_level_fset,
    )


#----------------------------------------------------------------------------
#                              Plugins
#----------------------------------------------------------------------------

def get_control(espia_dev_nb='0', config_path='', config_name='', reconstruction_active='true',
                 _Maxipix=Maxipix, _Camera=MaxipixModule.Camera, _Interface=MaxipixModule.Interface,
                 **keys):
    #properties are passed here as string
    camera = _Maxipix._MaxipixCamera
    interface = _Maxipix._MaxipixInterface

    if reconstruction_active.lower() == 'true': active = True
    else: active  = False
    if camera is None:
        camera = _Camera(int(espia_dev_nb), config_path, config_name, active)
        interface = _Interface(camera)
        _Maxipix._MaxipixCamera = camera
        _Maxipix._MaxipixInterface = interface

    return core.CtControl(interface)


def get_tango_specific_class_n_device():
    return Maxipix.TangoClassClass, Maxipix
