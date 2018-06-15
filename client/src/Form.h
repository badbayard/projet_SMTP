#ifndef FORM_H
#define FORM_H

#include <gtkmm.h>
#include <iostream>
#include "Mail.h"

using namespace std;

class Form: public Gtk::Window
{
public:
	Form(Mail * mail);
	virtual ~Form();

private:
	Mail * mail;

	Glib::Thread * thread;
	Glib::Mutex mutex;
	Glib::Dispatcher sig_done;

	void display();
	void attachmentDialog();

protected:
	Gtk::Frame m_frame;

	void send();
	void run();
	void done();

	// Signal handlers:
	// Our new improved on_button_clicked(). (see below)
	void on_button_clicked(Glib::ustring data);

	// Child widgets:
	Gtk::Table m_table;
	Gtk::Label l_from, l_to, l_subject, l_attachment;
	Gtk::Entry e_from, e_to, e_subject;
	Gtk::Button b_send, b_attachment;

	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::TextView m_TextView;
	Glib::RefPtr<Gtk::TextBuffer> buffer_content;
};

#endif // FORM_H