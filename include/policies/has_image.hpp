#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_image : _SuperT{
      virtual ~has_image() = default;

      has_image() : _SuperT()
      {
      }

      has_image(const has_image&) = delete;
      has_image(has_image&&) = delete;
      has_image &operator=(const has_image &) = delete;
      has_image &operator=(has_image&&) = delete;

    };
  }
}