#include <UI/UINotification.hxx>
#include <list>

namespace ui {
namespace notify {

void Publisher::subscribe(SubscriberHandle subscriber) {
  if (subscriber.expired()) {
    return;
  }
  std::string name = subscriber.lock()->identity();
  m_subscribers.emplace(name, subscriber);
}

void Publisher::unsubscribe(const std::string &subscriber) {
  m_subscribers.erase(subscriber);
}

void Publisher::notifyAll() {
  std::list<std::string> expired;

  for (auto &[name, subscriber] : m_subscribers) {
    if (subscriber.expired()) {
      expired.push_back(name);
    }
    auto owningPtr = subscriber.lock();
    if(m_state == std::nullopt){
      owningPtr->notify(identity(), std::nullopt);
    } else{
      owningPtr->notify(identity(), *m_state);
    }
  }

  for (auto &sub : expired) {
    m_subscribers.erase(sub);
  }
}

bool Publisher::updateState(const std::string &state) {
  bool update = (state != m_state);
  m_state = state;
  if (update) {
    notifyAll();
  }
  return update;
}

bool Publisher::updateState(std::optional<std::string> state) {
  bool update = (state != m_state);
  m_state = state;
  if (update) {
    notifyAll();
  }
  return update;
}

} // namespace notify
} // namespace ui