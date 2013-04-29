/**
   @file   main.cpp
   @brief  main app


   @author Faizal Zakaria
   @date   2012-07-20
*/

#include <QtGui>
#include "mainwindow.h"
#include "key_filter.h"

#if defined RMPLATFORM
#include <QWSServer>
#endif

bool remappingKeyCallback( int keycode, void* ctx );
bool exitApplication( int keycode, void* ctx );

int main(int argc, char *argv[])
{
    QApplication a(argc, argv, QApplication::GuiServer );

	KeyFilter*	 pFilter = new KeyFilter();
	MainWindow mainWindow;
	mainWindow.show();

	QWSServer::addKeyboardFilter( pFilter );
	QWSServer::setCursorVisible( false );

	pFilter->addKeyCallback( Qt::Key_MediaNext, remappingKeyCallback, NULL );
	pFilter->addKeyCallback( Qt::Key_MediaPrevious, remappingKeyCallback, NULL );
	pFilter->addKeyCallback( Qt::Key_Return, remappingKeyCallback, NULL );
	pFilter->addKeyCallback( Qt::Key_Home, exitApplication, NULL );

	return a.exec();
}

bool remappingKeyCallback( int keycode, void* ctx )
{
	QKeyEvent* ev;

	ctx = ctx;

	switch( keycode )
	{
	case Qt::Key_MediaPrevious:
		printf( "Remapping to backtab\n" );
		ev = new QKeyEvent( QEvent::KeyPress, Qt::Key_Backtab, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget(), ev );
		ev = new QKeyEvent( QEvent::KeyRelease, Qt::Key_Tab, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget(), ev );
		break;

	case Qt::Key_MediaNext:
		printf( "Remapping to tab\n" );
		ev = new QKeyEvent( QEvent::KeyPress, Qt::Key_Tab, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget(), ev );
		ev = new QKeyEvent( QEvent::KeyRelease, Qt::Key_Tab, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget(), ev );
		break;
	case Qt::Key_Return:
		printf( "Remapping to select\n" );
		ev = new QKeyEvent( QEvent::KeyPress, Qt::Key_Select, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget() , ev );
		ev = new QKeyEvent( QEvent::KeyRelease, Qt::Key_Select, 0 );
		QCoreApplication::sendEvent( (QObject*)QApplication::focusWidget(), ev );
		break;
	}

	return true;
}

bool exitApplication( int keycode, void* ctx )
{
	QApplication::exit(0);
	
	return true;
}
