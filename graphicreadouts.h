#ifndef GRAPHICREADOUTS_H
#define GRAPHICREADOUTS_H

#include <QDialog>

namespace Ui {
class GraphicReadouts;
}

class GraphicReadouts : public QDialog
{
    Q_OBJECT

public:
    explicit GraphicReadouts(float temp, float press, float hum, int aqi, QWidget *parent = nullptr);
    ~GraphicReadouts();

private:
    Ui::GraphicReadouts *ui;
    float temperature;
    float pressure;
    float humidity;
    int _aqi;
};

#endif // GRAPHICREADOUTS_H
