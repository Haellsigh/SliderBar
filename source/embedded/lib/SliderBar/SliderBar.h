#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

#include <cstdint>

#include <Buffer.hpp>
#include <DataInterface.h>

#include <messages/sliderbar.pb.h>
#include <protocol_definition.h>

#include "FixedPointPI.h"

/**
 * @brief The SliderBar class.
 * Serves as the main loop of the SliderBar system.
 * This receives commands through USB, and execute actions accordingly.
 * 
 */
class SliderBar : public DataOutInterface {
public:
    SliderBar()  = default;
    ~SliderBar() = default;

    /**
     * @brief Runs the main loop of the SliderBar.
     * It decodes the messages sent through USB, and executes actions accordingly.
     */
    inline void run()
    {
        decode();
    }

private:
    /**
     * @brief Receive new data.
     * Called by the data layer (ie: USB, serial, ...) when there is new data
     * available. This should normally contain startflag, packet data 
     * (type, value, crc) and endflag.
     *
     * @param buf Pointer to the data.
     * @param len Length of the data.
     */
    void receive(uint8_t* buf, uint16_t len) final;

    /**
     * @brief Transmits data.
     * Called by encode() internally when a new packed is being sent.
     * 
     * @param buf Pointer to the data.
     * @param len Length of the data.
     * @return true If the transfer was successful.
     * @return false If the transfer failed.
     */
    bool transmit(uint8_t* buf, uint16_t len) final;
    void transmit(const Response& response);
    void transmitNack();

    void decode();

    void process(const Request& request);
    void process(const Request_SetValue& value);
    void process(const Request_GetValue& value);
    void process(const Request_Vibrate& value);
    void process(const Request_ResistAt& value);
    void process(const Request_ResistClear& value);
    void process(const Request_Calibration& value);

    /**
     * @brief Calibrates the SliderBar.
     * Stores the minimum position, maximum position and the maximum
     *
     */
    void calibrate();

    // Actions
    /**
     * @brief Vibrates the SliderBar for duration ms.
     * 
     * @param duration 
     */
    void vibrate(uint32_t duration);

private:
    Buffer<protocol::MAX_PACKET_SIZE> m_decodeBuffer;
    bool newData = false;

    FixedPointPI* m_controller;

    uint16_t m_position = 0;
    uint16_t m_velocity = 0;
    uint16_t m_gainP    = 10;
    uint16_t m_gainI    = 1;
};

#endif // __SLIDERBAR_H__
