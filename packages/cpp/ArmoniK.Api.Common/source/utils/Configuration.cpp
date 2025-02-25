#include "utils/Configuration.h"

#include "options/ComputePlane.h"
#include "options/ControlPlane.h"
#include "utils/JsonConfiguration.h"

namespace API_COMMON_NAMESPACE::utils {
Configuration &Configuration::add_json_configuration(std::string_view file_path) {
  JsonConfiguration::fromPath(*this, file_path);
  return *this;
}

Configuration &Configuration::add_env_configuration() {
  use_environment_ = true;
  above_env_keys_.clear();
  return *this;
}

options::ComputePlane Configuration::get_compute_plane() const { return *this; }

void Configuration::set(const Configuration &other) {
  for (auto &&[key, value] : other.list()) {
    set(key, value);
  }
}
void Configuration::set(const std::string &key, const std::string &value) {
  if (use_environment_) {
    above_env_keys_.insert(key);
  }
  options_[key] = value;
}

std::string Configuration::get(const std::string &string) const {
  if (use_environment_ && above_env_keys_.find(string) == above_env_keys_.end()) {
    char *value = std::getenv(string.c_str());
    if (value != nullptr) {
      return value;
    }
  }
  auto position = options_.find(string);
  return position == options_.end() ? "" : position->second;
}

const std::map<std::string, std::string> &Configuration::list() const { return options_; }
options::ControlPlane Configuration::get_control_plane() const { return *this; }

} // namespace API_COMMON_NAMESPACE::utils
