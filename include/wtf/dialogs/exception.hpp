/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace dialogs {

#define throw_comdlg_err_if(_test, _expression) _throw_comdlg_err_if( _test, __FILE__, __LINE__, #_test, _expression )

    struct exception : wtf::exception {
      template<typename _TestT, typename _ExprT>
      static _TestT _throw_comdlg_err_if(_TestT test, const char *sfile, int line, const char *sTest, _ExprT expr) noexcept(false) {
        if (!expr(test)) return test;
        throw exception(sfile, line, sTest, CommDlgExtendedError());
      }

      exception(const char *sfile, int iline, const char *scode, DWORD dlgerr) : wtf::exception(sfile, iline, scode, err_message(dlgerr)) {}

    protected:
      static const char * err_message(DWORD errnum) {
        static std::map<DWORD, const char *> err_msg = {
          { CDERR_DIALOGFAILURE, "The dialog box could not be created. The common dialog box function's call to the DialogBox function failed. For example, this error occurs if the common dialog box call specifies an invalid window handle." },
          { CDERR_FINDRESFAILURE, "The common dialog box function failed to find a specified resource." },
          { CDERR_INITIALIZATION, "The common dialog box function failed during initialization. This error often occurs when sufficient memory is not available." },
          { CDERR_LOADRESFAILURE, "The common dialog box function failed to load a specified resource." },
          { CDERR_LOADSTRFAILURE, "The common dialog box function failed to load a specified string." },
          { CDERR_LOCKRESFAILURE, "The common dialog box function failed to lock a specified resource." },
          { CDERR_MEMALLOCFAILURE, "The common dialog box function was unable to allocate memory for internal structures." },
          { CDERR_MEMLOCKFAILURE, "The common dialog box function was unable to lock the memory associated with a handle." },
          { CDERR_NOHINSTANCE, "The ENABLETEMPLATE flag was set in the Flags member of the initialization structure for the corresponding common dialog box, but you failed to provide a corresponding instance handle." },
          { CDERR_NOHOOK, "The ENABLEHOOK flag was set in the Flags member of the initialization structure for the corresponding common dialog box, but you failed to provide a pointer to a corresponding hook procedure." },
          { CDERR_NOTEMPLATE, "The ENABLETEMPLATE flag was set in the Flags member of the initialization structure for the corresponding common dialog box, but you failed to provide a corresponding template." },
          { CDERR_REGISTERMSGFAIL, "The RegisterWindowMessage function returned an error code when it was called by the common dialog box function." },
          { CDERR_STRUCTSIZE, "The lStructSize member of the initialization structure for the corresponding common dialog box is invalid." },
          { PDERR_CREATEICFAILURE, "The PrintDlg function failed when it attempted to create an information context." },
          { PDERR_DEFAULTDIFFERENT, "You called the PrintDlg function with the DN_DEFAULTPRN flag specified in the wDefault member of the DEVNAMES structure, but the printer described by the other structure members did not match the current default printer. This error occurs when you store the DEVNAMES structure, and the user changes the default printer by using the Control Panel." },
          { PDERR_DNDMMISMATCH, "The data in the DEVMODE and DEVNAMES structures describes two different printers." },
          { PDERR_GETDEVMODEFAIL, "The printer driver failed to initialize a DEVMODE structure." },
          { PDERR_INITFAILURE, "The PrintDlg function failed during initialization, and there is no more specific extended error code to describe the failure. This is the generic default error code for the function." },
          { PDERR_LOADDRVFAILURE, "The PrintDlg function failed to load the device driver for the specified printer." },
          { PDERR_NODEFAULTPRN, "A default printer does not exist." },
          { PDERR_NODEVICES, "No printer drivers were found." },
          { PDERR_PARSEFAILURE, "The PrintDlg function failed to parse the strings in the [devices] section of the WIN.INI file." },
          { PDERR_PRINTERNOTFOUND, "The [devices] section of the WIN.INI file did not contain an entry for the requested printer." },
          { PDERR_RETDEFFAILURE, "The PD_RETURNDEFAULT flag was specified in the Flags member of the PRINTDLG structure, but the hDevMode or hDevNames member was not NULL." },
          { PDERR_SETUPFAILURE, "The PrintDlg function failed to load the required resources." },
          { CFERR_MAXLESSTHANMIN, "The size specified in the nSizeMax member of the CHOOSEFONT structure is less than the size specified in the nSizeMin member." },
          { CFERR_NOFONTS, "No fonts exist." },
          { FNERR_BUFFERTOOSMALL, "The buffer pointed to by the lpstrFile member of the OPENFILENAME structure is too small for the file name specified by the user. The first two bytes of the lpstrFile buffer contain an integer value specifying the size required to receive the full name, in characters." },
          { FNERR_INVALIDFILENAME, "A file name is invalid." },
          { FNERR_SUBCLASSFAILURE, "An attempt to subclass a list box failed because sufficient memory was not available." },
          { FRERR_BUFFERLENGTHZERO, "A member of the FINDREPLACE structure points to an invalid buffer." }
        };
        if (err_msg.cend() == err_msg.find(errnum)) return "";
        return err_msg[errnum];
      }
    };
  }
}