#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <cstring>

#include "keys.h"
#include "benchmark.h"
#include "game.h"
#include "js_shell.h"
#include "ui.h"
#include "curl.h"

using namespace std;

Window JS_window(0, 0);
int JS_openWindow(duk_context *ctx) {
	int nargs = duk_get_top(ctx);

	int w = duk_get_int(ctx, 0);
	int h = duk_get_int(ctx, 1);
	int x = 0, y = 0;
	if (nargs >= 4) {
		x = duk_get_int(ctx, 2);
		y = duk_get_int(ctx, 3);
	}

	JS_window.init(w, h, x, y);
	return 0; /* return undefined */
}

int JS_print(duk_context *ctx) {
	int nargs = duk_get_top(ctx);
	if (nargs == 0) return 0;

	const char *_message = duk_get_string(ctx, 0);
	JS_window.print("%s", _message);

	return 0;
}

int JS_println(duk_context *ctx) {
	JS_print(ctx);
	JS_window.println();

	return 0;
}

int JS_draw(duk_context *ctx) {
	int base_x = duk_get_int(ctx, 0);
	int y = duk_get_int(ctx, 1);
	int line = 2;
	int nargs = duk_get_top(ctx);

	while (line < nargs) {
		const char *message = duk_get_string(ctx, line);
	
		char *str = new char[strlen(message) + 1];
		strcpy(str, message);

		char *token = strtok(str, "\n");
		while (token != NULL) {
			int x = base_x;
			while (token[0] != 0 && token[0] == ' ') {
				token ++;
				x ++;
			}
			
			JS_window.setCursor(x, y++);
			JS_window.printlnleft("%s", token);

			token = strtok(NULL, "\n");
		}

		free(str);
		line ++;
	}

	return 0;
}

int JS_drawImage(duk_context *ctx) {
	string path = "world/" + string(duk_get_string(ctx, 2));
	duk_pop(ctx);

	string line;
	ifstream input(path, std::ios::in);
	if (input) {
		while (getline(input, line)) {
			duk_push_string(ctx, line.c_str());
		}
	}

	return JS_draw(ctx);
}

int JS_setcursor(duk_context *ctx) {
	int cx = duk_get_int(ctx, 0);
	int cy = duk_get_int(ctx, 1);

	JS_window.setCursor(cx, cy);

	return 0;
}

int JS_modsearch(duk_context *ctx) {
	string path = "world/" + string(duk_get_string(ctx, 0));
	string file;
	ifstream module(path, std::ios::in | std::ios::binary);
	if (module) {
		module.seekg(0, std::ios::end);
		file.resize(module.tellg());
    	module.seekg(0, std::ios::beg);
    	module.read(&file[0], file.size());
    	module.close();
	}
	else {
		file = "exports.error = 'File " + path +  " not found';";
	}
	
	duk_push_string(ctx, file.c_str());

	return 1;
}

JSScript::JSScript(const std::string &filename) : isComplete(false) {
	ctx = duk_create_heap_default();
	if (filename != "")
		init(filename);
}
JSScript::~JSScript() {
	if (ctx) duk_destroy_heap(ctx);
}

void JSScript::init(const std::string &filename) {
	if (!ctx) {
		ctx = duk_create_heap_default();
	}

	if (duk_pcompile_file(ctx, 0, filename.c_str())) {
		printError("Loading " + filename + " failed: " + string(duk_safe_to_string(ctx, -1)));
		duk_destroy_heap(ctx);
		ctx = NULL;
		isComplete = true;
		return;
	}
}

void JSScript::printError(const string &message) {
	Window errWin(4 + message.size(), 7);
	errWin.printlncenter("Error");
	errWin.printlncenter(message.c_str());
	errWin.println();
	errWin.printlncenter("Press any button to continue...");

	UI::refresh();

	UI::getchar();
}

#define DEF_FUN(name, fundef, nargs) duk_push_global_object(ctx);\
	DEF_OBJ_FUN(-2, name, fundef, nargs);\
	duk_pop(ctx);
#define DEF_OBJ_FUN(index, name, fundef, nargs) duk_push_c_function(ctx, fundef, nargs);\
	duk_put_prop_string(ctx, index, name);
#define SAFE_PCALL(call, methodName) if (call) {\
	duk_get_prop_string(ctx, -1, "fileName");\
	duk_get_prop_string(ctx, -2, "lineNumber");\
	printError("Script execution failed [" + string(duk_to_string(ctx, -2)) + ":" + duk_to_string(ctx, -1) + "]: " + duk_to_string(ctx, -3));\
	duk_destroy_heap(ctx);\
	ctx = NULL;\
	return;\
	}
void JSScript::run() {
	if (!ctx) {
		printError("Script not loaded");
		isComplete = true;
		return;
	}

	DEF_FUN("window",    JS_openWindow, DUK_VARARGS);
	DEF_FUN("draw",      JS_draw,       DUK_VARARGS);
	DEF_FUN("drawImage", JS_drawImage,  3);
	DEF_FUN("print",     JS_print,      DUK_VARARGS);
	DEF_FUN("println",   JS_println,    DUK_VARARGS);
	DEF_FUN("cursor",    JS_setcursor,  2);

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "Duktape");
	duk_push_c_function(ctx, JS_modsearch, 1);
	duk_put_prop_string(ctx, -2, "modSearch");
	duk_pop_2(ctx);

	pre_run();

	SAFE_PCALL(duk_pcall(ctx, 0), "main");
	
	post_run();
}

/* GameObject and Region Stuff */
JSRegion *running_region;
inline GameObject *duk_get_this(duk_context *ctx) {
	duk_push_this(ctx);
	duk_get_prototype(ctx, -1); // Get this.__proto__
	duk_get_prop_string(ctx, -1, "valueOf");
	duk_dup(ctx, -3);
	duk_call_method(ctx, 0);
	GameObject *result = (GameObject *)duk_get_pointer(ctx, -1);
	duk_pop_3(ctx); // Pop this, this.__proto__ and result

	return result;
}

inline Json::Value duk_get_json(duk_context *ctx, duk_idx_t ind) {
	Json::Value result;

	// Get type of the current value
	duk_int_t type = duk_get_type(ctx, ind);
	switch (type) {
	case DUK_TYPE_NUMBER:
		result = duk_get_number(ctx, ind);
		break;
	case DUK_TYPE_BOOLEAN:
		result = duk_get_boolean(ctx, ind);
		break;
	case DUK_TYPE_STRING:
		result = Json::Value(duk_get_string(ctx, ind));
		break;
	case DUK_TYPE_OBJECT:
		// Recursive case is fun case
		duk_enum(ctx, ind, DUK_ENUM_OWN_PROPERTIES_ONLY);
		while (duk_next(ctx, -1, 1)) {
			string prop = duk_get_string(ctx, -2);
			Json::Value val = duk_get_json(ctx, -1);

			result[prop] = val;

			duk_pop_2(ctx);
		}
		duk_pop(ctx);
		break;
	default:
		break;
	}

	return result;
}

bool isInt(string prop) {
	return prop == "level" || prop == "health" || prop == "max_health";
}

int GO_create(duk_context *ctx) {
	Json::Value defaults;
	duk_enum(ctx, 1, DUK_ENUM_OWN_PROPERTIES_ONLY);
	while (duk_next(ctx, -1, 1)) {
		string prop = duk_get_string(ctx, -2);
		if (isInt(prop))
			defaults[prop] = duk_get_int(ctx, -1);
		else
			defaults[prop] = string(duk_get_string(ctx, -1));

		duk_pop_2(ctx); // Remove values
	}
	duk_pop(ctx);

	duk_push_pointer(ctx, running_region->game->getOrBuild(duk_to_string(ctx, 0), defaults));
	duk_to_object(ctx, -1);
	duk_eval_string(ctx, "GameObject.prototype");
	duk_set_prototype(ctx, -2);

	return 1;
}

int GO_find(duk_context *ctx) {
	duk_push_pointer(ctx, running_region->game->getObject(duk_to_string(ctx, 0)));
	duk_to_object(ctx, -1);
	duk_eval_string(ctx, "GameObject.prototype");
	duk_set_prototype(ctx, -2);

	return 1;
}

int GO_fetch(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);
	obj->fetch();

	return 0;
}

int GO_save(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);
	obj->save();

	return 0;
}

int GO_damage(duk_context *ctx) {
	GameObject *fighter = duk_get_this(ctx);
	duk_get_prototype(ctx, 0); // Get this.__proto__
	duk_get_prop_string(ctx, -1, "valueOf");
	duk_dup(ctx, 0); // Put object on top of stack
	duk_call_method(ctx, 0);
	GameObject *victim = (GameObject *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx); // Pop victim.__proto__ and result
	
	// Do the damage
	fighter->damage(victim, duk_get_int(ctx, 1));

	return 0;
}

int GO_act(duk_context *ctx) {
	GameObject *self = duk_get_this(ctx);
	GameObject *other = NULL;

	int nargs = duk_get_top(ctx);
	if (nargs == 2) {
		duk_get_prototype(ctx, 0); // Get other.__proto__
		duk_get_prop_string(ctx, -1, "valueOf");
		duk_dup(ctx, 0); // Put object on top of stack
		duk_call_method(ctx, 0);
		other = (GameObject *)duk_get_pointer(ctx, -1);
		duk_pop_2(ctx); // Pop victim.__proto__ and result
	}

	Json::Value action = duk_get_json(ctx, nargs - 1);
	
	// Do the damage
	self->act(other, action);

	return 0;
}

int GO_set(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);

	Json::Value upd;
	if (duk_get_top(ctx) == 1) {
		upd = duk_get_json(ctx, 0);
	}
	else {
		std::string prop(duk_get_string(ctx, 0));
		
		if (isInt(prop))
			upd[prop] = Json::Value(duk_get_int(ctx, 1));
		else
			upd[prop] = Json::Value(duk_get_string(ctx, 1));
	}
	
	obj->update(upd);

	return 0;
}

int GO_get(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);

	std::string prop(duk_to_string(ctx, 0));
	Json::Value val = obj->get(prop);
	if (isInt(prop))
		duk_push_number(ctx, val.asInt());
	else
		duk_push_string(ctx, val.asCString());

	return 1;
}

int GO_viewInfo(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);

	std::string title(obj->name);

	int nargs = duk_get_top(ctx);
	if (nargs >= 1) title = string(duk_to_string(ctx, 0));

	obj->infoPage(title);

	return 0;
}

void JSRegion::move(const string &next) {
	game->setRegion(next);
	isComplete = true;
};

int JS_move(duk_context *ctx) {
	running_region->move(string(duk_to_string(ctx, 0)));
	return 0;
}

int JS_depend(duk_context *ctx) {
	int nargs = duk_get_top(ctx);
	for (int i = 0; i < nargs; i ++) {
		// Pop all arguments and use them
		string dep = duk_to_string(ctx, i);
		running_region->game->addDependency(dep);
	}

	return 0;
}

void JSRegion::pre_run() {
	running_region = this;

	// Define global functions
	DEF_FUN("move", JS_move, 1);
	DEF_FUN("depend", JS_depend, DUK_VARARGS);

	// Create GameObject prototype
	duk_push_global_object(ctx);             // Save for later
	duk_eval_string(ctx, "Duktape.Pointer.prototype");
	duk_enum(ctx, -1, DUK_ENUM_INCLUDE_NONENUMERABLE); // Enumerate prototype

	duk_push_global_object(ctx);             // Save for later
	duk_idx_t GO       = duk_push_c_function(ctx, GO_create, 2);
	duk_idx_t GO_proto = duk_push_object(ctx);
	while (duk_next(ctx, -4, 1)) {           // for (key in Duktape.Pointer.prototype)
		duk_put_prop(ctx, -3);                //    new_proto[key] = Duktape.Pointer.prototype[key]
	}
	DEF_OBJ_FUN(GO_proto, "fetch", GO_fetch,    0);
	DEF_OBJ_FUN(GO_proto, "sync",  GO_save,     0);
	DEF_OBJ_FUN(GO_proto, "get",   GO_get,      1);
	DEF_OBJ_FUN(GO_proto, "set",   GO_set,      2);
	DEF_OBJ_FUN(GO_proto, "set",   GO_set,      1);
	DEF_OBJ_FUN(GO_proto, "damage",GO_damage,   2);
	DEF_OBJ_FUN(GO_proto, "act",   GO_act,      1);
	DEF_OBJ_FUN(GO_proto, "actOn", GO_act,      2);
	DEF_OBJ_FUN(GO_proto, "info",  GO_viewInfo, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "prototype");  // Define GameObject.prototype
	duk_put_prop_string(ctx, -2, "GameObject"); // Define it globally
	duk_pop_n(ctx, 3);                          // Pop back to global context

	// Current stack: |...|global
	duk_push_pointer(ctx, game->getPlayer());
	duk_to_object(ctx, -1);
	duk_eval_string(ctx, "GameObject.prototype");
	duk_set_prototype(ctx, -2);
	// Define it globally
	duk_put_prop_string(ctx, -2, "player");
	duk_pop(ctx); // Pop global context
}

#define GET_AND_CHECK(name, objndx) if (!duk_get_prop_string(ctx, objndx, name)) {\
		isComplete = true;\
		printError(string(name) + " function is not defined");\
		return;\
	}
void JSRegion::post_run() {
	// Get render and update functions
	duk_push_global_object(ctx);
	GET_AND_CHECK("render", -1);
	GET_AND_CHECK("update", -2);

	while (!isComplete) {
		UI::clear();
		mvprintw(0, 0, ".-----------------------.");
		mvprintw(1, 0, "| Press ESC to pause... |");
		mvprintw(2, 0, "|                       |");
		mvprintw(3, 0, "| Latency: %.3fms      |", Curl::getLatency());
		mvprintw(4, 0, "^-----------------------^");
		duk_dup(ctx, -2); // Queue up render()
		SAFE_PCALL(duk_pcall(ctx, 0), "render");
	
		duk_pop(ctx); // Discard return val
		UI::refresh();			  

		int ch = UI::getchar();

		bool should_update = true;
			
		duk_dup(ctx, -1); // Queue up update

		if (ch == 27) {
			timeout(0);
			ch = UI::getchar();
			timeout(-1);
			if (ch == ERR) {
				should_update = false;

				if (!game->pause())
					isComplete = true;
			}
			else {
				ch = UI::getchar();

				if (ch == K_LEFT) {
					duk_push_string(ctx, "left");
				}
				else if (ch == K_UP) {
					duk_push_string(ctx, "up");
				}
				else if (ch == K_DOWN) {
					duk_push_string(ctx, "down");
				}
				else if (ch == K_RIGHT) {
					duk_push_string(ctx, "right");
				}
				else {
					should_update = false;
				}
			}
		}
		else {
			if (ch == 127) ch = '\b';
			char in[2];
			in[0] = ch;
			in[1] = 0; // Make a "string"

			duk_push_string(ctx, in);
		}

		if (should_update) {
			SAFE_PCALL(duk_pcall(ctx, 1), "update");
		}
		duk_pop(ctx); // Discard return val (or function ref)
	}
}
