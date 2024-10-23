#include "SDRRadioTS2KSerialConnection.hpp"

SDRRadioTS2KSerialConnection::SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout)
    : SerialConnection(serialPort, speed, timeout)
{
    this->trxStatusPtr = new TransceiverStatus;
}

SDRRadioTS2KSerialConnection::~SDRRadioTS2KSerialConnection()
{
    delete this->trxStatusPtr;
    this->trxStatusPtr = nullptr;
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

void SDRRadioTS2KSerialConnection::loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus)
{
    if (trx != nullptr && currentTrxStatus != nullptr)
    {
        bool sendRequired = false;
        char buffer[64] = {'\0'};
        // ignore receive frequency sent cmd if we change manually from rotary encoder in the last 500ms
        if (millis() - currentTrxStatus->lastFrequencyChangedByLocalControl > 250)
        {
            snprintf(buffer, sizeof(buffer), "FA%011" PRIu64 ";MD;SL;SH;AG0;MU;SM0;", currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].frequency);
            sendRequired = true;
        }
        else if (millis() - this->lastTXActivity > MILLISECONDS_BETWEEN_LOOP)
        {
            snprintf(buffer, sizeof(buffer), "%s", "FA;MD;SL;SH;AG0;MU;SM0;");
            sendRequired = true;
        }
        if (sendRequired)
        {
            this->send(buffer);
        }
        while (this->serial->available() > 0)
        {
            this->lastRXActivity = millis();
            String receivedData = this->serial->readStringUntil(';');
            if (receivedData.startsWith("FA") && receivedData != "FA")
            {
                this->lastRXValidCommand = millis();
                // ignore received frequency if we change manually from rotary encoder in the last 500ms
                if (millis() - currentTrxStatus->lastFrequencyChangedByLocalControl > 500)
                {
                    uint64_t f = receivedData.substring(2).toInt();
                    if (currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].frequency != f)
                    {
                        trx->setActiveVFOFrequency(f);
                    }
                }
            }
            else if (receivedData.startsWith("MD") && receivedData != "MD")
            {
                this->lastRXValidCommand = millis();
                TrxVFOMode mode = (TrxVFOMode)receivedData.substring(2).toInt();
                if (currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].mode != mode)
                {
                    trx->setVFOMode(currentTrxStatus->activeVFOIndex, mode);
                }
            }
            else if (receivedData.startsWith("SL") && receivedData != "SL")
            {
                this->lastRXValidCommand = millis();
                uint64_t lowFilter = receivedData.substring(2).toInt();
                if (currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].filter.LF != lowFilter)
                {
                    trx->setVFOFilterLowCut(currentTrxStatus->activeVFOIndex, lowFilter);
                }
            }
            else if (receivedData.startsWith("SH") && receivedData != "SH")
            {
                this->lastRXValidCommand = millis();
                uint64_t highFilter = receivedData.substring(2).toInt();
                if (currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].filter.HF != highFilter)
                {
                    trx->setVFOFilterHighCut(currentTrxStatus->activeVFOIndex, receivedData.substring(2).toInt());
                }
            }
            else if (receivedData.startsWith("AG") && receivedData != "AG")
            {
                this->lastRXValidCommand = millis();
                uint8_t AFGain = receivedData.substring(2).toInt();
                if (currentTrxStatus->AFGain != AFGain)
                {
                    trx->setAFGain(AFGain);
                }
            }
            else if (receivedData.startsWith("MU") && receivedData != "MU")
            {
                this->lastRXValidCommand = millis();
                uint8_t newMutedStatus = receivedData.substring(2).toInt();
                if (currentTrxStatus->audioMuted && newMutedStatus == 0)
                {
                    trx->setAudioUnMuted();
                }
                else if (!currentTrxStatus->audioMuted && newMutedStatus == 1)
                {
                    trx->setAudioMuted();
                }
            }
            else if (receivedData.startsWith("SM") && receivedData != "SM")
            {
                this->lastRXValidCommand = millis();
                uint8_t smLevel = receivedData.substring(2).toInt();
                trx->setSignalMeter(SIGNAL_METER_TS2K_SDR_RADIO_LEVEL, smLevel);
            }
        }
    }
    else
    {
        this->rxFlush();
    }
}

void SDRRadioTS2KSerialConnection::syncLocalToRemote(Transceiver *trx, const TransceiverStatus *currentTrxStatus)
{
    if (currentTrxStatus != nullptr)
    {
        if (currentTrxStatus->poweredOn)
        {
            // connected => main
            /*
            this->send("FA;");   // get frequency
            this->send("MD;");   // get mode
            this->send("SL;");   // get low filter
            this->send("SH;");   // get high filter
            this->send("AG0;");  // get af gain (volume)
            this->send("MU;");   // get audio mute status
            this->send("SM0;");  // get signal meter level
            */
            // this works BETTER than sending separated commands (with own delay)
            char customCMD[256];
            sprintf(customCMD, "FA%011llu;MD;SL;SH;AG0;MU;SM0;", currentTrxStatus->VFO[currentTrxStatus->activeVFOIndex].frequency);
            // this->send(customCMD);
            this->rxFlush();
            this->lastCATActivity = millis();
        }
    }
}
