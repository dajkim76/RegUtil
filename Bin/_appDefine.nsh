; 기본 설치 폴더
!define _INSDIR			"EasyRegistry"

; 프로그램 이름
!define _NAME			"EasyRegistry"

; 제조사 이름
!define _BRANDINGTEXT	"EasyRegistry (김대정 www.mdiwebma.com)"
!define _COMPANYNAME	"김대정 www.mdiwebma.com"

; 실행권한 user or admin
RequestExecutionLevel   admin

; 셋업파일의 경로
!define _OUTPUT_FILENAME	".\EasyRegistrySetup.exe"

; 언인스톨 파일이름
!define _UNINST_FILENAME	"uninst.exe"

; 기본 설치폴더
!define _DEFAULT_INSTRDIR	"$APPDATA\${_INSDIR}"

; 어플 레지스트리 키
!define _APPREGKEY	"Software\EasyRegistry"

; 라이센스 파일
!define _LICENSE_FILENAME	"license.txt"

; 메인프로그램 
!define _APP_FILENAME	"EasyRegistry.exe"

; korean 을, 를
!define _KOREAN_RULEUL	"Korean-rul"

!define _CREATE_PROGRAM_GROUP
!define _CREATE_SHORTCUT_DESKTOP
!define _CREATE_SHORTCUT_QUICK

!define _SETUP_DESC	"EasyRegistry: 레지스트리 즐겨찾기 툴"