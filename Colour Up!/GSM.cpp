#include "GSM.h"


GSM::GSM(State::Context newContext) : context(newContext) { }

void GSM::requestSet(StateID id, int level = 0) {
	requests.push_back(std::make_pair(id, SET));
}

void GSM::requestPush(StateID id, int level = 0) {
	requests.push_back(std::make_pair(id, PUSH));
}

void GSM::requestPop() {
	requests.push_back(std::make_pair(NONE, POP));
}

void GSM::requestClear() {
	requests.push_back(std::make_pair(NONE, CLEAR));
}

void GSM::applyRequests() {
	for (std::vector<std::pair<StateID, Action> >::iterator iter) { 

}