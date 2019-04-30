

// OpenLCB Adaptation of FlexCAN library
// copyright DPH 2017

//#pragma message("!!! compiling ESPcan.cpp ")


#include "Arduino.h"

#include "OlcbCan.h"
#include "ESPcan.h"
#include "ESP32can.h"

class ESP32can;
ESP32can esp32can;

// ----------------------------------------------------------------------------
/**
 * \ingroup  can_interface
 * \brief Initialisierung des CAN Interfaces
 *
 * \param bitrate Gewuenschte Geschwindigkeit des CAN Interfaces
 *
 * \return  false falls das CAN Interface nicht initialisiert werden konnte,
 *      true ansonsten.
 */
void Can::init() {
    //Serial.print("\nIn ESP Can::init");
    //esp32can.begin(125000);
    if (!CAN.begin(125E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }
    return;
}

uint8_t Can::avail() {
    uint8_t r = esp32can.avail();
                //Serial.print("\n Can::avail()="); Serial.print(r);
    return r;
    //return esp32can.avail();
}

uint8_t Can::read() {
    //Serial.print("\nIn ESPcan::read()");
    if(!esp32can.avail()) return 0;
    bool extended;
    esp32can.read(id, length, data, extended);
    if(flags.rtr) return 0;
    //if(m.err!=0) return 0;
    //this->id = m.id;
    flags.extended = extended;
    //this->flags.rtr = m.rtr;
    //this->length = m.dlc;
    //for(int i=0;i<m.dlc;i++) this->data[i] = m.buf[i];
    return 1;
}

uint8_t Can::txReady() {
            //Serial.print("\n ESPcan::txReady()");
    bool b = esp32can.txReady();
            //Serial.print("\n esp Can::txReady()=");
            //Serial.print(b );
    return b;
}

uint8_t Can::write(long timeout) {
    //CAN_message_t m;
            //Serial.print("\n ESP Can::write()");
    //m.id = this->id;
    //m.dlc = this->length;
    //m.eff = 1;
    //m.rtr=0;
    //memcpy(m.buf,this->data,m.dlc);
            //Serial.print(id, HEX);
            //Serial.print("]("); Serial.print(length);
            //Serial.print(") ");
            //for(int i=0;i<length;i++)
            //    { Serial.print(data[i],HEX); Serial.print(" "); }
    if(timeout==0 && this->txReady()) {
        active = true;
        return esp32can.write(id, length, data);
    }
            //Serial.print("\n     TivaCan::write()#C");
    long to = millis() + timeout;
    while(millis()<to) {
        if(this->txReady()) {
            active = true;
            return esp32can.write(id, length, data);
        }
    }
    return false;
}
uint8_t Can::write() { return this->write(0); }
