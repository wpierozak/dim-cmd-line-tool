#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
namespace notify {

#include <string>

uint64_t fnv1aHash(const std::string &s);

class Node {
public:
  Node(uint64_t identity_) : m_identity(identity_) {}
  uint64_t identity() { return m_identity; }
  virtual ~Node() = default;

private:
  uint64_t m_identity;
};

class Subscriber : virtual public Node {
public:
  Subscriber(uint64_t identity_) : Node(identity_) {}
  virtual void notify(uint64_t publisher) = 0;
  virtual ~Subscriber() = default;
};

typedef std::weak_ptr<Subscriber> SubscriberHandle;

class Publisher : virtual public Node {
public:
  Publisher(uint64_t identity_) : Node(identity_) {}

  void subscribe(SubscriberHandle subscriber);
  void unsubscribe(uint64_t subscriber);

  virtual void evaluateState() {};
  virtual ~Publisher() = default;

protected:
  void notifyAll();
  bool updateState(const std::string &state, bool forceUpdate = false);
  bool updateState(std::optional<std::string> state, bool forceUpdate = false);

private:
  std::optional<std::string> m_state;
  std::map<uint64_t, SubscriberHandle> m_subscribers;
  bool m_updateAlways{false};
};

} // namespace notify