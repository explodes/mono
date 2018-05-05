#ifndef WEBS_SCAFFOLDING_COMPONENT_H
#define WEBS_SCAFFOLDING_COMPONENT_H

#include <vector>

class Component {
 public:
  virtual void init() = 0;
  virtual void start() = 0;
  virtual void shutdown() = 0;

  virtual ~Component() = default;
};

class CompositeComponent final : public Component {
 public:
  CompositeComponent() : components_() {}
  explicit CompositeComponent(std::vector<Component *> components)
      : components_(components) {}

  void init() override;
  void start() override;
  void shutdown() override;

 private:
  std::vector<Component *> components_;
};

void executeComponent(Component &component);

#endif