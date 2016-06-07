#pragma once


#include "State.h"
#include <stack>
#include <vector>
#include <utility>

class GSM : public std::stack<State*> {

private:
	enum Action { 
		POP,
		PUSH,
		SET,
		CLEAR
	};

public:
	GSM(State::Context newContext);

	void requestSet(StateID id, int level = 0);
	void requestPush(StateID id, int level = 0);
	void requestPop();
	void requestClear();

	void applyRequests();

	void setState(State *s) {
		pop();
		std::stack<State*>::push(s);
	}

	void push(State *s) {
		std::stack<State*>::push(s);
	}

	void pop() {
		delete top();
		std::stack<State*>::pop();
	}

	void processKeys(bool *keySet) {
		top()->processKeys(keySet);
	}

	void update(const double dt) {
		top()->update(dt);
	}

	void render() {
		top()->render();
	}

//private: 
	State::Context context;
	std::vector<std::pair<StateID, Action> > requests;
	int level;

};

