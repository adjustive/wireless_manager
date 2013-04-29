/**
   @file   mainwindow.h
   @brief  main window


   @author Faizal Zakaria
   @date   2012-07-20
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wireless.h"
#include <QListView>
#include <QtGui>
#include <QString>
#include <Qt>
#include "password_qdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow();
    ~MainWindow();
    
private:
	int currentIndex;
	QString currentSSID;
	QListView *networkList;
	Wireless *wireless;
	QStringListModel *listModel;
	PasswordQDialog *passwordQDialog;

	void createStatusBar();


private slots:
	void scanCompleted();
	void joinNetwork();
	void joinWPANetwork();
	void hidePasswordQDialog();

};

#endif // MAINWINDOW_H
