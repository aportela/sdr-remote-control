#include "SDRRadioTS2KSerialConnection.hpp"

SDRRadioTS2KSerialConnection::SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout)
    : SerialConnection(serialPort, speed, timeout)
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
            trx->incSerialCommandCount();
            break;
        }
        else if (receivedData == "PS1")
        {
            this->lastRXValidCommand = millis();
            trx->incSerialCommandCount();
            this->flush();
            connected = true;
            break;
        }
    }
    return (connected);
}

void SDRRadioTS2KSerialConnection::loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus)
{
    if (trx != nullptr && currentTrxStatus != nullptr && !currentTrxStatus->lockedByControls)
    {
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
        if (millis() - this->lastTXActivity > MILLISECONDS_BETWEEN_LOOP)
        {
            this->send("FA;MD;SL;SH;AG0;MU;SM0;");
        }
        while (this->serial->available() > 0)
        {
            this->lastRXActivity = millis();
            String receivedData = this->serial->readStringUntil(';');
            if (receivedData.startsWith("FA") && receivedData != "FA")
            {
                this->lastRXValidCommand = millis();
                uint64_t f = receivedData.substring(2).toInt();
                trx->setActiveVFOFrequency(f);
                trx->incSerialCommandCount();
            }
            else if (receivedData.startsWith("MD") && receivedData != "MD")
            {
                this->lastRXValidCommand = millis();
                trx->setVFOMode(currentTrxStatus->activeVFOIndex, (TrxVFOMode)receivedData.substring(2).toInt());
                trx->incSerialCommandCount();
            }
            else if (receivedData.startsWith("SL") && receivedData != "SL")
            {
                this->lastRXValidCommand = millis();
                trx->setVFOFilterLowCut(currentTrxStatus->activeVFOIndex, receivedData.substring(2).toInt());
                trx->incSerialCommandCount();
            }
            else if (receivedData.startsWith("SH") && receivedData != "SH")
            {
                this->lastRXValidCommand = millis();
                trx->setVFOFilterHighCut(currentTrxStatus->activeVFOIndex, receivedData.substring(2).toInt());
                trx->incSerialCommandCount();
            }
            else if (receivedData.startsWith("AG") && receivedData != "AG")
            {
                this->lastRXValidCommand = millis();
                trx->setAFGain(receivedData.substring(2).toInt());
                trx->incSerialCommandCount();
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
                trx->incSerialCommandCount();
            }
            else if (receivedData.startsWith("SM") && receivedData != "SM")
            {
                this->lastRXValidCommand = millis();
                trx->setSignalMeter(SIGNAL_METER_TS2K_SDR_RADIO_LEVEL, receivedData.substring(2).toInt());
                trx->incSerialCommandCount();
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
