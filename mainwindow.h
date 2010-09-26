#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "IrregularVerb.h"
#include <iostream>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QList<IrregularVerb> irregularVerbList;
	QList<IrregularVerb> failedRecentlyList;

private:
    Ui::MainWindow *ui;
    IrregularVerb currentIrregularVerb;

private slots:
    void on_lineEditPastParticiple_returnPressed();
	void on_lineEditPast_returnPressed();
    void on_lineEditPastParticiple_textChanged(QString );
    void on_lineEditPast_textChanged(QString );
    void on_pushButtonSkipThis_clicked();
    void on_pushButtonCheckOut_clicked();
    void on_pushButtonShowAnswer_clicked();
};

#endif // MAINWINDOW_H
