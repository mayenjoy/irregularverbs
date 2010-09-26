#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <QCloseEvent>
#include <QtAlgorithms>

//#define DEBUG

QList<IrregularVerb> MainWindow::irregularVerbList;

using namespace std;

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
        int appearances = 0;
        int fails = 0;
        int hits = 0;
		file >> infinitive >> past >> pastParticiple >> appearances >> fails >> hits;
        while (!file.eof())
        {
            IrregularVerb irregularVerb;
            irregularVerb.setInfinitive(QString::fromStdString(infinitive).replace("_", " "));
            irregularVerb.addPast(QString::fromStdString(past).replace("_", " "));
			irregularVerb.addPastParticiple(QString::fromStdString(pastParticiple).replace("_", " "));
            irregularVerb.appearances = appearances;
            irregularVerb.fails = fails;
            irregularVerb.hits = hits;
            irregularVerbList.append(irregularVerb);

			file >> infinitive >> past >> pastParticiple >> appearances >> fails >> hits;
        }
        file.close();
    }

	// Exit if irregular verbs can't be loaded.
	if (irregularVerbList.size() == 0)
	{
		QMessageBox::warning(0, "Finishing application...", "An error was ocurred and the program will be closed. There are not any irregular verb in the list. Check your irregularVerbs.txt file.", QMessageBox::Close);
		exit(0);
	}

	// Set an irregular verb.
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

	// It updates the status of the curret irregular verb and, if it's correct, it choices another irregular verb.
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

		// Remove from failed recently list and add it again to refresh its status.
		for (int i = 0; i < failedRecentlyList.size(); i++)
		{
			if (failedRecentlyList.at(i).getInfinitive() == infinitive)
			{
				failedRecentlyList.removeAt(i);
				break;
			}
		}
		failedRecentlyList.append(currentIrregularVerb);
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

		// Remove from failed recently list.
		for (int i = 0; i < failedRecentlyList.size(); i++)
		{
			if (failedRecentlyList.at(i).getInfinitive() == infinitive)
			{
				failedRecentlyList.removeAt(i);
				break;
			}
		}

        on_pushButtonSkipThis_clicked();
    }
}

void MainWindow::on_pushButtonShowAnswer_clicked()
{
    // Clearing backgrounds.
    QPalette palette;
    ui->lineEditPast->setPalette(palette);
	ui->lineEditPastParticiple->setPalette(palette);

    // Showing answer.
    ui->lineEditPast->setText(currentIrregularVerb.getPast());
	ui->lineEditPastParticiple->setText(currentIrregularVerb.getPastParticiple());

    // Disabling buttons.
    ui->pushButtonCheckOut->setEnabled(false);
    ui->pushButtonShowAnswer->setEnabled(false);
}

void MainWindow::on_pushButtonSkipThis_clicked()
{
	// Clear backgrounds.
    QPalette palette;
    ui->lineEditPast->setPalette(palette);
	ui->lineEditPastParticiple->setPalette(palette);

	// Put another irregular verb from the list depending on order by combo box.
	int action = rand() % 100;
	if (action < 20 && failedRecentlyList.size() > 0)
	{
		// Failed recently.
		int random = rand() % failedRecentlyList.size();
		currentIrregularVerb = failedRecentlyList.at(random);

		#ifdef DEBUG
		cout << "Failed recently (" << failedRecentlyList.size() << ")" << endl;
		#endif
	}
	else if (action < 30)
	{
		// Most failed/appearanced.
		QList<IrregularVerb> auxiliarList = irregularVerbList;
		qSort(auxiliarList.begin(), auxiliarList.end());
		currentIrregularVerb = auxiliarList.at(0);

		#ifdef DEBUG
		cout << "Most failed" << endl;

		for (int i = 0; i < 10; i++)
		{
			cout << auxiliarList.at(i).toString().toStdString() << endl;
		}
		#endif
	}
	else
	{
		// Totally random.
		int random = rand() % irregularVerbList.size();
		currentIrregularVerb = irregularVerbList.at(random);

		#ifdef DEBUG
		cout << "Totally random" << endl;
		#endif
	}

	// Increase appearances.
	currentIrregularVerb.appearances++;
	for (int i = 0; i < irregularVerbList.size(); i++)
	{
		if (irregularVerbList.at(i).getInfinitive() == currentIrregularVerb.getInfinitive())
		{
			irregularVerbList.replace(i, currentIrregularVerb);
			break;
		}
	}

	// Update input boxes.
	ui->lineEditInfinitive->setText(currentIrregularVerb.getInfinitive());
	ui->lineEditPast->setText("");
	ui->lineEditPastParticiple->setText("");

    // Changing focus.
    ui->lineEditPast->setFocus(Qt::OtherFocusReason);

    // Enabling buttons.
    ui->pushButtonCheckOut->setEnabled(false);
    ui->pushButtonShowAnswer->setEnabled(true);

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

// Overwrite this method to save the file before exit the app.
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

void MainWindow::on_lineEditPast_returnPressed()
{
    on_pushButtonCheckOut_clicked();
}

void MainWindow::on_lineEditPastParticiple_returnPressed()
{
    on_pushButtonCheckOut_clicked();
}
