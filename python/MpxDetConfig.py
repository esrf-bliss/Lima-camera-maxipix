import os
import ConfigDict
import MpxDacs
from MpxCommon import *

class MpxDetConfig:

    def __init__(self, path=None, name=None):
	self.path= None
	self.reset()

	if path is not None:
	    self.setPath(path)
	if name is not None:
	    self.loadConfig(name)

    def reset(self):
	self.name= None
	self.cfgFile= None

	self.mpxCfg= None
	self.priamCfg= None
	self.thlCfg= None
	self.dacsCfg= None

    def setPath(self, path):
	spath= os.path.normpath(path)
	if not os.path.isdir(spath):
	    raise MpxError("<%s> is not a directory"%path)
	if not os.access(spath, os.R_OK):
	    raise MpxError("no read permission on <%s>"%path)
	self.path= spath

    def loadConfig(self, name):
	cfgFile= self.__getConfigFile(name, None, "cfg")
	self.loadDetectorConfig(cfgFile)
	self.cfgFile= cfgFile
	self.name= name

    def __getConfigFile(self, name, chip, ext):
	fname= name
	if chip is not None:
	    fname= "%s_chip_%d"%(fname, chip)
	fname= "%s.%s"%(fname, ext)
	if self.path is not None:
	    fname= "%s/%s"%(self.path, fname)
	return fname

    def __checkConfigFile(self, fname):
	if not os.path.isfile(fname):
            raise MpxError("<%s> is not a valid file"%fname)
        if not os.access(fname, os.R_OK):
            raise MpxError("No read permission on <%s>"%fname)
        return fname

    def __setParamError(self, msg):
	txt = "ConfigFile <%s>"%self.cfgFile
	if self.__section is not None:
	    txt= txt + " : Section <%s>"%self.__section
	txt= txt + " : " + msg
	raise MpxError(txt)

    def __getParamNeeded(self, pars, name, chklist=None):
	if not pars.has_key(name):
	    self.__setParamError("Missing mandatory parameter <%s>"%name)
	param= pars[name]
	if chklist is not None:
	    if param not in chklist:
		self.__setParamError("<%s> has an invalid value. Should be in %s"%(name, str(chklist)))
	return param

    def __getParamOptional(self, pars, name, chklist=None, default=None):
	if not pars.has_key(name):
	    if default is not None:
		return default
	    else:
	        return None
	param= pars[name]
	if chklist is not None:
	    if param not in chklist:
		self.__setParamError("<%s> has an invalid value. Should be in %s"%(name, str(chklist)))
	return param

    def loadDetectorConfig(self, fname):
	self.cfgFile= self.__checkConfigFile(fname)
	cfg= ConfigDict.ConfigDict()
	cfg.read(fname)

	self.__section= None
	self.__parseMaxipixSection(cfg)
	self.__parsePriamSection(cfg)
	self.__parseThresholdSection(cfg)
	self.__parseDacsSection(cfg)

    def __parseMaxipixSection(self, cfg):
	if not cfg.has_key("maxipix"):
	    self.__setParamError("No <maxipix> section found")
	pars= cfg["maxipix"]
	self.__section= "maxipix"
	self.mpxCfg= {}
	self.mpxCfg["type"]= self.__getParamNeeded(pars, "type", MpxTypes)
	self.mpxCfg["version"]= mpxVersion(self.mpxCfg["type"])
	self.mpxCfg["polarity"]= self.__getParamNeeded(pars, "polarity", [0,1])
	self.mpxCfg["frequency"]= self.__getParamNeeded(pars, "frequency")
	
	self.mpxCfg["xchip"]= self.__getParamNeeded(pars, "xchip", range(1,6))
	self.mpxCfg["ychip"]= self.__getParamOptional(pars, "ychip", [1,2], 1)
	self.mpxCfg["nchip"]= self.mpxCfg["xchip"]*self.mpxCfg["ychip"]

	self.mpxCfg["xgap"]= self.__getParamOptional(pars, "xgap", None, 0)
	self.mpxCfg["ygap"]= self.__getParamOptional(pars, "ygap", None, 0)

    def __parsePriamSection(self, cfg):
	self.priamCfg= {}
	self.priamCfg["espia"]= 0
	if self.mpxCfg["xchip"]==2 and self.mpxCfg["ychip"]==2:
	    self.priamCfg["chip_1"]= 2
	    self.priamCfg["chip_2"]= 3
	    self.priamCfg["chip_3"]= 1
	    self.priamCfg["chip_4"]= 4
	else:
	    for idx in range(1, self.mpxCfg["nchip"]+1):
		self.priamCfg["chip_%d"%idx]= idx

	if cfg.has_key("priam"):
	    pars= cfg["priam"]
	    self.__section= "priam"
	    self.priamCfg["espia"]= self.__getParamOptional(pars, "espia", range(10), self.priamCfg["espia"])
	    for name in [ "chip_%d"%idx for idx in range(1, self.mpxCfg["nchip"]+1)]:
		self.priamCfg[name]= self.__getParamOptional(pars, name, range(1, 6), self.priamCfg[name])

    def __parseThresholdSection(self, cfg):
	self.thlCfg= {}
	self.__section= "threshold"
	pars= cfg.get("threshold", {})
	for name in [ "thlnoise_%d"%idx for idx in range(1, self.mpxCfg["nchip"]+1)]:
	    self.thlCfg[name]= self.__getParamOptional(pars, name, None, 0)
	self.thlCfg["estep"]= self.__getParamOptional(pars, "estep", None, 0)
	self.thlCfg["e0thl"]= self.__getParamOptional(pars, "e0thl", None, 0)

    def __parseDacsSection(self, cfg):
	self.dacsCfg= {}
	self.__section= "dacs"
	if cfg.has_key("dacs"):
	    pars= cfg["dacs"]
	    fsrKeys= MpxDacs.getMpxFsrDef(self.mpxCfg["version"]).listKeys()
	    for key in pars.keys():
		if key not in fsrKeys:
		    self.__setParamError("Invalid key <%s>"%key)
		else:
		    self.dacsCfg[key]= pars[key]

    def getPath(self):
	return self.path

    def getFilename(self):
	return self.cfgFile

    def getName(self):
	return self.name

    def getMaxipixCfg(self):
	return self.mpxCfg

    def getPriamCfg(self):
	return self.priamCfg

    def getThresholdCfg(self):
	return self.thlCfg

    def getDacsCfg(self):
	return self.dacsCfg

if __name__=="__main__":
    import sys
    if len(sys.argv)!=3:
	print "Usage: %s <path> <config_name>"%sys.argv[0]
    else:
	def printDict(pars):
	    for (key,val) in pars.items():
		print "\t", key, "=", val
	    print

	cfg= MpxDetConfig(path= sys.argv[1], name= sys.argv[2])
	print
	print "> Path       =", cfg.getPath()
	print "> ConfigName =", cfg.getName()
	print "> FileName   =", cfg.getFilename()
	print 
	print "[maxipix]"
	printDict(cfg.getMaxipixCfg())
	print "[priam]"
	printDict(cfg.getPriamCfg())
	print "[threshold]"
	printDict(cfg.getThresholdCfg())
	print "[dacs]"
	printDict(cfg.getDacsCfg())

