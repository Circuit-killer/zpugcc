/* gtkfiledialogpeer.c -- Native implementation of GtkFileDialogPeer
   Copyright (C) 1998, 1999, 2002 Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


#include "gtkpeer.h"
#include "gnu_java_awt_peer_gtk_GtkComponentPeer.h"
#include "gnu_java_awt_peer_gtk_GtkFileDialogPeer.h"

static void window_closed (GtkDialog *dialog,
                           gint responseId,
                           jobject peer_obj);
static void ok_clicked (GtkButton *button,
	                jobject peer_obj);
static void cancel_clicked (GtkButton *button,
	                    jobject peer_obj);

/*
 * Make a new file selection dialog
 */

JNIEXPORT void JNICALL 
Java_gnu_java_awt_peer_gtk_GtkFileDialogPeer_create 
  (JNIEnv *env, jobject obj)
{
  gpointer widget;

  /* Create global reference and save it for future use */
  NSA_SET_GLOBAL_REF (env, obj);

  gdk_threads_enter ();
  
  widget = gtk_file_selection_new ("");
  /* GtkFileSelect is not modal by default */
  gtk_window_set_modal (GTK_WINDOW (widget), TRUE);

  /* We must add this window to the group so input in the others are
     disable while it is being shown */
  gtk_window_group_add_window (global_gtk_window_group, GTK_WINDOW (widget));

  gdk_threads_leave ();

  NSA_SET_PTR (env, obj, widget);
}

JNIEXPORT void JNICALL
Java_gnu_java_awt_peer_gtk_GtkFileDialogPeer_connectJObject
  (JNIEnv *env, jobject obj)
{
  void *ptr = NSA_GET_PTR (env, obj);

  gdk_threads_enter ();

  gtk_widget_realize (GTK_WIDGET (ptr));

  connect_awt_hook (env, obj, 1, GTK_WIDGET (ptr)->window);

  gdk_threads_leave ();
}

JNIEXPORT void JNICALL 
Java_gnu_java_awt_peer_gtk_GtkFileDialogPeer_connectSignals
  (JNIEnv *env, jobject obj)
{
  void *ptr = NSA_GET_PTR (env, obj);
  jobject *gref = NSA_GET_GLOBAL_REF (env, obj);
  g_assert (gref);

  gdk_threads_enter ();

  gtk_widget_realize (GTK_WIDGET (ptr));

  /* connect buttons to handlers */

  g_signal_connect (G_OBJECT (GTK_DIALOG (ptr)),
                    "response", 
		    GTK_SIGNAL_FUNC (window_closed), *gref);

  g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (ptr)->ok_button),
                    "clicked", 
		    GTK_SIGNAL_FUNC (ok_clicked), *gref);

  g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (ptr)->cancel_button),
                    "clicked", 
		    GTK_SIGNAL_FUNC (cancel_clicked), *gref);
		    
  gdk_threads_leave ();

  /* Connect the superclass signals.  */
  Java_gnu_java_awt_peer_gtk_GtkComponentPeer_connectSignals (env, obj);
}

/*
 * Set the filename in the file selection dialog.
 */

JNIEXPORT void JNICALL 
Java_gnu_java_awt_peer_gtk_GtkFileDialogPeer_nativeSetFile 
    (JNIEnv *env, jobject obj, jstring filename)
{
  void *ptr;
  const char *str;

  ptr = NSA_GET_PTR (env, obj);
    
  str = (*env)->GetStringUTFChars (env, filename, 0);
     
  gdk_threads_enter ();

  gtk_file_selection_set_filename (GTK_FILE_SELECTION (ptr), str);

  gdk_threads_leave ();

  (*env)->ReleaseStringUTFChars (env, filename, str);
}

static void
window_closed (GtkDialog *dialog __attribute__((unused)),
               gint responseId,
               jobject peer_obj)
{
  static int isIDSet = 0;
  static jmethodID disposeID;
  void *ptr;

  // We only need this for the case when the user closed the window
  if (responseId != GTK_RESPONSE_DELETE_EVENT)
    return;

  ptr = NSA_GET_PTR (gdk_env, peer_obj);
  
  if (!isIDSet)
    {
      jclass cx = (*gdk_env)->GetObjectClass (gdk_env, peer_obj);
      disposeID = (*gdk_env)->GetMethodID (gdk_env, cx, "gtkDisposeFileDialog", "()V");
      isIDSet = 1;
    }
    
  gdk_threads_leave ();

  /* We can dispose of the dialog now (and unblock show) */
  (*gdk_env)->CallVoidMethod (gdk_env, peer_obj, disposeID);

  gdk_threads_enter ();
}

static void
ok_clicked (GtkButton *button __attribute__((unused)),
	                jobject peer_obj)
{
  static int isIDSet = 0;
  static jmethodID gtkSetFilenameID;
  static jmethodID hideID;
  void *ptr;
  G_CONST_RETURN gchar *fileName;

  ptr = NSA_GET_PTR (gdk_env, peer_obj);
  
  fileName = gtk_file_selection_get_filename (
               GTK_FILE_SELECTION (GTK_WIDGET (ptr)));
  
  if (!isIDSet)
    {
      jclass cx = (*gdk_env)->GetObjectClass (gdk_env, peer_obj);
      hideID = (*gdk_env)->GetMethodID (gdk_env, cx, "gtkHideFileDialog", "()V");
      gtkSetFilenameID = (*gdk_env)->GetMethodID (gdk_env, cx,
                                   "gtkSetFilename", "(Ljava.lang.String;)V");
      isIDSet = 1;
    }
    
  gdk_threads_leave ();
  
  /* Set the Java object field 'file' with this value. */
  jstring str_fileName = (*gdk_env)->NewStringUTF (gdk_env, fileName);
  (*gdk_env)->CallVoidMethod (gdk_env, peer_obj, gtkSetFilenameID, str_fileName);

  /* We can hide the dialog now (and unblock show) */
  (*gdk_env)->CallVoidMethod (gdk_env, peer_obj, hideID);

  gdk_threads_enter ();
}

static void
cancel_clicked (GtkButton *button __attribute__((unused)),
	                    jobject peer_obj)
{
  static int isIDSet = 0;
  static jmethodID gtkSetFilenameID;
  static jmethodID hideID;
  void *ptr;

  ptr = NSA_GET_PTR (gdk_env, peer_obj);
  
  if (!isIDSet)
    {
      jclass cx = (*gdk_env)->GetObjectClass (gdk_env, peer_obj);
      hideID = (*gdk_env)->GetMethodID (gdk_env, cx, "gtkHideFileDialog", "()V");
      gtkSetFilenameID = (*gdk_env)->GetMethodID (gdk_env, cx,
                                   "gtkSetFilename", "(Ljava.lang.String;)V");
      isIDSet = 1;
    }
    
  gdk_threads_leave ();

  /* Set the Java object field 'file' with the null value. */
  (*gdk_env)->CallVoidMethod (gdk_env, peer_obj, gtkSetFilenameID, NULL);

  /* We can hide the dialog now (and unblock show) */
  (*gdk_env)->CallVoidMethod (gdk_env, peer_obj, hideID);

  gdk_threads_enter ();
}


