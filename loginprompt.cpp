#include "loginprompt.h"
#include "ui_loginprompt.h"

LoginPrompt::LoginPrompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPrompt)
{
    ui->setupUi(this);
}

LoginPrompt::~LoginPrompt()
{
    delete ui;
}

void LoginPrompt::on_buttonBox_accepted()
{
    password[0] = ui->dip_1_check->isChecked();
    password[1] = ui->dip_2_check->isChecked();
    password[2] = ui->dip_3_check->isChecked();
    password[3] = ui->dip_4_check->isChecked();
    password[4] = ui->dip_5_check->isChecked();
    password[5] = ui->dip_6_check->isChecked();
    password[6] = ui->dip_7_check->isChecked();
    password[7] = ui->dip_8_check->isChecked();
}

