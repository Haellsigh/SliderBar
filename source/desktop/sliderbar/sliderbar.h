#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <QObject>

#include <Buffer/Buffer.hpp>
#include <DataInterface/DataInterface.h>
#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>

class SliderBar : public QObject, public DataOutInterface {
    Q_OBJECT

public:
    SliderBar(QWidget* parent);
    ~SliderBar() override;

public slots:
    /**
     * @brief Connects the SliderBar.
     */
    void connect();

    /**
     * @brief Show the settings window.
     * @note Blocking function.
     *
     */
    void manageSettings();

    /**
     * @brief Show the plugin management window.
     * @note Blocking function.
     */
    void managePlugins();

    /**
     * @brief Sets the autoconnect setting.
     * @param enabled True if autoconnect should be enabled.
     */
    void autoconnect(bool enabled);

    void receive(uint8_t* buf, uint16_t len) final;
    bool transmit(uint8_t* buf, uint16_t len) final;
    void transmit(const Example& msg);

signals:
    void settingsChanged();

private:
    QWidget* m_parent;
    Buffer<protocol::MAX_PACKET_SIZE> m_dataBuffer;
};

#endif // SLIDERBAR_H
