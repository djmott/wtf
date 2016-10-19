#pragma once

namespace wtf {

    struct message_box : MSGBOXPARAMS {
      enum class buttons {
        abort_retry_ignore = MB_ABORTRETRYIGNORE,
        cancel_retry_continue = MB_CANCELTRYCONTINUE,
        help = MB_HELP,
        ok = MB_OK,
        ok_cancel = MB_OKCANCEL,
        retry_cancel = MB_RETRYCANCEL,
        yes_no = MB_YESNO,
        yes_no_cancel = MB_YESNOCANCEL,
      };
      enum class icons {
        none = 0,
        warning = MB_ICONWARNING,
        asterisk = MB_ICONASTERISK,
        question = MB_ICONQUESTION,
        stop = MB_ICONSTOP,
      };
      enum class default_button {
        button1 = MB_DEFBUTTON1,
        button2 = MB_DEFBUTTON2,
        button3 = MB_DEFBUTTON3,
        button4 = MB_DEFBUTTON4,
      };
      enum class modality {
        application = MB_APPLMODAL,
        system = MB_SYSTEMMODAL,
        task = MB_SYSTEMMODAL,
      };
      enum class flags {
        default_desktop_only = MB_DEFAULT_DESKTOP_ONLY,
        right_justify = MB_RIGHT,
        rtl_reading = MB_RTLREADING,
        set_foreground = MB_SETFOREGROUND,
        top_most = MB_TOPMOST,
        service_notification = MB_SERVICE_NOTIFICATION,
      };
      enum class response {
        abort = IDABORT,
        cancel = IDCANCEL,
        continue_ = IDCONTINUE,
        ignore = IDIGNORE,
        no = IDNO,
        ok = IDOK,
        retry = IDRETRY,
        try_again = IDTRYAGAIN,
        yes = IDYES,
      };

      static response exec(HWND parent, const tstring &message, const tstring &caption = _T(""),
                           buttons oButtons = buttons::ok, icons oIcon = icons::none,
                           default_button oDefault = default_button::button1,
                           modality oModal = modality::application) {
        auto flags = static_cast<UINT>(oButtons);
        flags |= static_cast<UINT>(oIcon);
        flags |= static_cast<UINT>(oDefault);
        flags |= static_cast<UINT>(oModal);
        return static_cast<response>(wtf::exception::throw_lasterr_if(
          MessageBoxEx(parent, message.c_str(), caption.c_str(), flags, 0), [](int i) { return !i; }));
      }
    };
  }
