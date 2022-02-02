#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

namespace Ui {
class Settings;
}

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_inputRejected();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
