#include "webs/rngapi/app.h"

void App::init() { components_.init(); }

void App::start() { components_.start(); }

void App::shutdown() { components_.shutdown(); }