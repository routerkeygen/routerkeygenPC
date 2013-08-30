; CMakeQt NSIS Config script
;
; NSIS Script by:
; Ludovic Fauvet <etix@l0cal.com>
; Rohit Yadav <rohityadav89@gmail.com>
; Rui Araújo <ruka.araujo@gmail.com>

;--------------------------------
; Definitions
;--------------------------------

# [OpenCandy]

	; The following values get passed to the OpenCandy API. Once you've signed up
	; for OpenCandy you'll be provided with a customized set of values specific to
	; your product that you must set here before releasing your installer.
	;
	; These values must be defined before including OCSetupHlp.nsh.
	;
	; For internal purposes it's okay to use the sample values that are preset
	; below. If everything is working properly you'll see a sample offer screen
	; display in your installer. When you compile your installer with these
	; sample values some warnings will be displayed in the compiler output
	; window to remind you to make changes before your public release.

	; Please change the key and secret to the ones assigned for your specific products
	; Product key and secret for first offer
	!define OC_STR_KEY "ebcebb2d8aba00cba615d7282ac47b86"
	!define OC_STR_SECRET "20001116ec44d827a37da109d4cf177e"
	; Product key and secret for second offer screen if showing two offers, otherwise
	; set these to empty strings.
	!define OC_STR_KEY2 "5108d2ea75d407c246ea47ca7186d2ae"
	!define OC_STR_SECRET2 "7635d19b9ff91a130783706127081df7"
	; Optionally change the path to OCSetupHlp.dll here if it's not in the same folder
	; as your .nsi file. You must specify the relative path from your .nsi file location.
	!define OC_OCSETUPHLP_FILE_PATH ".\OCSetupHlp.dll"

# [/OpenCandy]

# [OpenCandy]

	; The following values customize the UI for the OpenCandy loading screen.
	; The loading screen is displayed only for a limited time when there has not
	; been sufficient time between loading the OpenCandy client and end user navigation
	; to the OpenCandy offer screen to finish checking for available offers.
	; You may use LangStrings to localize messages.
	;
	; These values must be defined before including OCSetupHlp.nsh.
	;
	!define OC_LOADING_SCREEN_CAPTION " "
	!define OC_LOADING_SCREEN_DESCRIPTION " "
	!define OC_LOADING_SCREEN_MESSAGE "Loading..."
	!define OC_LOADING_SCREEN_FONTFACE "Arial"
	!define OC_LOADING_SCREEN_FONTSIZE 100

# [/OpenCandy]

!include "FileAssociation.nsh"

;--------------------------------
; General

; Name
Name "@PROJECT_NAME_SHORT@"

; Output file
OutFile "@NSIS_OUTPUT_FILE@"

VIProductVersion "@CPACK_PACKAGE_VERSION@.0"
VIAddVersionKey ProductName "@PROJECT_NAME_SHORT@"
VIAddVersionKey Comments "@PROJECT_DESCRIPTION@"
VIAddVersionKey CompanyName "Rui Araújo"
VIAddVersionKey LegalCopyright "Rui Araújo"
VIAddVersionKey FileDescription "@PROJECT_DESCRIPTION@"
VIAddVersionKey FileVersion "@CPACK_PACKAGE_VERSION@"
VIAddVersionKey ProductVersion "@CPACK_PACKAGE_VERSION@"
VIAddVersionKey InternalName "@PROJECT_NAME_SHORT@"
VIAddVersionKey LegalTrademarks "Copyright 2013 Rui Araújo"
VIAddVersionKey OriginalFilename "routerkeygen.exe"


; Get installation folder from registry if available
InstallDirRegKey HKLM "Software\@PROJECT_NAME_SHORT@" "Install_Dir"

; Install directory
InstallDir "$PROGRAMFILES\@PROJECT_NAME_SHORT@"

; Request admin permissions for Vista and higher
RequestExecutionLevel admin

; Compression method
SetCompressor /SOLID lzma

; Optimize Data Block
SetDatablockOptimize on

; Restore last write datestamp of files
SetDateSave on

; Show un/installation details
ShowInstDetails   show
ShowUnInstDetails show


;--------------------------------
; Includes
;--------------------------------

; Use Modern UI to make the installer look nice
!ifdef OPTION_USE_MUI_2
!include "MUI2.nsh"
!else
!include "MUI.nsh"
!endif

; Include Sections header so that we can manipulate
; section properties in .onInit
!include "Sections.nsh"

# [OpenCandy]
	; Include the OpenCandy Setup Helper header
	; This provides all the OpenCandy helper macros, functions
	; and definitions that are used by this install script.
	!include "OCSetupHlp.nsh"
# [/OpenCandy]

;--------------------------------
; Reserve files
;--------------------------------

!insertmacro MUI_RESERVEFILE_LANGDLL
# [/OpenCandy]
	; Improve performance by reserving an early place in
	; the file data block for OpenCandy DLL.
	!insertmacro OpenCandyReserveFile
# [/OpenCandy]
!ifndef OPTION_USE_MUI_2
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
!endif


;--------------------------------
; Modern UI Configuration
;--------------------------------

; MUI Settings
!define MUI_ABORTWARNING

; MUI Settings / Icons
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

; MUI Settings / Header
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"

; MUI Settings / Wizard
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"



;--------------------------------
; Installer pages
;--------------------------------

; Welcome page
!insertmacro MUI_PAGE_WELCOME

# [OpenCandy]
	; You must display the OpenCandy EULA during installation. We recommend
	; that you append the OpenCandy EULA to your own license agreement,
	; and that you provide clearly visible notice of the presence of the
	; OpenCandy EULA using a table of contents at the top of the
	; combined presentation.
# [/OpenCandy]
; End user license agreement
!insertmacro MUI_PAGE_LICENSE  "@CMAKE_SOURCE_DIR@/COPYING"
!insertmacro MUI_PAGE_LICENSE "OpenCandy EULA.txt"
# [OpenCandy]
	; This placeholder page supports deferred extraction and loading of the
	; OpenCandy Network Client library when using OC_INIT_PERFORM_BYPAGEORDER.
	; It will typically be inserted immediately after the license page. No
	; page is actually displayed to the end user. Inserting this page later in
	; the page list may reduce offer rate and increase the likelihood that end
	; users will see the OpenCandy loading page.
!insertmacro OpenCandyLoadDLLPage
# [/OpenCandy]

# [OpenCandy]
	; This placeholder page connects to the OpenCandy Network to check for
	; recommendations after deferred loading using OpenCandyLoadDLLPage. It will
	; typically be inserted immediately after OpenCandyLoadDLLPage. No page is
	; actually displayed to the end user. Inserting this page later in the page
	; list may reduce offer rate and increase the likelihood that end users will
	; see the OpenCandy loading page.
!insertmacro OpenCandyConnectPage
# [/OpenCandy]

; Component selection (you might want to omit this in a simpler setup)
!insertmacro MUI_PAGE_COMPONENTS

; Choose directory (you might want to omit this in a simpler setup)
!insertmacro MUI_PAGE_DIRECTORY

# [OpenCandy]
	; Insert a loading screen before the OpenCandy offer page. This screen
	; will be displayed only if the OpenCandy client is still determining
	; offer availability and for a limited period of time. Omitting this
	; page may reduce offer rate. This page must be inserted after
	; OpenCandyConnectPage and before OpenCandyOfferPage.
!insertmacro OpenCandyLoadingPage
# [/OpenCandy]

# [OpenCandy]
	; Insert the OpenCandy offer page
!insertmacro OpenCandyOfferPage
# [/OpenCandy]

# [OpenCandy]
	; Insert the second OpenCandy offer page
!insertmacro OpenCandyOfferPage2
# [/OpenCandy]

; Perform installation (executes each enabled Section)
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!insertmacro MUI_PAGE_FINISH


; Uninstall
UninstPage uninstConfirm
UninstPage instfiles


;--------------------------------
; Language support
;--------------------------------

!insertmacro MUI_LANGUAGE "English"
;!insertmacro MUI_LANGUAGE "Spanish"
;!insertmacro MUI_LANGUAGE "Portuguese"
LangString "@PROJECT_NAME_SHORT@ (required)"  ${LANG_ENGLISH} "@PROJECT_NAME_SHORT@"


;---------------------------
; Install sections
;---------------------------

# [OpenCandy]
; This section is hidden. It will always execute during installation
; but it won't appear on your component selection screen.
Section "-OpenCandyEmbedded"
	; Handle any offers the user accepted
	!insertmacro OpenCandyInstallEmbedded
SectionEnd
# [/OpenCandy]


;--------------------------------
; Installer sections

Section "@PROJECT_NAME_SHORT@ (required)" SECTIONID_MAINPRODUCT

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  File "routerkeygen.exe"
  File "@CMAKE_SOURCE_DIR@/share/routerkeygen.ico"
  File "@CMAKE_SOURCE_DIR@/COPYING"
  File "@CMAKE_SOURCE_DIR@/AUTHORS"
  File "@CMAKE_SOURCE_DIR@/NEWS"

  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\@PROJECT_NAME_SHORT@" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "DisplayName" "@PROJECT_NAME_LONG@"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "DisplayIcon" '"$INSTDIR\routerkeygen.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "DisplayVersion" "@PROJECT_VERSION@"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "Publisher" "Rui Araújo"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "URLInfoAbout" "http://code.google.com/p/android-thomson-key-solver/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  ; File association, put here:
  ;${registerExtension} "$INSTDIR\routerkeygen.exe" ".bbf" "Bla bla file format"

SectionEnd

Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\@PROJECT_NAME_LONG@"
  CreateShortCut "$SMPROGRAMS\@PROJECT_NAME_LONG@\@PROJECT_NAME_SHORT@.lnk" "$INSTDIR\routerkeygen.exe" "" "$INSTDIR\routerkeygen.ico" 0
  CreateShortCut "$SMPROGRAMS\@PROJECT_NAME_LONG@\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0

SectionEnd

Section "Desktop Shortcut"

  CreateShortCut "$DESKTOP\@PROJECT_NAME_LONG@.lnk" "$INSTDIR\routerkeygen.exe" "" "$INSTDIR\routerkeygen.ico" 0

SectionEnd

;--------------------------------
; Uninstaller sections

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\@PROJECT_NAME_SHORT@"
  DeleteRegKey HKLM "Software\@PROJECT_NAME_SHORT@"

  ; Remove files and uninstaller
  Delete "$INSTDIR\routerkeygen.exe"
  Delete "$INSTDIR\routerkeygen.ico"
  Delete "$INSTDIR\uninstall.exe"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\AUTHORS"
  Delete "$INSTDIR\NEWS"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\@PROJECT_NAME_LONG@\*.*"
  Delete "$DESKTOP\@PROJECT_NAME_LONG@.lnk"

  ; Remove directories used
  RMDir "$SMPROGRAMS\@PROJECT_NAME_LONG@"
  RMDir "$INSTDIR"

  ; Remove file association
  ;${unregisterExtension} ".bbf" "Bla bla file format"

SectionEnd



;---------------------------
; Localized descriptions
;---------------------------

; Add localized section descriptions. These appear when you hover
; the mouse over items on the component selection screen. This code has
; to come after all of your Sections.
LangString DESC_SECTIONID_MAINPRODUCT    ${LANG_ENGLISH} "Installs @PROJECT_NAME_SHORT@"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SECTIONID_MAINPRODUCT} $(DESC_SECTIONID_MAINPRODUCT)
!insertmacro MUI_FUNCTION_DESCRIPTION_END



;--------------------------------
; .onInit NSIS callback
;--------------------------------

Function .onInit
	; Display a language selection dialog box for languages
	; This will only show if you have added multiple languages
	; using the MUI_LANGUAGE macro.
	!insertmacro MUI_LANGDLL_DISPLAY

	; Set the main product section to read-only so that users
	; can't turn it off on the component selection screen.
	!insertmacro SetSectionFlag ${SECTIONID_MAINPRODUCT} ${SF_RO}

# [OpenCandy]
	; Note: If you use a language selection system,
	; e.g. MUI_LANGDLL_DISPLAY or calls to LangDLL, you must insert
	; these macros after the language selection code in order for
	; OpenCandy to detect the user-selected language and for any
	; LangStrings you may use to be applied.

	; Initialize OpenCandy
	!insertmacro OpenCandyAsyncInit "${OC_STR_KEY}"  "${OC_STR_SECRET}"  ${OC_INIT_MODE_NORMAL} \
	                                "${OC_STR_KEY2}" "${OC_STR_SECRET2}" ${OC_INIT_MODE_NORMAL} \
	                                ${OC_INIT_PERFORM_NOW}

# [/OpenCandy]
FunctionEnd



;--------------------------------
; .onInstSuccess NSIS callback
;--------------------------------

Function .onInstSuccess
# [OpenCandy]
	; Signal successful installation, download and install accepted offers
	!insertmacro OpenCandyOnInstSuccess
# [/OpenCandy]
FunctionEnd



;--------------------------------
; .onGUIEnd NSIS callback
;--------------------------------

Function .onGUIEnd
# [OpenCandy]
	; Inform the OpenCandy API that the installer is about to exit
	!insertmacro OpenCandyOnGuiEnd
# [/OpenCandy]
FunctionEnd



# [OpenCandy]
	; Have the compiler perform some basic OpenCandy API implementation checks
	!insertmacro OpenCandyAPIDoChecks
# [/OpenCandy]
