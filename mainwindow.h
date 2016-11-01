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
    void on_inputBrowseButton_clicked();
    void on_goButton_clicked();
    void on_saveButton_clicked();

    void update();

private:
    Ui::MainWindow *ui;

    void runStats();
    void parseFile();
    void showMessage(QString msgString);
    void saveFile(QString filename);
};

#endif // MAINWINDOW_H
