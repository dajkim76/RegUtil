
;-------------------------------------------------------
; 각종 매크로입니다. 여기서 필요한 부분만 수정하고 다시 컴파일 하시면 됩니다.
;
;7Zip 방식으로 압축한다.
SetCompressor /SOLID lzma
XPStyle on
CRCCheck on
ShowInstDetails show
BrandingText "EasyRegistry (www.mdiwebma.com)"

; 서비스 업체를 지정합니다.
!define _SERVICE	""

;버전 표기
!include "_version.nsh"

; 기본 설치 폴더
!define _INSDIR		"EasyRegistry"

; 프로그램 이름
!define _NAME		"EasyRegistry"


; 비스타에서는 Appdata에 설치하므로 userlevel로 실행되도록 한다. 
; 비스타가아닌 OS에서는 상관없다.
RequestExecutionLevel   admin
;--------------------------------------------------------

;
; UserInfo.dll을 이용하여 설치사용자가 관리자 권한이 있는 사용자인지 아닌지를 판단하는 
; 스크립트 함수가 구현된 파일을 include 한다.
;


; nsi파일이 있는 디렉토리에 셋업파일을 생성합니다.
OutFile ".\EasyRegistrySetup.exe"

; 기본 설치 폴더
InstallDir "$APPDATA\${_INSDIR}"

InstallDirRegKey HKLM Software\EasyRegistry ""

;--------------------------------

var /GLOBAL LTAPPDATA


; Pages

!include "MUI.nsh"
!include "Sections.nsh"

; 인스톨러의 이름을 지정합니다.
Name	"${_NAME}"
Caption "${_NAME} (ver ${ER_VERSION}) 설치"

; 라이센스 페이지
!insertmacro MUI_PAGE_LICENSE	"license.txt"
; 디렉토리 선택
!insertmacro MUI_PAGE_DIRECTORY
; 파일복사 시작
!insertmacro MUI_PAGE_INSTFILES


;마지막 설치완료되었다는 페이지 출력할려면 주석제거
;그리고 그때에 실행할 프로그램 선택가능하게 지정
!define MUI_FINISHPAGE_RUN "$INSTDIR\EasyRegistry.exe"
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE  "Korean-rul"

;--------------------------------
!include "StrFunc.nsh"
!define SHELLFOLDERS  "Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders"

Function .onInit
FunctionEnd

Function un.onInit
FunctionEnd

; The stuff to install
Section "" ;No components page, name is not important
  
  LT2ApiW::IsRunning "EasyRegistry.exe"
  Pop  $R0
  StrCmp $R0 "true" 0 ContinueGoon

SHOWBOX:
	  MessageBox MB_YESNO "EasyRegistry가 실행중입니다. 종료하겠습니까?" IDYES TERMINATE
      goto ContinueGoon #Abort "설치가 취소 되었습니다."
	TERMINATE:
      LT2ApiW::KillProcess "EasyRegistry.exe"
	  Sleep 2000
ContinueGoon:
  ;
  ; 하위폴더까지 한꺼번에 복사하기 위해서는 /r 옵션을 주면 된다. 
  ;
  
  SetOverwrite try
  SetOutPath $INSTDIR

  SetOverwrite off
  FILE "..\deploy\EasyRegistry.ini"
  FILE "..\deploy\SubMenu.ini"

  SetOverwrite on
  FILE "EasyRegistry.exe"
  FILE "EasyRegistry.txt"

  ;언인스톨러를 추가해야지 Uninstall 섹션이 동작한다.	
  WriteUninstaller "$INSTDIR\uninst.exe"
  
  WriteRegStr HKLM "Software\EasyRegistry" "" "$INSTDIR"

  ;프로그램 그룹 폴더 생성
  CreateDirectory "$SMPROGRAMS\${_NAME}"
  CreateShortCut "$SMPROGRAMS\${_NAME}\${_NAME}.lnk" "$INSTDIR\EasyRegistry.exe" ""
  CreateShortCut "$SMPROGRAMS\${_NAME}\${_NAME} 제거.lnk" "$INSTDIR\uninst.exe" ""

  CreateShortCut "$DESKTOP\${_NAME}.lnk" "$INSTDIR\EasyRegistry.exe" ""
  CreateShortCut "$QUICKLAUNCH\${_NAME}.lnk" "$INSTDIR\EasyRegistry.exe" ""  

  ; true로 하면 확인없이 바로 사라진다.
  SetAutoClose true
SectionEnd ; end the section


Section Uninstall
  ;진짜로 삭제할래?
  ;
  ;MessageBox MB_YESNO "EasyRegistry를 삭제하려고 합니다.$\r$\n$\r$\n계속 진행하겠습니가?" IDYES continue_uninstall
  ;Abort "사용자에 의해 삭제가 취소되었습니다"
  ;continue_uninstall:

  ;실행중인지 확인
  ;
  LT2ApiW::IsRunning "EasyRegistry.exe"
  Pop  $R0
  StrCmp $R0 "true" 0 ContinueGoon
  MessageBox MB_YESNO "EasyRegistry가 실행중입니다. 종료하겠습니까?" IDYES TERMINATE
  Abort "삭제가 취소 되었습니다."
TERMINATE:
  LT2ApiW::KillProcess "EasyRegistry.exe"
  Sleep 2000
ContinueGoon:


  DeleteRegKey  HKLM "SOFTWARE\EasyRegistry"
  DeleteRegKey  HKCU "SOFTWARE\EasyRegistry"

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

VIProductVersion "${ER_VERSION}"
VIAddVersionKey /LANG=${LANG_KOREAN} "ProductName" "EasyRegistry"
VIAddVersionKey /LANG=${LANG_KOREAN} "CompanyName" "김대정 www.mdiwebma.com"
VIAddVersionKey /LANG=${LANG_KOREAN} "FileDescription" "EasyRegistry: 레지스트리 즐겨찾기 툴"
VIAddVersionKey /LANG=${LANG_KOREAN} "FileVersion" "${ER_VERSION}"
VIAddVersionKey /LANG=${LANG_KOREAN} "ProductVersion" "${ER_VERSION}"

; EOF