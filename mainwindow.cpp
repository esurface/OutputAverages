#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QVector>

#include "stats.h"

QVector<double> v_monthsToDeath;
QVector<double> v_totalCost;
QVector<double> v_costPerYear;
QVector<double> v_ageAtDeath;

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
        ui->okButton->setEnabled(true);
    } else {
        ui->okButton->setEnabled(false);
    }
}

void MainWindow::runStats()
{
    double a_monthsToDeath = 0.0;
    double a_totalCost = 0.0;
    double a_costPerYear = 0.0;
    double a_ageAtDeath = 0.0;

    double sd_monthsToDeath = 0.0;
    double sd_totalCost = 0.0;
    double sd_costPerYear = 0.0;
    double sd_ageAtDeath = 0.0;

    Stats stats = Stats(v_monthsToDeath);
    a_monthsToDeath = stats.getMean();
    sd_monthsToDeath = stats.getStdDev();

    stats = Stats(v_totalCost);
    a_totalCost = stats.getMean();
    sd_totalCost = stats.getStdDev();

    stats = Stats(v_costPerYear);
    a_costPerYear = stats.getMean();
    sd_costPerYear = stats.getStdDev();

    stats = Stats(v_ageAtDeath);
    a_ageAtDeath = stats.getMean();
    sd_ageAtDeath = stats.getStdDev();

    if (ui->avgMonthsToDeath_CB->isChecked())
    {
        ui->avgMonthsToDeath->setText(QString::number(a_monthsToDeath));
        ui->sdMonthsToDeath->setText(QString::number(sd_monthsToDeath));
    }

    if (ui->avgTotalCost_CB->isChecked())
    {
        ui->avgTotalCost->setText(QString::number(a_totalCost));
        ui->sdTotalCost->setText(QString::number(sd_totalCost));
    }

    if (ui->avgCostPerYear_CB->isChecked())
    {
        ui->avgCostPerYear->setText(QString::number(a_costPerYear));
        ui->sdCostPerYear->setText(QString::number(sd_costPerYear));
    }

    if (ui->avgAgeAtDeath_CB->isChecked())
    {
        ui->avgAgeAtDeath->setText(QString::number(a_ageAtDeath));
        ui->sdAgeAtDeath->setText(QString::number(sd_ageAtDeath));
    }
}

void MainWindow::showError(QString errString)
{
    //
}

void MainWindow::parseFile()
{
    QFile file(ui->fileLabel->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        showError(QString("Enter a proper *.in or *.out file"));
        return;
    }

    int initAge = 0;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString ba = in.readLine();

        if (ba.startsWith(QByteArray("BEGIN PATIENT")))
        {
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

            v_monthsToDeath.append(lms);
            v_totalCost.append(cost);
            v_costPerYear.append((cost)/(lms));
            v_ageAtDeath.append((initAge + lms));
        }
    }

    runStats();
}

void MainWindow::on_okButton_clicked()
{
    ui->avgMonthsToDeath->clear();
    ui->sdMonthsToDeath->clear();

    ui->avgTotalCost->clear();
    ui->sdTotalCost->clear();

    ui->avgCostPerYear->clear();
    ui->sdCostPerYear->clear();

    ui->avgAgeAtDeath->clear();
    ui->sdAgeAtDeath->clear();

    parseFile();
}
