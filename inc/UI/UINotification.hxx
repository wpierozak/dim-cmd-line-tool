#pragma once
#include "UITools.hxx"
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
namespace ui {
namespace notify {

class Node {
public:
  Node(const std::string &identity_) : m_identity(identity_) {}
  std::string identity() { return m_identity; }
  virtual ~Node() = default;

private:
  std::string m_identity;
};

class Subscriber : virtual public Node {
public:
  Subscriber(const std::string &identity_) : Node(identity_) {}
  virtual void notify(const std::string &publisher, opt_str_ref context) = 0;
  virtual ~Subscriber() = default;
};

typedef std::weak_ptr<Subscriber> SubscriberHandle;

class Publisher : virtual public Node {
public:
  Publisher(const std::string &identity_) : Node(identity_) {}

  void subscribe(SubscriberHandle subscriber);
  void unsubscribe(const std::string &subscriber);
  void updateAlways(bool on) { m_updateAlways = on; }

  virtual void evaluateState() = 0;
  virtual ~Publisher() = default;

protected:
  void notifyAll();
  bool updateState(const std::string &state);
  bool updateState(std::optional<std::string> state);

private:
  std::optional<std::string> m_state;
  std::map<std::string, SubscriberHandle> m_subscribers;
  bool m_updateAlways{false};
};

} // namespace notify
} // namespace ui