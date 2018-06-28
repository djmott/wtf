/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

    struct icon : std::shared_ptr<HICON__> {
      enum class system_icons {
        application = 32512,
        asterisk = 32516,
        exclamation = 32515,
        hand = 32513,
        question = 32514,
        winlogo = 32517,
        shield = 32518,
      };

      static icon from_system(system_icons Style)  {
        return icon(
          wtf::exception::throw_lasterr_if(::LoadIcon(nullptr, MAKEINTRESOURCE(Style)), [](HICON h) noexcept { return !h; }),
          [](HICON) noexcept {});
      }

      static icon from_resource(const tstring& ResourceName)  {
        return icon(
          wtf::exception::throw_lasterr_if(::LoadIcon(instance_handle(), ResourceName.c_str()), [](HICON h) noexcept { return !h; }),
          [](HICON)noexcept {});
      }
      static icon from_resource(int ResourceID)  {
        return icon(
          wtf::exception::throw_lasterr_if(::LoadIcon(instance_handle(), MAKEINTRESOURCE(ResourceID)), [](HICON h) noexcept { return !h; }),
          [](HICON)noexcept {});
      }

      icon() = delete;
      ~icon() = default;

      icon(const icon&) = delete;
      icon &operator=(const icon &) = delete;

      icon(icon &&src) : shared_ptr(std::move(src)) {}

      icon &operator=(icon &&src) noexcept {
        shared_ptr::swap(src);
        return *this;
      }

      operator HICON() const noexcept { return get(); }

    protected:
      template<typename ... _ArgTs>
      icon(_ArgTs &&...oArgs)  : shared_ptr(std::forward<_ArgTs>(oArgs)...) {}
    };
  }

