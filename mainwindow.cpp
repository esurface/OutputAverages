#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>

QString *chosenFile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    // init file dialog
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setNameFilter("*.in *.out");

    if(fileDialog->exec()) {
        QStringList fileList = fileDialog->selectedFiles();
        if (!fileList.isEmpty()) {
            ui->fileLabel->setText(fileList.first());
            chosenFile = new QString(fileList.first());
        }

    }
}

void MainWindow::update()
{
    if ((ui->avgAgeAtDeath_CB->isChecked() ||
        ui->avgCostPerYear_CB->isChecked() ||
        ui->avgMonthsToDeath_CB->isChecked() ||
        ui->avgTotalCost_CB->isChecked()) &&
        !ui->fileLabel->text().isEmpty()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }

}

void MainWindow::on_buttonBox_accepted()
{
    QFile file(*chosenFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    double t_monthsToDeath = 0.0;
    double t_totalCost = 0.0;
    double t_costPerYear = 0.0;
    double t_ageAtDeath = 0.0;
    double n_patients = 0.0;
    int initAge = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString ba = in.readLine();

        if (ba.startsWith(QByteArray("BEGIN PATIENT")))
        {
            n_patients++;
            initAge = 0;
        }


        if (ba.startsWith(QByteArray("  gender: ")))
        {
            QStringList split = ba.split(" ");
            initAge = split.at(6).toInt();
        }

        if (ba.startsWith(QByteArray("  LMs ")))
        {
            QStringList split = ba.split(" ");
            double lms = split.at(3).toDouble();
            double cost = split.at(7).toDouble();

            t_monthsToDeath += lms;
            t_totalCost += cost;
            t_costPerYear += (cost)/(lms);
            t_ageAtDeath += (initAge + lms);
        }
    }

    double a_monthsToDeath = 0.0;
    double a_totalCost = 0.0;
    double a_costPerYear = 0.0;
    double a_ageAtDeath = 0.0;

    a_monthsToDeath = t_monthsToDeath / n_patients;
    a_totalCost = t_totalCost / n_patients;
    a_costPerYear = t_costPerYear / n_patients;
    a_ageAtDeath = t_ageAtDeath / n_patients;

    ui->avgMonthsToDeath->setText(QString::number(a_monthsToDeath));
    ui->avgTotalCost->setText(QString::number(a_totalCost));
    ui->avgCostPerYear->setText(QString::number(a_costPerYear));
    ui->avgAgeAtDeath->setText(QString::number(a_ageAtDeath));
}
