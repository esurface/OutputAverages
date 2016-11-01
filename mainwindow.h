#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();
    void on_okButton_clicked();
    void update();

private:
    Ui::MainWindow *ui;

    void runStats();
    void parseFile();
    void showError(QString errString);
};

#endif // MAINWINDOW_H
