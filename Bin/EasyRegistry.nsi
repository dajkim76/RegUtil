
;-------------------------------------------------------
; 각종 매크로입니다. 여기서 필요한 부분만 수정하고 다시 컴파일 하시면 됩니다.
;

!addplugindir .\

;버전 표기
!include "_version.nsh"
!include "_appDefine.nsh"


;7Zip 방식으로 압축한다.
SetCompressor /SOLID lzma
XPStyle on
CRCCheck on
ShowInstDetails show
BrandingText "${_BRANDINGTEXT}"



; nsi파일이 있는 디렉토리에 셋업파일을 생성합니다.
OutFile "${_OUTPUT_FILENAME}"

; 기본 설치 폴더
InstallDir "${_DEFAULT_INSTRDIR}"

InstallDirRegKey HKLM "${_APPREGKEY}" "InstallDir"

; Pages
!include "MUI.nsh"
!include "Sections.nsh"

; 인스톨러의 이름을 지정합니다.
Name	"${_NAME}"
Caption "${_NAME} (ver ${_APP_VERSION}) 설치"

; 라이센스 페이지
!insertmacro MUI_PAGE_LICENSE	"${_LICENSE_FILENAME}"

; 디렉토리 선택
!insertmacro MUI_PAGE_DIRECTORY

; 파일복사 시작
!insertmacro MUI_PAGE_INSTFILES


;마지막 설치완료되었다는 페이지 출력할려면 주석제거
;그리고 그때에 실행할 프로그램 선택가능하게 지정
!define MUI_FINISHPAGE_RUN "$INSTDIR\${_APP_FILENAME}"
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE  "${_KOREAN_RULEUL}"

;--------------------------------
!include "StrFunc.nsh"
!define SHELLFOLDERS  "Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders"

Function .onInit
FunctionEnd

Function un.onInit
FunctionEnd

; The stuff to install
Section "" ;No components page, name is not important
  
  NSISApiW::IsRunning "${_APP_FILENAME}"
  Pop  $R0
  StrCmp $R0 "true" 0 ContinueGoon

SHOWBOX:
	  MessageBox MB_YESNO "${_NAME} 프로그램이 실행중입니다. 종료하겠습니까?" IDYES TERMINATE
      goto ContinueGoon #Abort "설치가 취소 되었습니다."
	TERMINATE:
      NSISApiW::KillProcess "${_APP_FILENAME}"
	  Sleep 2000
ContinueGoon:

  SetOutPath $INSTDIR

  SetOverwrite off
  FILE "..\deploy\EasyRegistry.ini"
  FILE "..\deploy\SubMenu.ini"

  SetOverwrite on
  FILE /oname=${_APP_FILENAME} "EasyRegistry.exe"
  FILE "EasyRegistry.txt"

  ;언인스톨러를 추가해야지 Uninstall 섹션이 동작한다.	
  WriteUninstaller "$INSTDIR\${_UNINST_FILENAME}"
  
  ;프로그램 그룹 폴더 생성
  !ifdef _CREATE_PROGRAM_GROUP
	  CreateDirectory "$SMPROGRAMS\${_NAME}"
	  CreateShortCut "$SMPROGRAMS\${_NAME}\${_NAME}.lnk" "$INSTDIR\${_APP_FILENAME}" ""
	  CreateShortCut "$SMPROGRAMS\${_NAME}\${_NAME} 제거.lnk" "$INSTDIR\${_UNINST_FILENAME}" ""
  !endif

  !ifdef _CREATE_SHORTCUT_DESKTOP
	  CreateShortCut "$DESKTOP\${_NAME}.lnk" "$INSTDIR\${_APP_FILENAME}" ""
  !endif

  !ifdef _CREATE_SHORTCUT_QUICK
	CreateShortCut "$QUICKLAUNCH\${_NAME}.lnk" "$INSTDIR\${_APP_FILENAME}" ""  
  !endif

  ; true로 하면 확인없이 바로 사라진다.
  SetAutoClose true
SectionEnd ; end the section


Section Uninstall
  ;진짜로 삭제할래?
  ;
  ;MessageBox MB_YESNO "${_NAME} 프로그램을 삭제하려고 합니다.$\r$\n$\r$\n계속 진행하겠습니가?" IDYES continue_uninstall
  ;Abort "사용자에 의해 삭제가 취소되었습니다"
  ;continue_uninstall:

  ;실행중인지 확인
  ;
  NSISApiW::IsRunning "${_APP_FILENAME}"
  Pop  $R0
  StrCmp $R0 "true" 0 ContinueGoon
  MessageBox MB_YESNO "${_NAME} 프로그램이 실행중입니다. 종료하겠습니까?" IDYES TERMINATE
  Abort "삭제가 취소 되었습니다."
TERMINATE:
  NSISApiW::KillProcess "${_APP_FILENAME}"
  Sleep 2000
ContinueGoon:


  DeleteRegKey  HKLM "${_APPREGKEY}"
  DeleteRegKey  HKCU "${_APPREGKEY}"

  ; 프로그램 폴더 삭제, 하위폴더도 몽땅 삭제를 위해 /r 옵션 추가
  ;
  RMDir /r "$INSTDIR"

  ; 바로가기 폴더 삭제, 하위폴더,파일 삭제를 위해 /r 옵션 추가
  ;
  RMDir /r "$SMPROGRAMS\${_NAME}"

  ; 바탕화면의 바로가기 삭제
  ;
  Delete "$DESKTOP\${_NAME}.lnk"
  Delete "$QUICKLAUNCH\${_NAME}.lnk"

  SetDetailsPrint both

  ; 이라인 주석을 제거하면 제거후 다이얼로그가 바로 사라진다.
  SetAutoClose true

SectionEnd

VIProductVersion "${_APP_VERSION}"
VIAddVersionKey /LANG=${LANG_KOREAN} "ProductName" "${_NAME}"
VIAddVersionKey /LANG=${LANG_KOREAN} "CompanyName" "${_COMPANYNAME}"
VIAddVersionKey /LANG=${LANG_KOREAN} "FileDescription" "${_SETUP_DESC}"
VIAddVersionKey /LANG=${LANG_KOREAN} "FileVersion" "${_APP_VERSION}"
VIAddVersionKey /LANG=${LANG_KOREAN} "ProductVersion" "${_APP_VERSION}"

; EOF