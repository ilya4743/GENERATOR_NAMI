#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_inputRejected();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
