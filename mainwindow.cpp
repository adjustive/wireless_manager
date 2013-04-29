/**
   @file   mainwindow.cpp
   @brief  main window


   @author Faizal Zakaria
   @date   2012-07-20
*/

#include "mainwindow.h"


MainWindow::MainWindow(void) : QMainWindow()
{
    wireless = new Wireless();
    
    currentIndex = -1;
    currentSSID = "";

    QFrame *frame = new QFrame;
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);

    QPushButton *scanWIFI;
    scanWIFI = new QPushButton("Scan Network");
    connect(scanWIFI, SIGNAL(clicked()), this, SLOT(scanCompleted()));

    QPushButton *joinNetwork;
    joinNetwork = new QPushButton("Join");
    connect(joinNetwork, SIGNAL(clicked()), this, SLOT(joinNetwork()));

    networkList = new QListView();
    networkList->setViewMode(QListView::ListMode);

    listModel = new QStringListModel();

    networkList->setModel(listModel);
    frameLayout->addWidget(scanWIFI);
    frameLayout->addWidget(networkList);
    frameLayout->addWidget(joinNetwork);

    setFixedSize( 640, 480 );

    //frame->setFrameRect(QRect( 10, 10, 640, 480 ));

    setCentralWidget(frame);

    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move( x, y );

    createStatusBar();

    passwordQDialog = new PasswordQDialog( this );

    setWindowTitle( tr("Wireless Manager") );

    // SelectionModel
    /*
    QItemSelectionModel *selectionModel = networkList->selectionModel();
    connect( selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)) );
    */
}

MainWindow::~MainWindow()
{
	delete wireless;
	delete passwordQDialog;
	delete listModel;
	delete networkList;
}

void MainWindow::scanCompleted()
{
	QString text = "Scanning network ... ";
	int status = 0;
	statusBar()->showMessage(text);
	status = wireless->scanNetworks( (const QString) "wlan0" );
	if (status != 0)
	{
		text = "Failed to scan network ... ";
		statusBar()->showMessage(text);
	}
	else
	{
		text = "Scan network done! ";
		statusBar()->showMessage(text);
	}

    listModel->setStringList( (const QStringList)*wireless->getNetworkList() );
}

void MainWindow::joinNetwork()
{
	const QModelIndex index = networkList->selectionModel()->currentIndex();
	currentSSID = index.data(Qt::DisplayRole).toString();
	QString text;

	text = "Connecting to " + currentSSID + " ...";
	statusBar()->showMessage(text);

	int status = wireless->joinOpenNetwork((const QString) currentSSID);
	if ( status < 0 )
	{
		text = "Failed to join Network : "  +  currentSSID + ", try again";
		statusBar()->showMessage(text);
		//passwordQDialog->setSSID( &currentSSID );
		passwordQDialog->show();
	}
	else
	{
		text = "Connected to " + currentSSID;
		statusBar()->showMessage(text);
	}
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::hidePasswordQDialog()
{
	passwordQDialog->hide();
}

void MainWindow::joinWPANetwork()
{
	QString text;

	text = "Connecting to " + currentSSID + " ..." + passwordQDialog->getPassword();
	statusBar()->showMessage(text);

	int status = wireless->joinWPANetwork((const QString) currentSSID, (const QString) passwordQDialog->getPassword() );
	if ( status < 0 )
	{
		text = "Failed to join Network : "  +  currentSSID + ", try again";
		statusBar()->showMessage(text);
	}
	else
	{
		text = "Connected to " + currentSSID;
		statusBar()->showMessage(text);
	}
	passwordQDialog->hide();
}
