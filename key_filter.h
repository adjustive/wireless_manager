/**
 * \file    KeyFilter.h
 * \brief   Class definition of the keyboard filter object.
 * \date    March 16, 2011
 *
 * The KeyFilter object is used to remap keys for navigation on the STB.
 */

#ifndef _KEY_FILTER_H
#define _KEY_FILTER_H

#include <QObject>
#include <QMutex>
#include <QWSServer>

typedef bool (*keyCallback)(int,void*);

#define KEY_FILTER_MAX_KEYS	(int)30

class KeyFilter : public QWSServer::KeyboardFilter
{
public:
   KeyFilter();
   virtual ~KeyFilter() {};
   bool filter( int, int, int, bool, bool );
   int addKeyCallback( int, keyCallback, void* );
   void removeKeyCallback( int );
   
private:
   struct {
      int	  keycode;
      keyCallback func;
      void*	  ctx;
   } mKeys[ KEY_FILTER_MAX_KEYS ];

   QMutex mMutex;
   
   int findKey( int );
};

#endif // _KEY_FILTER_H
