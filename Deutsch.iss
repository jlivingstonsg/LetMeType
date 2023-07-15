; This is a script for Inno Setup, which can be downloaded for free
; from http://www.jrsoftware.org

[Setup]
AppName=LetMeType
AppVerName=LetMeType deutsch
AppCopyright=Copyright © 2002 Carsten Clasohm
DefaultDirName={pf}\LetMeType
DefaultGroupName=LetMeType
MinVersion=4,4
SourceDir=dist.de
OutputDir=install
OutputBaseFilename=Setup
AlwaysCreateUninstallIcon=1
AllowNoIcons=1
DisableStartupPrompt=1
MessagesFile=compiler:German-2.isl
WizardImageFile=compiler:WizModernImg01.bmp
ChangesAssociations=1

[Files]
Source: "Versionen.txt"; DestDir: "{app}"
Source: "gpl.txt"; DestDir: "{app}"
Source: "LMTHook.dll"; DestDir: "{app}"; CopyMode: alwaysskipifsameorolder; Flags: comparetimestampalso
Source: "LetMeType.cnt"; DestDir: "{app}"
Source: "LetMeType.exe"; DestDir: "{app}"; CopyMode: alwaysskipifsameorolder; Flags: comparetimestampalso
Source: "LetMeType.hlp"; DestDir: "{app}"
Source: "LiesMich.txt"; DestDir: "{app}"
Source: "standard.lmt"; DestDir: "{app}"; CopyMode: onlyifdoesntexist; Flags: uninsneveruninstall

[Icons]
Name: "{userstartup}\LetMeType"; Filename: "{app}\standard.lmt"
Name: "{group}\LetMeType"; Filename: "{app}\standard.lmt"
Name: "{group}\Hilfe"; Filename: "{app}\LetMeType.hlp"
Name: "{group}\LiesMich"; Filename: "{app}\LiesMich.txt"

[Registry]
Root: HKCU; Subkey: "Software\ccl Software"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\ccl Software\LetMeType"; Flags: uninsdeletekey
Root: HKCR; Subkey: ".lmt"; ValueType: string; ValueName: ""; ValueData: "LetMeTypeVocabulary"; Flags: uninsdeletekey
Root: HKCR; Subkey: "LetMeTypeVocabulary"; ValueType: string; ValueName: ""; ValueData: "LetMeType Wortschatz"; Flags: uninsdeletekey
Root: HKCR; Subkey: "LetMeTypeVocabulary\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\LetMeType.exe,0"
Root: HKCR; Subkey: "LetMeTypeVocabulary\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\LetMeType.exe"" /dde"
Root: HKCR; Subkey: "LetMeTypeVocabulary\shell\open\ddeexec"; ValueType: string; ValueName: ""; ValueData: "[open(""%1"")]"

[Run]
Filename: "{app}\standard.lmt"; Description: "LetMeType starten"; Flags: postinstall nowait shellexec

