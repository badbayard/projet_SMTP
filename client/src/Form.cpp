#include "Form.h"

Form::Form(Mail * mail) : b_send("(•̀ᴗ•́)و send my e-mail !"), b_attachment("add an attachment")
{
	this->mail = mail;
	display();
}

void Form::display()
{
	const int N_COLL = 20;
	const int N_ROW = 40;
	set_title("Need some help to send an e-mail ?");
	set_size_request(600,600);
	set_position(Gtk::WIN_POS_CENTER);
	set_border_width(10);

	l_from.set_text("From: ");
	l_to.set_text("To: ");
	l_subject.set_text("Subject: ");

	e_from.set_text(mail->getFrom());
	e_to.set_text(mail->getTo());
	e_subject.set_text(mail->getSubject());
	buffer_content = Gtk::TextBuffer::create();
	buffer_content->set_text(mail->getContent());
	m_TextView.set_buffer(buffer_content);

	add(m_table);
	m_table.attach(l_from,0,1,0,1,Gtk::SHRINK,Gtk::SHRINK);
	m_table.attach(e_from,1,N_COLL,0,1,Gtk::FILL,Gtk::SHRINK);

	m_table.attach(l_to,0,1,1,2,Gtk::SHRINK,Gtk::SHRINK);
	m_table.attach(e_to,1,N_COLL,1,2,Gtk::FILL,Gtk::SHRINK);
	
	m_table.attach(l_subject,0,1,3,4,Gtk::SHRINK,Gtk::SHRINK);
	m_table.attach(e_subject,1,N_COLL,3,4,Gtk::FILL,Gtk::SHRINK);

	m_table.attach(b_attachment,0,1,4,5);
	m_table.attach(l_attachment,1,N_COLL,4,5);

	m_ScrolledWindow.add(m_TextView);
	m_table.attach(m_ScrolledWindow,0,N_COLL,5,N_ROW-1);
	
	m_table.attach(b_send,0,N_COLL,N_ROW-1,N_ROW);

	m_table.set_row_spacings(5);

	b_send.signal_clicked().connect(sigc::mem_fun(*this, &Form::send));
	b_attachment.signal_clicked().connect(sigc::mem_fun(*this, &Form::attachmentDialog));

  	show_all_children();
}

void Form::attachmentDialog()
{
	Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Let's add this file (*‿*✿)", Gtk::RESPONSE_OK);

	if(dialog.run() == Gtk::RESPONSE_OK){
		l_attachment.set_text(dialog.get_filename());
		mail->setAttachment(dialog.get_filename().c_str());
	}
	else
		cout << "proooooooooooobleme" << endl;
}

void Form::send()
{
	mail->setFrom(e_from.get_text());
	mail->setTo(e_to.get_text());
	mail->setSubject(e_subject.get_text());
	mail->setContent(buffer_content->get_text());
	if(mail->ready(FORM)){
		thread = Glib::Thread::create(sigc::mem_fun(*this, &Form::run), true);
		sig_done.connect(sigc::mem_fun(*this, &Form::done));
	}
	else
		cout << "error during mail sending" << endl;
}

void Form::done()
{
	//~Form();
}

void Form::run()
{
	cout << "run" << endl;
	Glib::Mutex::Lock lock (mutex);
	b_send.set_sensitive(false);
	b_attachment.set_sensitive(false);
	e_from.set_sensitive(false);
	e_to.set_sensitive(false);
	e_subject.set_sensitive(false);
	m_TextView.set_sensitive(false);
	mail->send();
	sig_done();
}

Form::~Form()
{
	if (thread)
        thread->join();
}