#pragma once

#include <dim/dic.hxx>
#include <thread>

#include "Subscriber.hxx"

namespace dim_handlers {
class ServiceInfo : public Subscriber, public DimStampedInfo {
public:
  ServiceInfo(std::string name, std::optional<std::string> alias,
              std::optional<uint32_t> timeout = std::nullopt)
      : Subscriber(name, alias, Type::ServiceInfo, timeout),
        DimStampedInfo(const_cast<char *>(name.c_str()),
                       const_cast<char *>(SERVICE_NO_LINK.data())) {}

  void infoHandler() override {
    std::string newData = getString();
    LOG_SERVICE(DEBUG) << "Received new data";

    if (newData != SERVICE_NO_LINK) {
      handleNewData(newData, getTimestamp(), getTimestampMillisecs());
    }
  }

  std::optional<std::string> waitForData() override {
    LOG_SERVICE(DEBUG) << " - waiting for data; timeout: " << timeout();
    size_t previouseDataSize = availableData();
    auto startTime = std::chrono::high_resolution_clock::now();
    while (availableData() == previouseDataSize) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      if (checkTimeout(startTime, std::chrono::high_resolution_clock::now())) {
        LOG_SERVICE(DEBUG) << " - timeout reached";
        return std::nullopt;
      }
    }
    return {Subscriber::getData()};
  }

private:
  static constexpr std::string_view SERVICE_NO_LINK = "<!>NO_LINK<!>";
};

} // namespace dim_handlers