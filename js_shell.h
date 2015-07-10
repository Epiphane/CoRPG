#ifndef JS_SHELL_H
#define JS_SHELL_H

#include "duktape.h" 

class JSScript {
public:
	JSScript() : ctx(NULL), isComplete(true) {};
	JSScript (const std::string& filename);
	void init(const std::string& filename);
	~JSScript();

	virtual void run();

	void printError(const std::string &message);
	void varNotFound(const std::string &variable, const std::string reason);

	template<typename T> T get(const std::string &variable);

	bool error() { return ctx == NULL; }
protected:
	bool isComplete;

	duk_context *ctx;
	
	virtual void pre_run() {};
	virtual void post_run() {};
};

class Game;
class JSRegion : public JSScript {
public:
	JSRegion() {};
	JSRegion(const std::string& name, Game *g) : JSScript(name), game(g) {};
	void move(const std::string &region);
	void render();
	Game *game;
private:
	void pre_run();
	void post_run();
};

#endif /* JS_SHELL_H */
