#ifndef SIMDJSON_WESTMERE_DOM_PARSER_IMPLEMENTATION_H
#define SIMDJSON_WESTMERE_DOM_PARSER_IMPLEMENTATION_H

#include "simdjson.h"
#include "isadetection.h"

namespace simdjson {
namespace westmere {

class dom_parser_implementation final : public internal::dom_parser_implementation {
public:
  const uint8_t *buf{}; // Buffer passed to stage 1
  size_t len{0}; // Length passed to stage 1

  really_inline dom_parser_implementation();
  dom_parser_implementation(const dom_parser_implementation &) = delete;
  dom_parser_implementation &operator =(const dom_parser_implementation &) = delete;
  
  WARN_UNUSED virtual error_code parse(const uint8_t *buf, size_t len, dom::parser &parser) noexcept;
  WARN_UNUSED virtual error_code stage1(const uint8_t *buf, size_t len, dom::parser &parser, bool streaming) noexcept;
  WARN_UNUSED virtual error_code stage2(const uint8_t *buf, size_t len, dom::parser &parser) noexcept;
  WARN_UNUSED virtual error_code stage2(const uint8_t *buf, size_t len, dom::parser &parser, size_t &next_json) noexcept;
  WARN_UNUSED error_code set_capacity(size_t capacity) noexcept final;
  WARN_UNUSED error_code set_max_depth(size_t max_depth) noexcept final;
};

really_inline dom_parser_implementation::dom_parser_implementation() {}

// Leaving these here so they can be inlined if so desired
WARN_UNUSED error_code dom_parser_implementation::set_capacity(size_t) noexcept {
  return SUCCESS;
}

WARN_UNUSED error_code dom_parser_implementation::set_max_depth(size_t) noexcept {
  return SUCCESS;
}

WARN_UNUSED error_code dom_parser_implementation::parse(const uint8_t *_buf, size_t _len, dom::parser &parser) noexcept {
  error_code code = stage1(_buf, _len, parser, false);
  if (!code) {
    code = stage2(_buf, _len, parser);
  }
  return code;
}

} // namespace westmere
} // namespace simdjson

#endif // SIMDJSON_WESTMERE_DOM_PARSER_IMPLEMENTATION_H