

namespace Maxipix {

%TypeHeaderCode
#include <string>
#include <vector>

#include "PriamSerial.h"
#include "PriamAcq.h"
#include "Constants.h"

using namespace lima;
%End

class PriamAcq {

public:

  enum MpxVersion {
    DUMMY,
    MPX2,
    MPX2MXR20,
    TPX10
  };

  enum MpxPolarity {
    NEGATIVE,
    POSITIVE
  };

  enum TimeUnit {
    UNIT_US,
    UNIT_10US,
    UNIT_100US,
    UNIT_MS,
    UNIT_10MS,
    UNIT_100MS,
    UNIT_S,
    UNIT_NB
  };

  enum SignalLevel {
    LOW_FALL,
    HIGH_RISE
  };

  enum ShutterMode {
    FRAME,
    SEQUENCE
  };

  enum GateMode {
    INACTIVE,
    ACTIVE
  };

  enum ReadyMode {
    EXPOSURE,
    EXPOSURE_READOUT
  };

  enum ReadoutMode {
    PARALELL,
    SERIAL
  };

  enum ImageMode {
    NORMAL,
    RAW
  };

    PriamAcq(Maxipix::PriamSerial& priam_serial);
    ~PriamAcq();

    // --- configuration

    void setup(MpxVersion ver, MpxPolarity pol, float freq, std::string fsr0);
    void setChipType(MpxVersion, MpxPolarity);

    void getBoardVersion(short& pcb, short& firmware);
    void getChipID(short chip, long& id);

    void setFastFOSpeed(bool fast);
    void getFastFOSpeed(bool& fast);
    
    void setOscillator(float freq);
    void getOscillator(float& freq);

    void setChipFsr(short chip, std::string fsr);
    void setChipCfg(short chip, std::string cfg);
    
    void enableSerial(short chip);

    // --- timing

    void setTimeUnit(TimeUnit unit);
    void getTimeUnit(TimeUnit& unit);

    void setExposureTime(double askexpo, double& setexpo);
    void getExposureTime(double& expo);

    void setMaxExposureTime();
    void getExposureTimeRange(double& min_expo, double& max_expo);

    void setIntervalTime(double asktime, double& settime);
    void getIntervalTime(double& itime);

    void getMinIntervalTime(double& minit);

    void setShutterTime(double asktime, double& settime);
    void getShutterTime(double& stime);

    // --- external IO signals

    void setShutterLevel(SignalLevel level);
    void getShutterLevel(SignalLevel& level);

    void setShutterMode(ShutterMode mode);
    void getShutterMode(ShutterMode& mode);

    void setReadyLevel(SignalLevel level);
    void getReadyLevel(SignalLevel& level);

    void setReadyMode(ReadyMode mode);
    void getReadyMode(ReadyMode& mode);

    void setGateLevel(SignalLevel level);
    void getGateLevel(SignalLevel& level);

    void setGateMode(GateMode mode);
    void getGateMode(GateMode& mode);

    void setTriggerLevel(SignalLevel level);
    void getTriggerLevel(SignalLevel& level);

    void setTriggerMode(TrigMode mode);
    void getTriggerMode(TrigMode& mode);

    // --- acquisition

    void setNbFrames(int nb);
    void getNbFrames(int& nb);

    void setSerialReadout(short chip);
    void setParalellReadout(std::vector<bool> chips);
    void getReadoutMode(ReadoutMode& mode, std::vector<bool>& chips);

    void setImageMode(ImageMode image);
    void getImageMode(ImageMode& image);

    void setFFCorrection(short flat);
    void getFFCorrection(short& flat);

    void startAcq();
    void stopAcq();

    void getStatus(DetStatus& status);

    // --- reset

    void resetFifo(short chip);
    void resetChip(short chip);

  };
};
