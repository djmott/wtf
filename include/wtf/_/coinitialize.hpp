/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf {
  namespace _ {
    struct coinitialize{
      static coinitialize& get() {
        static thread_local coinitialize _coinitialize;
        return _coinitialize;
      }
      ~coinitialize() { ::CoUninitialize(); 
      }
    private:
      coinitialize() {
        wtf::exception::throw_lasterr_if(CoInitializeEx(nullptr, COINIT_MULTITHREADED), [](HRESULT h) { return S_OK != h; });
      }
    };
  }
}