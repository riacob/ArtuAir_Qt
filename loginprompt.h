#ifndef LOGINPROMPT_H
#define LOGINPROMPT_H

#include <QDialog>

namespace Ui {
class LoginPrompt;
}

class LoginPrompt : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPrompt(QWidget *parent = nullptr);
    ~LoginPrompt();
    bool password[8];

private slots:

    void on_buttonBox_accepted();

private:
    Ui::LoginPrompt *ui;
};

#endif // LOGINPROMPT_H
