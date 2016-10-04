#pragma once

namespace wtf {
    struct menu {
    };

    namespace menus {
      struct null_menu {
        HMENU native_handle() const { return nullptr; }

        HMENU operator()() const { return nullptr; }

        operator HMENU() const { return nullptr; }
      };
    }
  }
