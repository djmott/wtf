/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/

#include "wtf/wtf.hpp"

//attaching lambdas to events
void LambdaExample() {
  wtf::form oForm;

  oForm.OnCreate += [](...){
    wtf::message_box::exec(nullptr, _T("This example demonstrates attaching lambdas to events with the '+=' operator"));
  };

  oForm.OnClose += [](wtf::window *, bool& cancel) {
    auto msgbox_ret = wtf::message_box::exec(nullptr, _T("The window will now close. Click OK to continue or CANCEL to abort"), _T("Events Example"), wtf::message_box::buttons::ok_cancel);
    cancel = (wtf::message_box::response::cancel == msgbox_ret);
  };

  oForm.run();
}

//attaching static methods to events
void StaticOnCreate(...) {
  wtf::message_box::exec(nullptr, _T("This example demonstrates attaching static methods to events with the '+=' operator"));
}

void StaticOnClose(wtf::window*, bool& cancel) {
  auto msgbox_ret = wtf::message_box::exec(nullptr, _T("The window will now close. Click OK to continue or CANCEL to abort"), _T("Events Example"), wtf::message_box::buttons::ok_cancel);
  cancel = (wtf::message_box::response::cancel == msgbox_ret);
}

void StaticExample() {
  wtf::form oForm;
  oForm.OnCreate += &StaticOnCreate;
  oForm.OnClose += &StaticOnClose;
  oForm.run();
}


//attaching class members to events
struct MyEventReceiver {
  void MemberOnCreate(wtf::window *) {
    wtf::message_box::exec(nullptr, _T("This example demonstrates attaching a class instance and member method to events with 'connect'"));
  }
  void MemberOnClose(wtf::window *, bool& cancel) {
    auto msgbox_ret = wtf::message_box::exec(nullptr, _T("The window will now close. Click OK to continue or CANCEL to abort"), _T("Events Example"), wtf::message_box::buttons::ok_cancel);
    cancel = (wtf::message_box::response::cancel == msgbox_ret);
  }
};

void MemberExample() {
  wtf::form oForm;
  MyEventReceiver oHandler1;
  MyEventReceiver oHandler2;
  oForm.OnCreate.connect<MyEventReceiver, &MyEventReceiver::MemberOnCreate>(&oHandler1);
  oForm.OnClose.connect<MyEventReceiver, &MyEventReceiver::MemberOnClose>(&oHandler2);
  oForm.run();
}


int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  LambdaExample();
  StaticExample();
  MemberExample();
  return 0;
}

