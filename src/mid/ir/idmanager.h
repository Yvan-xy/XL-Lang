#ifndef RJIT_IDMANAGER_H
#define RJIT_IDMANAGER_H

#include <optional>
#include <unordered_map>
#include "usedef/value.h"

namespace RJIT::mid{
enum class IdType {
  _ID_VAR        = 0,
  _ID_BLOCK      = 1,
  _ID_IF_COND    = 2,
  _ID_THEN       = 3,
  _ID_ELSE       = 4,
  _ID_IF_END     = 5,
  _ID_WHILE_COND = 6,
  _ID_LOOP_BODY  = 7,
  _ID_WHILE_END  = 8
};

class IdManager {
private:
  std::size_t                                         _cur_id;        // current id
  std::size_t                                         _block_id;      // current block id
  std::size_t                                         _if_cond_id;    // current cond id
  std::size_t                                         _then_id;       // current then block id
  std::size_t                                         _else_id;       // current else block id
  std::size_t                                         _if_end_id;     // current else block id
  std::size_t                                         _while_cond_id; // current cond id
  std::size_t                                         _loop_body_id;  // current loop block id
  std::size_t                                         _while_end_id;  // current while end id


  std::unordered_map<const Value *, std::size_t>      _ids;           // local values id
  std::unordered_map<const Value *, std::size_t>      _blocks;        // store blocks name
  std::unordered_map<const Value *, std::string_view> _names;         // store global variables name

  std::optional<std::size_t> findValue(const Value *value, IdType type);
public:

  IdManager()
    : _cur_id(0), _block_id(0), _if_cond_id(0), _then_id(0), _else_id(0),
      _if_end_id(0), _while_cond_id(0), _loop_body_id(0), _while_end_id(0) {}

  void Reset();

  // get id for local vale
  std::size_t GetId(const Value *value, IdType idType = IdType::_ID_VAR);
  std::size_t GetId(const SSAPtr &value, IdType idType = IdType::_ID_VAR);

  // record global values(variables and functions)
  void RecordName(const Value *value, std::string_view name);

  // get name of global values
  std::optional<std::string_view> GetName(const Value *value);
  std::optional<std::string_view> GetName(const SSAPtr &value) { return GetName(value.get()); }
};

}
#endif //RJIT_IDMANAGER_H
