#include <iostream>
#include <ncurses.h>

#include "game.h"
#include "js_shell.h"
#include "ui.h"

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

int JS_setcursor(duk_context *ctx) {
	int cx = duk_get_int(ctx, 0);
	int cy = duk_get_int(ctx, 1);

	JS_window.setCursor(cx, cy);

	return 0;
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
	printError("Script execution failed [" + string(methodName) + "]: " + string(duk_to_string(ctx, -1)));\
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

	DEF_FUN("window",  JS_openWindow, DUK_VARARGS);
	DEF_FUN("print",   JS_print,      DUK_VARARGS);
	DEF_FUN("println", JS_println,    DUK_VARARGS);
	DEF_FUN("cursor",  JS_setcursor,  2);

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

int GO_set(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);
	
	std::string prop(duk_to_string(ctx, 0));
	if (isInt(prop))
		obj->set(prop, Json::Value(duk_get_int(ctx, 1)));
	else
		obj->set(prop, Json::Value(duk_get_string(ctx, 1)));

	return 0;
}

int GO_get(duk_context *ctx) {
	GameObject *obj = duk_get_this(ctx);

	std::string prop(duk_to_string(ctx, 0));
	if (prop == "level")
		duk_push_int(ctx, obj->level);
	else if (prop == "health")
		duk_push_int(ctx, obj->health);
	else if (prop == "max_health")
		duk_push_int(ctx, obj->maxhealth);
	else
		duk_push_string(ctx, obj->get(prop).asCString());

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
	DEF_OBJ_FUN(GO,       "find",  GO_find,     1);
	DEF_OBJ_FUN(GO_proto, "fetch", GO_fetch,    DUK_VARARGS);
	DEF_OBJ_FUN(GO_proto, "get",   GO_get,      1);
	DEF_OBJ_FUN(GO_proto, "set",   GO_set,      2);
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
		mvprintw(2, 0, "^-----------------------^");
		duk_dup(ctx, -2); // Queue up render()
		SAFE_PCALL(duk_pcall(ctx, 0), "render");
		duk_pop(ctx); // Discard return val
		UI::refresh();			  
	
		char in[2];
		in[0] = UI::getchar();
		in[1] = 0; // Make a "string"

		if (in[0] == 27 && !game->pause())
			isComplete = true;
		else {
			if (in[0] == 127) in[0] = '\b';

			duk_dup(ctx, -1); // Queue up render
			duk_push_string(ctx, in);
			SAFE_PCALL(duk_pcall(ctx, 1), "update");
			duk_pop(ctx); // Discard return val
		}
	}
}
