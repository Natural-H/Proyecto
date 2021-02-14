#include "../../include/Things/Things.hpp"

Vector<Things *> Things::things;

Things::Things(String name) : name(name) {}

Things::~Things() {}

String Things::getName() { return this->name; }

void Things::Action() {}

void Things::Action(byte args) {}