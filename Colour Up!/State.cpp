#include "State.h"
#include "GSM.h"

State::~State()
{
}

State::Context& State::getContext() { return gsm.context; }