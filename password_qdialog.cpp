/**
   @file   password_qdialog.cpp
   @brief  password qdialog


   @author Faizal Zakaria
   @date   2012-07-20
*/

#include "password_qdialog.h"

PasswordQDialog::PasswordQDialog(QWidget *parent)
	: QDialog(parent)
 {
     label = new QLabel(tr("Password"));
     lineEdit = new QLineEdit;
     label->setBuddy(lineEdit);

     joinButton = new QPushButton(tr("Join"));
     joinButton->setDefault(true);

     cancelButton = new QPushButton(tr("Cancel"));

     connect(joinButton, SIGNAL(clicked()), parent, SLOT(joinWPANetwork()));
     connect(cancelButton, SIGNAL(clicked()), parent, SLOT(hidePasswordQDialog()));

     QVBoxLayout *passwordLayout = new QVBoxLayout();
     passwordLayout->addWidget(label);
     passwordLayout->addWidget(lineEdit);

     QHBoxLayout *buttonLayout = new QHBoxLayout();
     buttonLayout->addWidget(joinButton);
     buttonLayout->addWidget(cancelButton);
     
     QVBoxLayout *mainLayout = new QVBoxLayout();
     mainLayout->addLayout( passwordLayout );
     mainLayout->addLayout( buttonLayout );

     setLayout(mainLayout);
  
     setWindowTitle(tr("Network"));
 }

QString PasswordQDialog::getPassword()
{
	return lineEdit->text();
}

