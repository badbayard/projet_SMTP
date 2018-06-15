#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <argp.h>
#include <vector>
#include <gtkmm.h>
#include <X11/Xlib.h>
#include "Mail.h"
#include "Form.h"

using namespace std;

const char *argp_program_bug_address = "Axel LASSEUR and Yannis HUTT";
static char args_doc[] = "<server>:<port>";
static char doc[] = "A mail sender";

static struct argp_option options[] = {
	{"from", 'f', "string", 0, "Author mail address" },
	{"to", 't', "string", 0, "Dest(s) mail address(es) - to send multiple mails, just use ',' separator like this:\nexample@mail.com,example2@mail.com,etc..." },
	{"subject", 's', "string", 0, "Mail title"},
	{"content", 'c', "string", 0, "Mail content" },
	{ 0 }
};

struct arguments
{
	char *args[1];         /* arg */
	char const *from;
	char const *to;
	char const *subject;
	char const *content;
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
		 know is a pointer to our arguments structure. */
	struct arguments *arguments = (struct arguments *) state->input;

	switch (key){
		case 'f':
			arguments->from = arg;
			break;
		case 't':
			arguments->to = arg;
			break;
		case 's':
			arguments->subject = arg;
			break;
		case 'c':
			arguments->content = arg;
			break;

		case ARGP_KEY_ARG:
			if (state->arg_num >= 1)
				/* Too many arguments. */
				argp_usage (state);

			arguments->args[state->arg_num] = arg;

			break;

		case ARGP_KEY_END:
			if (state->arg_num < 1)
				/* Not enough arguments. */
				argp_usage (state);
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv){
	struct arguments arguments;

	arguments.from = "";
	arguments.to = "";
	arguments.subject = "";
	arguments.content = "";

	vector<string> dests;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	Mail * mail = new Mail(arguments.args[0]);
	mail->setFrom(arguments.from);
	mail->setTo(arguments.to);
	mail->setSubject(arguments.subject);
	mail->setContent(arguments.content);

	if(mail->ready())
		mail->send();
	else{
		Glib::thread_init();
		Gtk::Main kit(argc, argv);
		Form window(mail);
  		Gtk::Main::run(window);
	}

	return 0;
}