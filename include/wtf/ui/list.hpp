/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#if 0
namespace wtf {
  namespace policy {

    template <typename _super_t>
    struct isa_list : _super_t {
      struct item {
        item() : _text({ _T("") }) {}
        using pointer = std::shared_ptr<item>;
        tstring& text() { return _text; }
        const tstring& text() const { return _text; }
        void text(const tstring& newval) { _text = newval; }

        tstring& operator[](int index) { return _text[index]; }
        const tstring& operator[](int index) const { return _text[index]; }

      private:
        std::vector<tstring> _text;
      };
      struct header {
        using pointer = std::shared_ptr<item>;
        tstring& text() { return _text; }
        const tstring& text() const { return _text; }
        void text(const tstring& newval) { _text = newval; }
      private:
        tstring _text;

      };

    protected:
      isa_list(window * pParent) noexcept : _super_t(pParent){}
    };

  }

  namespace _ {
    template <> struct policy_traits<policy::isa_list> {
      using requires = policy_list<policy::isa_panel>;
    };
  }

  class list : public window_impl<list, policy::isa_list> {
    using _super_t = window_impl<list, policy::isa_list>;
  public:
    explicit list(window * pParent) noexcept : _super_t(pParent){}
  };

}
#endif