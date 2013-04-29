/**
   @file   password_qdialog.h
   @brief  password qdialog


   @author Faizal Zakaria
   @date   2012-07-20
*/

#ifndef PASSWORD_QDIALOG_H
#define PASSWORD_QDIALOG_H

#include <QtGui>
#include <QString>
#include <Qt>
#include <QDialog>

class PasswordQDialog : public QDialog
{
    Q_OBJECT
    
public:
	PasswordQDialog(QWidget *parent = 0);
	QString getPassword();
    
private:
	QLabel *label;
	QLineEdit *lineEdit;
	QPushButton *joinButton;
	QPushButton *cancelButton;

private slots:

};

#endif // PASSWORD_QDIALOG_H
