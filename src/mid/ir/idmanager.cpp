#include "idmanager.h"

namespace RJIT::mid {


void IdManager::Reset() {
  _cur_id = 0;
  _ids.clear();
}

std::size_t IdManager::GetId(const Value *value) {
  auto it = _ids.find(value);
  if (it == _ids.end()) {
    auto id = _cur_id++;
    _ids.insert({value, id});
    return id;
  } else {
    return it->second;
  }
}

void IdManager::RecordName(const Value *value, std::string_view name) {
  if (_names.find(value) == _names.end()) {
    _names.insert({value, name});
  }
}

std::optional<std::string_view> IdManager::GetName(const Value *value) {
  auto it = _names.find(value);
  if (it != _names.end()) {
    return it->second;
  } else {
    return {};
  }
}


}