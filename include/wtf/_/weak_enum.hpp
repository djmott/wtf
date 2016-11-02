/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace _{
    /** weak_enum_class
    * there is a need to use strong enums in several places to avoid identifier collisions
    * one example is the vertical and horizontal text alignment types which both have a 'center' identifier
    * the values are different but its desirable that they remain in the same scope
    * since these types map to C weak enums and they need to be bitwise OR'd here and there
    * the weak_enum_class abuses strong typed enums and treats them like weak enums by encapsulating
    * all the casts outside the main application logic code. Dont do this at home without parental supervision.
    */
    template<typename _Ty>
    struct weak_enum{

      explicit weak_enum(_Ty init) : value(init){}

      weak_enum(const weak_enum &src) : value(src.value){}

      weak_enum() : value(static_cast<_Ty>(0)){}

      operator _Ty() const{ return value; }

      _Ty operator()() const{ return value; }

      operator _Ty &(){ return value; }

      operator const _Ty &() const{ return value; }

      _Ty &operator*(){ return value; }

      const _Ty &operator*() const{ return value; }

      weak_enum &operator=(_Ty newval){
        value = newval;
        return *this;
      }

      weak_enum &operator=(const weak_enum &src){
        value = src.value;
        return *this;
      }

      weak_enum &operator|=(_Ty newval){
        value = static_cast<_Ty>(
          static_cast<uint64_t>(value) |
          static_cast<uint64_t>(newval)
          );
        return *this;
      }

      bool operator&(_Ty val) const{
        return (static_cast<uint64_t>(value) & static_cast<uint64_t>(val)) ? true : false;
      }

      static _Ty set_flags(){ return static_cast<_Ty>(0); }

      template<typename..._TailT>
      static _Ty set_flags(_Ty head, _TailT &&...tail){
        auto vhead = static_cast<uint64_t>(head);
        auto vtail = static_cast<uint64_t>(set_flags(std::forward<_TailT>(tail)...));
        return static_cast<_Ty>(vhead | vtail);
      }

      template <typename _OtherT> _OtherT as() const{ return static_cast<_OtherT>(value); }

    private:
      _Ty value;

    };

  }
}