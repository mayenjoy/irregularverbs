#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <QCloseEvent>

QList<IrregularVerb> MainWindow::irregularVerbList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Read the in file with irregular verbs and create the list.
    std::ifstream file;
    file.open("irregularverbs.txt", std::ios::in);
    if (file.is_open())
    {
        std::string infinitive = "";
        std::string past = "";
        std::string pastParticiple = "";
        std::string translation = "";
        int appearances = 0;
        int fails = 0;
        int hits = 0;
        file >> infinitive >> past >> pastParticiple >> translation >> appearances >> fails >> hits;
        while (!file.eof())
        {
            IrregularVerb irregularVerb;
            irregularVerb.setInfinitive(QString::fromStdString(infinitive).replace("_", " "));
            irregularVerb.addPast(QString::fromStdString(past).replace("_", " "));
            irregularVerb.addPastParticiple(QString::fromStdString(pastParticiple).replace("_", " "));
            irregularVerb.addTranslation(QString::fromStdString(translation).replace("_", " "));
            irregularVerb.appearances = appearances;
            irregularVerb.fails = fails;
            irregularVerb.hits = hits;
            irregularVerbList.append(irregularVerb);

            file >> infinitive >> past >> pastParticiple >> translation >> appearances >> fails >> hits;
        }
        file.close();
    }
    else
    {
        ui->statusBar->showMessage("I couldn't open the in file \"irregularverbs.txt\".");
    }

    // Setting an irregular verb depending on order by combo box.
    on_pushButtonSkipThis_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonCheckOut_clicked()
{
    QString infinitive = ui->lineEditInfinitive->text();
    QString past = ui->lineEditPast->text();
    QString pastParticiple = ui->lineEditPastParticiple->text();
    QString translation = ui->lineEditTranslation->text();

    bool error = false;

    // Comparing to the answer. If it doesn't match, it increases the fail count.
    if (currentIrregularVerb.isPast(past))
    {
        // Changing to green background.
        QPalette palette;
        palette.setColor(ui->lineEditPast->backgroundRole(), QColor(100, 255, 100));
        ui->lineEditPast->setPalette(palette);
    }
    else
    {
        // Changing to red background.
        QPalette palette;
        palette.setColor(ui->lineEditPast->backgroundRole(), QColor(255, 100, 100));
        ui->lineEditPast->setPalette(palette);
        if (!error)
            ui->lineEditPast->setFocus(Qt::OtherFocusReason);
        error = true;
    }

    // Comparing to the answer. If it doesn't match, it increases the fail count.
    if (currentIrregularVerb.isPastParticiple(pastParticiple))
    {
        // Changing to green background.
        QPalette palette;
        palette.setColor(ui->lineEditPastParticiple->backgroundRole(), QColor(100, 255, 100));
        ui->lineEditPastParticiple->setPalette(palette);
    }
    else
    {
        // Changing to red background.
        QPalette palette;
        palette.setColor(ui->lineEditPastParticiple->backgroundRole(), QColor(255, 100, 100));
        ui->lineEditPastParticiple->setPalette(palette);
        if (!error)
            ui->lineEditPastParticiple->setFocus(Qt::OtherFocusReason);
        error = true;
    }

    // Comparing to the answer. If it doesn't match, it increases the fail count.
    if (currentIrregularVerb.isTranslation(translation))
    {
        // Changing to green background.
        QPalette palette;
        palette.setColor(ui->lineEditTranslation->backgroundRole(), QColor(100, 255, 100));
        ui->lineEditTranslation->setPalette(palette);
    }
    else
    {
        // Changing to red background.
        QPalette palette;
        palette.setColor(ui->lineEditTranslation->backgroundRole(), QColor(255, 100, 100));
        ui->lineEditTranslation->setPalette(palette);
        if (!error)
            ui->lineEditTranslation->setFocus(Qt::OtherFocusReason);
        error = true;
    }

    // It updates the status of the curret irregular verb and, if it's correct, it choices another irregular verbs.
    if (error)
    {
        currentIrregularVerb.fails++;
        for (int i = 0; i < irregularVerbList.size(); i++)
        {
            if (irregularVerbList.at(i).getInfinitive() == infinitive)
            {
                irregularVerbList.replace(i, currentIrregularVerb);
                break;
            }
        }

        // Disable check out button until user changes something.
        ui->pushButtonCheckOut->setEnabled(false);
    }
    else
    {
        currentIrregularVerb.hits++;
        for (int i = 0; i < irregularVerbList.size(); i++)
        {
            if (irregularVerbList.at(i).getInfinitive() == infinitive)
            {
                irregularVerbList.replace(i, currentIrregularVerb);
                break;
            }
        }
        on_pushButtonSkipThis_clicked();
    }

    // Count fails and hits.
    int totalFails = 0, totalHits = 0;
    for (int i = 0; i < irregularVerbList.size(); i++)
    {
        totalFails += irregularVerbList.at(i).fails;
        totalHits += irregularVerbList.at(i).hits;
    }
    QString aux, aux2;
    QString message = "Fails: " + aux.setNum(totalFails) + " Hits: " + aux2.setNum(totalHits);
    ui->statusBar->showMessage(message);
}

void MainWindow::on_pushButtonShowAnswer_clicked()
{
    // Clearing backgrounds.
    QPalette palette;
    ui->lineEditPast->setPalette(palette);
    ui->lineEditPastParticiple->setPalette(palette);
    ui->lineEditTranslation->setPalette(palette);

    // Showing answer.
    ui->lineEditPast->setText(currentIrregularVerb.getPast());
    ui->lineEditPastParticiple->setText(currentIrregularVerb.getPastParticiple());
    ui->lineEditTranslation->setText(currentIrregularVerb.getTranslation());

    // Disabling buttons.
    ui->pushButtonCheckOut->setEnabled(false);
    ui->pushButtonShowAnswer->setEnabled(false);
}

void MainWindow::on_pushButtonSkipThis_clicked()
{
    // Clearing backgrounds.
    QPalette palette;
    ui->lineEditPast->setPalette(palette);
    ui->lineEditPastParticiple->setPalette(palette);
    ui->lineEditTranslation->setPalette(palette);

    // Putting another irregular verb from the list depending on order by combo box.
    //TODO
    if (irregularVerbList.size() > 0)
    {
        int random = rand() % irregularVerbList.size();
        currentIrregularVerb = irregularVerbList.at(random);
        ui->lineEditInfinitive->setText(currentIrregularVerb.getInfinitive());
        ui->lineEditPast->setText("");
        ui->lineEditPastParticiple->setText("");
        ui->lineEditTranslation->setText("");
    }
    //1. failed recently
    //2. most failed/appeared
    //3. least appearance
    //4. fully random
    int action = rand() % 4;
    switch (action)
    {
    case 0:
        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;
    }

    // Changing focus.
    ui->lineEditPast->setFocus(Qt::OtherFocusReason);

    // Enabling buttons.
    ui->pushButtonCheckOut->setEnabled(false);
    ui->pushButtonShowAnswer->setEnabled(true);
}

// Overwriting this method to save the file before exit the app.
void QWidget::closeEvent(QCloseEvent *event)
{
    std::ofstream file;
    file.open("irregularverbs.txt", std::ios::out);
    if (file.is_open())
    {
        for (int i = 0; i < MainWindow::irregularVerbList.size(); i++)
        {
            file << MainWindow::irregularVerbList.at(i).toString().toStdString() << std::endl;
        }
        file.close();
    }
    else
    {
        if (QMessageBox::warning(0, "Finishing application...", "Failed saving progress. Do you want to exit anyway?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
            event->ignore();
    }
}

void MainWindow::on_lineEditPast_textChanged(QString)
{
    ui->pushButtonCheckOut->setEnabled(true);
}

void MainWindow::on_lineEditPastParticiple_textChanged(QString)
{
    ui->pushButtonCheckOut->setEnabled(true);
}

void MainWindow::on_lineEditTranslation_textChanged(QString)
{
    ui->pushButtonCheckOut->setEnabled(true);
}

void MainWindow::on_lineEditPast_returnPressed()
{
    on_pushButtonCheckOut_clicked();
}

void MainWindow::on_lineEditPastParticiple_returnPressed()
{
    on_pushButtonCheckOut_clicked();
}

void MainWindow::on_lineEditTranslation_returnPressed()
{
    on_pushButtonCheckOut_clicked();
}

//TODO translate every verb
//TODO select a irregular verb manfully

