#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>

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

void MainWindow::showMessage(QString msgString)
{
    QMessageBox msgBox;
    msgBox.setText(msgString);
    msgBox.exec();
}

void MainWindow::on_inputBrowseButton_clicked()
{
    // File Dialog accepts one file with suffix *.in or *.out
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setNameFilter("*.in *.out");
    fileDialog->setFileMode(QFileDialog::ExistingFile);

    if(fileDialog->exec()) {
        QStringList fileList = fileDialog->selectedFiles();
        if (!fileList.isEmpty()) {
            ui->inputFileLabel->setText(fileList.first());
        }
    }

    delete(fileDialog);
}

void MainWindow::on_goButton_clicked()
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
void MainWindow::saveFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        showMessage(QString("Failed to save the output file. Try again."));
        return;
    }

    QString *out = new QString();
    if (ui->avgMonthsToDeath_CB->isChecked())
    {
        out->append("Number of months until death:\t");
        out->append(ui->avgMonthsToDeath->text());
        out->append("\t");
        out->append(ui->sdMonthsToDeath->text());
        out->append("\n");
    }

    if (ui->avgTotalCost_CB->isChecked())
    {
        out->append("Total cost to treat a person:\t");
        out->append(ui->avgTotalCost->text());
        out->append("\t");
        out->append(ui->sdTotalCost->text());
        out->append("\n");
    }

    if (ui->avgCostPerYear_CB->isChecked())
    {
        out->append("Cost per years of life lived:\t");
        out->append(ui->avgCostPerYear->text());
        out->append("\t");
        out->append(ui->sdCostPerYear->text());
        out->append("\n");
    }

    if (ui->avgAgeAtDeath_CB->isChecked())
    {
        out->append("Age of the patient at death:\t");
        out->append(ui->avgAgeAtDeath->text());
        out->append("\t");
        out->append(ui->sdAgeAtDeath->text());
        out->append("\n");
    }
    file.write(out->toUtf8());
    file.close();

    showMessage("File saved successfully.");

    delete(out);
}

void MainWindow::on_saveButton_clicked()
{
    // File Dialog accepts one file with suffix *.in or *.out
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);

    QStringList fileList;
    if(fileDialog->exec()) {
        fileList = fileDialog->selectedFiles();
        if (!fileList.isEmpty())
            saveFile(fileList.first());
    }

    delete(fileDialog);
}

void MainWindow::update()
{
    if ((ui->avgAgeAtDeath_CB->isChecked() ||
        ui->avgCostPerYear_CB->isChecked() ||
        ui->avgMonthsToDeath_CB->isChecked() ||
        ui->avgTotalCost_CB->isChecked()) &&
        !ui->inputFileLabel->text().isEmpty()) {
        // Enable the go button when a least on box is check and we have a file
        ui->goButton->setEnabled(true);
    } else {
        ui->goButton->setEnabled(false);
    }

    if (!ui->avgAgeAtDeath->text().isEmpty() ||
        !ui->avgCostPerYear->text().isEmpty() ||
        !ui->avgMonthsToDeath->text().isEmpty() ||
        !ui->avgTotalCost->text().isEmpty()) {
        ui->saveButton->setEnabled(true);
    } else {
        // Enable the save button when we have data
        ui->saveButton->setEnabled(false);
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

    update();
}

void MainWindow::parseFile()
{
    QFile file(ui->inputFileLabel->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        showMessage(QString("Enter a proper *.in or *.out file"));
        ui->inputFileLabel->selectAll();
        ui->inputFileLabel->setFocus();
        return;
    }

    QTextStream in(&file);
    if (in.atEnd())
    {
        showMessage(QString("The provided file is empty."));
        ui->inputFileLabel->selectAll();
        ui->inputFileLabel->setFocus();
        return;
    }

    v_monthsToDeath.clear();
    v_totalCost.clear();
    v_costPerYear.clear();
    v_ageAtDeath.clear();

    int initAge = 0;

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
