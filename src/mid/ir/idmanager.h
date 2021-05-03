#ifndef RJIT_IDMANAGER_H
#define RJIT_IDMANAGER_H

#include <optional>
#include <unordered_map>
#include "usedef/value.h"

namespace RJIT::mid{

class IdManager {
private:
  std::size_t                                         _cur_id; // current id
  std::unordered_map<const Value *, std::size_t>      _ids;    // local values id
  std::unordered_map<const Value *, std::string_view> _names;  // store global variables name

public:
  IdManager() : _cur_id(0) {}

  void Reset();

  // get id for local vale
  std::size_t GetId(const Value *value);
  std::size_t GetId(const SSAPtr &value) { return GetId(value.get()); }

  // record global values(variables and functions)
  void RecordName(const Value *value, std::string_view name);

  // get name of global values
  std::optional<std::string_view> GetName(const Value *value);
  std::optional<std::string_view> GetName(const SSAPtr &value) { return GetName(value.get()); }
};

}
#endif //RJIT_IDMANAGER_H
