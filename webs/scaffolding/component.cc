#include "webs/scaffolding/component.h"

void CompositeComponent::init() {
  for (const auto &component : components_) {
    component->init();
  }
}

void CompositeComponent::start() {
  for (const auto &component : components_) {
    component->start();
  }
}

void CompositeComponent::shutdown() {
  for (const auto &component : components_) {
    component->shutdown();
  }
}

void executeComponent(Component &component) {
  component.init();
  component.start();
  component.shutdown();
}