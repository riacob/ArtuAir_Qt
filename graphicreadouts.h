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
    explicit GraphicReadouts(QWidget *parent = nullptr);
    ~GraphicReadouts();
    float temperature;
    float pressure;
    float humidity;
    int aqi;

private:
    Ui::GraphicReadouts *ui;
};

#endif // GRAPHICREADOUTS_H
