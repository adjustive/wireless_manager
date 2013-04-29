/**
 * \file    KeyFilter.cpp
 * \brief   Class to filter IR input for navigation.
 * \date    March 16, 2011
 *
 * The KeyFilter class re-maps the UP and DOWN keys to be BACKTAB and TAB so
 * that normal Qt navigation can work.
 */

#include "key_filter.h"
#include <QKeyEvent>
#include <QCoreApplication>
#include <QPushButton>


KeyFilter::KeyFilter( void )
{
	memset( mKeys, 0, sizeof( mKeys ));
}


/**
 * Handle key events before they are sent to the rest of the Qt system.
 *
 * \param[in] unicode
 *            Unicode value for the key event.
 * \param[in] keycode
 *            The actual Qt:Key_xxx code that was received.
 * \param[in] modifiers
 *            A bitwise-inclusive-OR of the Qt::KeyboardModifier values.
 * \param[in] isPress
 *            True if the event was generated from a key press event.
 * \param[in] autorepeat
 *            True if the event was generated from the auto-repeat mechanism.
 *
 * \return Return false to have the system continue processing the event.
 *         Return true to stop processing of the event.
 */
bool KeyFilter::filter( int unicode, int keycode, int modifiers, bool isPress, bool autorepeat )
{
	bool	rc = false;
	int  idx;
   
	/* these three values are not used */
	unicode=unicode;
	modifiers=modifiers;
	autorepeat=autorepeat;

	fprintf(stdout, "unicode : %d %d\n", unicode, keycode);

	if( isPress )
	{
		mMutex.lock();
		idx = findKey( keycode );
		if(( idx >= 0 ) && ( NULL != mKeys[ idx ].func ))
		{
			rc = (*mKeys[ idx ].func)( keycode, mKeys[ idx ].ctx );
		}
		mMutex.unlock();

//      printf( "KeyFilter got key code 0x%x\n", keycode );
	}  
   
	return rc;
}


/**
 ** Add a key callback to the mKeys array.  If a callback already exists for
 ** the given keycode it is replaced with the new callback.
 **
 ** \param[in] keycode
 ** 	       The keycode to add a callback for.
 ** \param[in] func
 ** 	       The function to be called when the key is received.
 ** \param[in] ctx
 ** 	       Opaque application context data to pass to the callback.
 **
 ** \return 0 on success or -1 on error.
 **/
int KeyFilter::addKeyCallback( int keycode, keyCallback func, void* ctx )
{
	int idx;

	mMutex.lock();

	// First look for the given keycode to replace.  If that fails, look for an empty index.
	idx = findKey( keycode );
	if( -1 == idx )
	{
		idx = findKey( 0 );
	}

	if( idx >= 0 )
	{
		mKeys[ idx ].keycode = keycode;
		mKeys[ idx ].func    = func;
		mKeys[ idx ].ctx 	   = ctx;
	}

	mMutex.unlock();
   
	return (idx >= 0)?0:-1;
}


/**
 ** Remove the callback for the given keycode from the list.
 **
 ** \param[in] keycode
 ** 	       The keycode callback to remove.
 **/
void KeyFilter::removeKeyCallback( int keycode )
{
	int idx;

	mMutex.lock();

	idx = findKey( keycode );
	if( idx >= 0 )
	{
		mKeys[ idx ].keycode = 0;
		mKeys[ idx ].func    = NULL;
		mKeys[ idx ].ctx	   = NULL;
	}
      
	mMutex.unlock();

	return;
}


/**
 * Look for the given keycode in the mKeys array.  If a match is found, return the array
 * index, otherwise return -1 to indicate that no match was found.
 * 
 * \param[in] keycode
 * 	      The keycode to look for.  Pass zero to look for an empty slot.
 *
 * \return The matching array index [0..KEY_FILTER_MAX_KEYS-1] or -1.
 */
int KeyFilter::findKey( int keycode )
{
	int ii;
	int idx=-1;
   
	for( ii = 0; ii < KEY_FILTER_MAX_KEYS; ++ii )
	{
		if( mKeys[ ii ].keycode == keycode )
		{
			idx = ii;
			break;
		}
	}

	return idx;
}
