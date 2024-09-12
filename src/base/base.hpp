#ifndef PRIMER_H
#define PRIMER_H

#include <string>
#include <vector>

namespace ew {

class base {
public:
  base() {};
  virtual auto main(int argv, char *argc[]) -> int           = 0;
  virtual auto main(std::vector<std::string> &params) -> int = 0;
  virtual auto size() -> unsigned long { return sizeof(*this); };
  virtual auto name() -> const std::string        & = 0;
  virtual auto help() -> void                       = 0;
  virtual auto setAppName(std::string name) -> void = 0;

private:
  std::string              app_name;
  std::vector<std::string> app_description;
};

}  // namespace ew
#endif  // PRIMER_H
