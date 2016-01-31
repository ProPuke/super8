; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "qbism Super8"
#define MyAppVersion "235"
#define MyAppPublisher "qbism game engine"
#define MyAppURL "http://super8.qbism.com"
#define MyAppExeName "qbismS8.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{5637AFB8-F573-4A7E-8887-C3DF20871ADC}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\Quake
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=C:\quake\super8\docs\gnu3.txt
OutputBaseFilename=setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\quake\qbismS8.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\quake\libmad-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\quake\libogg-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\quake\libvorbis-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\quake\libvorbisfile-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\quake\super8\pak88.pak"; DestDir: "{app}\super8"; Flags: ignoreversion
Source: "C:\quake\super8\docs\*"; DestDir: "{app}\super8\docs"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
