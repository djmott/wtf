/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace layouts {

    struct grid {
      enum class size_strategies {
        spring,
        fixed,
        none,
      };
      enum class halign_strategies {
        left,
        center,
        right,
      };
      enum class valign_strategies {
        top,
        center,
        bottom,
      };


      struct column {
        using vector = std::vector<column>;
        column(size_strategies WidthStrategy, halign_strategies HAlignStrategy, uint32_t Width)
          :width_strategy(WidthStrategy), halign_strategy(HAlignStrategy), width(Width){}
        column() = delete;
        ~column() = default;
        column(const column&) = default;
        size_strategies width_strategy;
        halign_strategies halign_strategy;
        uint32_t width = -1;

      };

      void add_column(size_strategies WidthStrategy = size_strategies::none, halign_strategies HAlign= halign_strategies::center, uint32_t Width = -1)
      {
        _columns.push_back(column{ WidthStrategy, HAlign, Width });
      }
      
      struct row {
        size_strategies height_strategy;
        valign_strategies valign_strategy;
        uint32_t height;
      };

      void add_row(size_strategies HeightStrategy = size_strategies::none, valign_strategies VAlign = valign_strategies::center, uint32_t Height = -1) {

      }

    protected:
      column::vector _columns;

    };
  }
}
