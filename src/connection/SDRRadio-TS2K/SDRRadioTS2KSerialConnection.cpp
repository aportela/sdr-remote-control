#include "SDRRadioTS2KSerialConnection.hpp"

SDRRadioTS2KSerialConnection::SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout)
    : SerialConnection(serialPort, speed, timeout)
{
}

SDRRadioTS2KSerialConnection::~SDRRadioTS2KSerialConnection()
{
}

bool SDRRadioTS2KSerialConnection::tryConnection(Transceiver *trx)
{
    if (millis() - this->lastTXActivity > MILLISECONDS_WAITED_BEFORE_CONNECTION_TRIES)
    {
        this->flush();
        this->send("NA;PS;");
    }
    bool connected = false;
    while (this->serial->available() > 0 && !connected)
    {
        this->lastRXActivity = millis();
        String receivedData = this->serial->readStringUntil(';');
        if (receivedData.startsWith("NA") && receivedData != "NA")
        {
            trx->setRadioName(receivedData.substring(2).c_str());
            this->lastRXValidCommand = millis();
            break;
        }
        else if (receivedData == "PS1")
        {
            this->lastRXValidCommand = millis();
            this->flush();
            connected = true;
            break;
        }
    }
    return (connected);
}

void SDRRadioTS2KSerialConnection::loop(Transceiver *trx)
{
    if (trx != nullptr)
    {
        bool hasSyncCmds = false;     // this is for sending pending queue manual sync commands
        uint8_t maxLoopSyncCmds = 8;  // only get first 8 elements from queue for avoiding very long sync blocks
        bool manualFreqSet = false;   // this is for checking if set frequency manual sync command exists on queued cmds
        bool manualAFGainSet = false; // this is for checking if set af gain manual sync command exists on queued cmds
        bool manualModeSet = false;   // this is for checking if set mode manual sync command exists on queued cmds
        TransceiverSyncCommand syncCmd;
        TransceiverStatus trxStatus;
        trx->getCurrentStatus(&trxStatus, false);
        char str[1024] = {'\0'};
        uint64_t lastSyncCmdFreq = this->lastFrequency; // get last "known" frequency (required for increase/decrease)
        uint8_t lastSyncCmdAFGain = this->lastAFGain;   // get last "known" af gain (required for increase/decrease)
        uint64_t tmpCmdUint64Value = 0;
        while (trx->dequeueSyncCommand(&syncCmd, false) && maxLoopSyncCmds-- > 0)
        {
            char cmd[32] = {'\0'};
            switch (syncCmd.getCommandType())
            {
            case TSCT_ACTIVE_VFO_INDEX_CHANGED:
                // reset previous "current" values on vfo changes
                this->lastFrequency = 0;
                this->lastMode = TRX_VFO_MODE_ERROR;
                this->lastLFFilter = 0;
                this->lastHFFilter = 0;
                // TODO: send current active VFO values
                hasSyncCmds = true;
                manualFreqSet = true;
                manualModeSet = true;
                lastSyncCmdFreq = trxStatus.VFO[trxStatus.activeVFOIndex].frequency;
                sprintf(cmd, "FA%011llu;", lastSyncCmdFreq);
                strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                sprintf(cmd, "MD%u;", trxStatus.VFO[trxStatus.activeVFOIndex].mode);
                strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                break;
            case TSCT_SET_FREQUENCY:
                tmpCmdUint64Value = syncCmd.getUIntValue();
                if (tmpCmdUint64Value >= MIN_FREQUENCY && tmpCmdUint64Value <= MAX_FREQUENCY)
                {
                    hasSyncCmds = true;
                    manualFreqSet = true;
                    lastSyncCmdFreq = tmpCmdUint64Value;
                    sprintf(cmd, "FA%011llu;", lastSyncCmdFreq);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            case TSCT_INCREASE_FREQUENCY:
                tmpCmdUint64Value = syncCmd.getUIntValue() * trxStatus.VFO[trxStatus.activeVFOIndex].frequencyStep;
                if (lastSyncCmdFreq + tmpCmdUint64Value <= MAX_FREQUENCY)
                {
                    hasSyncCmds = true;
                    manualFreqSet = true;
                    lastSyncCmdFreq += tmpCmdUint64Value;
                    sprintf(cmd, "FA%011llu;", lastSyncCmdFreq);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            case TSCT_DECREASE_FREQUENCY:
                tmpCmdUint64Value = syncCmd.getUIntValue() * trxStatus.VFO[trxStatus.activeVFOIndex].frequencyStep;
                if (lastSyncCmdFreq - tmpCmdUint64Value >= MIN_FREQUENCY)
                {
                    hasSyncCmds = true;
                    manualFreqSet = true;
                    lastSyncCmdFreq -= tmpCmdUint64Value;
                    sprintf(cmd, "FA%011llu;", lastSyncCmdFreq);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            case TSCT_TOGGLE_VFO_MODE:
                hasSyncCmds = true;
                manualModeSet = true;
                sprintf(cmd, "MD%u;", trxStatus.VFO[trxStatus.activeVFOIndex].mode);
                strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                break;
            case TSCT_SET_AF_GAIN:
                tmpCmdUint64Value = syncCmd.getUIntValue();
                if (tmpCmdUint64Value >= MIN_AF_GAIN && tmpCmdUint64Value <= MAX_AF_GAIN)
                {
                    hasSyncCmds = true;
                    manualAFGainSet = true;
                    lastSyncCmdAFGain = tmpCmdUint64Value;
                    sprintf(cmd, "AG%03d;", lastSyncCmdAFGain);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            case TSCT_INCREASE_AF_GAIN:
                tmpCmdUint64Value = syncCmd.getUIntValue();
                if (lastSyncCmdAFGain + tmpCmdUint64Value <= MAX_AF_GAIN)
                {
                    hasSyncCmds = true;
                    manualAFGainSet = true;
                    lastSyncCmdAFGain += tmpCmdUint64Value;
                    sprintf(cmd, "AG%03d;", lastSyncCmdAFGain);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            case TSCT_DECREASE_AF_GAIN:
                tmpCmdUint64Value = syncCmd.getUIntValue();
                if (lastSyncCmdAFGain - tmpCmdUint64Value >= MIN_AF_GAIN)
                {
                    hasSyncCmds = true;
                    manualAFGainSet = true;
                    lastSyncCmdAFGain -= tmpCmdUint64Value;
                    sprintf(cmd, "AG%03d;", lastSyncCmdAFGain);
                    strncat(str, cmd, sizeof(str) - strlen(str) - 1);
                }
                break;
            }
        }
        if (hasSyncCmds)
        {
            if (!manualFreqSet)
            {
                strncat(str, "FA;", sizeof(str) - strlen(str) - 1);
            }
            if (!manualAFGainSet)
            {
                strncat(str, "AG;", sizeof(str) - strlen(str) - 1);
            }
            if (!manualModeSet)
            {
                strncat(str, "MD;", sizeof(str) - strlen(str) - 1);
            }
            strncat(str, "SH;SL;MU;SM0;", sizeof(str) - strlen(str) - 1);
            this->send(str);
        }
        else if (millis() - this->lastTXActivity > MILLISECONDS_BETWEEN_LOOP)
        {
            this->send("FA;MD;SH;SL;AG0;MU;SM0;");
        }
        while (this->serial->available() > 0)
        {
            this->lastRXActivity = millis();
            String receivedData = this->serial->readStringUntil(';');
            if (receivedData.startsWith("FA") && receivedData != "FA")
            {
                this->lastRXValidCommand = millis();
                uint64_t currentFrequency = receivedData.substring(2).toInt();
                if (currentFrequency != this->lastFrequency)
                {
                    trx->setActiveVFOFrequency(currentFrequency);
                    this->lastFrequency = currentFrequency;
                }
            }
            else if (receivedData.startsWith("MD") && receivedData != "MD")
            {
                this->lastRXValidCommand = millis();
                TrxVFOMode currentMode = (TrxVFOMode)receivedData.substring(2).toInt();
                if (currentMode != this->lastMode)
                {
                    trx->setCurrentVFOMode(currentMode);
                    this->lastMode = currentMode;
                }
            }
            else if (receivedData.startsWith("SH") && receivedData != "SH")
            {
                this->lastRXValidCommand = millis();
                uint32_t currentFilterCut = receivedData.substring(2).toInt();

                if (this->lastMode == TRX_VFO_MD_FM || this->lastMode == TRX_VFO_MD_SAM || this->lastMode == TRX_VFO_MD_WFM || this->lastMode == TRX_VFO_MD_BFM)
                {
                    // some modes have simmetrical LF/HF values and ignore TS2K command value LF (equal to 0) and SH represent both "cuts" (LF / HF)
                    // so SL0; and SH2000; seems to be equivalent to LOWCUT = 2000 & HI CUT = 2000
                    if (this->lastLFFilter != currentFilterCut || this->lastHFFilter != currentFilterCut)
                    {
                        this->lastLFFilter = currentFilterCut;
                        this->lastHFFilter = currentFilterCut;
                        trx->setCurrentVFOFilter(this->lastLFFilter, this->lastHFFilter);
                    }
                }
                else
                {
                    if (currentFilterCut != this->lastHFFilter)
                    {
                        trx->setCurrentVFOFilterHighCut(currentFilterCut);
                        this->lastHFFilter = currentFilterCut;
                    }
                }
            }
            else if (receivedData.startsWith("SL") && receivedData != "SL")
            {
                this->lastRXValidCommand = millis();
                uint32_t currentFilterCut = receivedData.substring(2).toInt();
                if (currentFilterCut == 0 && (this->lastMode == TRX_VFO_MD_FM || this->lastMode == TRX_VFO_MD_SAM || this->lastMode == TRX_VFO_MD_WFM || this->lastMode == TRX_VFO_MD_BFM))
                {
                    // some modes have simmetrical LF/HF values and use TS2K command value LF = 0 to represent HF value on both "cuts" (LF / HF)
                    // so SL0; and SH2000; is equivalent to LOWCUT = 2000 & HI CUT = 2000
                    if (this->lastLFFilter != this->lastHFFilter)
                    {
                        this->lastLFFilter = this->lastHFFilter;
                        trx->setCurrentVFOFilter(this->lastLFFilter, this->lastHFFilter);
                    }
                }
                else
                {
                    if (currentFilterCut != this->lastLFFilter)
                    {
                        trx->setCurrentVFOFilterLowCut(currentFilterCut);
                        this->lastLFFilter = currentFilterCut;
                    }
                }
            }
            else if (receivedData.startsWith("AG") && receivedData != "AG")
            {
                this->lastRXValidCommand = millis();
                uint8_t currentAFGain = receivedData.substring(2).toInt();
                if (currentAFGain != this->lastAFGain)
                {
                    trx->setAFGain(currentAFGain);
                    this->lastAFGain = currentAFGain;
                }
            }
            else if (receivedData.startsWith("MU") && receivedData != "MU")
            {
                this->lastRXValidCommand = millis();
                bool currentMutedStatus = receivedData.substring(2).toInt() == 1;
                if (currentMutedStatus != this->lastMutedStatus)
                {
                    if (!currentMutedStatus)
                    {
                        trx->setAudioMuted();
                    }
                    else
                    {
                        trx->setAudioUnMuted();
                    }
                    this->lastMutedStatus = currentMutedStatus;
                }
            }
            else if (receivedData.startsWith("SM") && receivedData != "SM")
            {
                this->lastRXValidCommand = millis();
                uint8_t currentSMeterUnits = receivedData.substring(2).toInt();
                if (currentSMeterUnits != this->lastSMeterUnits)
                {
                    trx->setSignalMeter(SIGNAL_METER_TS2K_SDR_RADIO_LEVEL, currentSMeterUnits);
                    this->lastSMeterUnits = currentSMeterUnits;
                }
            }
            else
            {
                this->rxFlush();
            }
        }
    }
}
