#include <Notify/Notification.hxx>
#include <list>

namespace notify {
uint64_t fnv1aHash(const std::string &s) {
  const uint64_t FNV_offset_basis = 1469598103934665603ULL;
  const uint64_t FNV_prime = 1099511628211ULL;
  uint64_t hash = FNV_offset_basis;
  for (unsigned char c : s) {
    hash ^= c;
    hash *= FNV_prime;
  }
  return hash;
}

void Publisher::subscribe(SubscriberHandle subscriber) {
  if (subscriber.expired()) {
    return;
  }
  uint64_t name = subscriber.lock()->identity();
  m_subscribers.emplace(name, subscriber);
}

void Publisher::unsubscribe(uint64_t subscriber) {
  m_subscribers.erase(subscriber);
}

void Publisher::notifyAll() {
  std::list<uint64_t> expired;

  for (auto &[id, subscriber] : m_subscribers) {
    if (subscriber.expired()) {
      expired.push_back(id);
    }
    auto owningPtr = subscriber.lock();
    if (m_state == std::nullopt) {
      owningPtr->notify(identity());
    } else {
      owningPtr->notify(identity());
    }
  }

  for (auto &sub : expired) {
    m_subscribers.erase(sub);
  }
}

bool Publisher::updateState(const std::string &state, bool forceUpdate) {
  bool update = (state != m_state) || forceUpdate;
  m_state = state;
  if (update) {
    notifyAll();
  }
  return update;
}

bool Publisher::updateState(std::optional<std::string> state,
                            bool forceUpdate) {
  bool update = (state != m_state) || forceUpdate;
  m_state = state;
  if (update) {
    notifyAll();
  }
  return update;
}

} // namespace notify