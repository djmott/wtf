/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{
  namespace _{

    template <typename _ImplT, WNDPROC window_proc>
    struct window_class_ex : WNDCLASSEX{

      static window_class_ex& get(){
        static window_class_ex _window_class_ex;
        return _window_class_ex;
      }

      LPCTSTR name() const { return _class_name.c_str(); }

      ~window_class_ex(){ UnregisterClass(_class_name.c_str(), instance_handle()); }
    private:
      window_class_ex(){
        memset(this, 0, sizeof(WNDCLASSEX));
        //this goofy looking bit of code creates a unique class name in unicode or multibyte
        std::string sTemp = "wtf:" + std::to_string(typeid(_ImplT).hash_code());
        std::copy(sTemp.begin(), sTemp.end(), std::back_inserter(_class_name));
        cbSize = sizeof(WNDCLASSEX);
        style = CS_OWNDC | CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        lpfnWndProc = window_proc;
        cbClsExtra = sizeof(void*);
        hInstance = instance_handle();
        lpszClassName = name();
        exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x){ return 0 == x; });
      }

      tstring _class_name;
    };

  }
}