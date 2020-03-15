; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProgressDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "msedit_wendy.h"
LastPage=0

ClassCount=6
Class1=CEINFO
Class2=CMsEdit_WendyApp
Class3=CAboutDlg
Class4=CMsEdit_WendyDlg
Class5=CShowData

ResourceCount=6
Resource1=IDD_READMDS
Resource2=IDD_E_INFO
Resource3=IDD_SHOWDATA
Resource4=IDD_ABOUTBOX
Resource5=IDD_PROGRESS
Class6=CProgressDlg
Resource6=IDR_MENU1

[CLS:CEINFO]
Type=0
BaseClass=CDialog
HeaderFile=EINFO.h
ImplementationFile=EINFO.cpp
Filter=D
VirtualFilter=dWC
LastObject=CEINFO

[CLS:CMsEdit_WendyApp]
Type=0
BaseClass=CWinApp
HeaderFile=MsEdit_Wendy.h
ImplementationFile=MsEdit_Wendy.cpp
Filter=N
VirtualFilter=AC
LastObject=CMsEdit_WendyApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MsEdit_WendyDlg.cpp
ImplementationFile=MsEdit_WendyDlg.cpp
LastObject=CAboutDlg

[CLS:CMsEdit_WendyDlg]
Type=0
BaseClass=CDialog
HeaderFile=MsEdit_WendyDlg.h
ImplementationFile=MsEdit_WendyDlg.cpp
LastObject=CMsEdit_WendyDlg
Filter=D
VirtualFilter=dWC

[CLS:CShowData]
Type=0
BaseClass=CDialog
HeaderFile=ShowData.h
ImplementationFile=ShowData.cpp
Filter=D
VirtualFilter=dWC
LastObject=CShowData

[DLG:IDD_E_INFO]
Type=1
Class=CEINFO
ControlCount=20
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT1,edit,1350631552
Control12=IDC_EDIT2,edit,1350631552
Control13=IDC_EDIT3,edit,1350631552
Control14=IDC_EDIT4,edit,1350631552
Control15=IDC_EDIT5,edit,1350631552
Control16=IDC_EDIT6,edit,1350631552
Control17=IDC_EDIT7,edit,1350631552
Control18=IDC_EDIT8,edit,1350631552
Control19=IDC_EDIT9,edit,1350631552
Control20=IDC_EDIT10,edit,1350631552

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_READMDS]
Type=1
Class=CMsEdit_WendyDlg
ControlCount=19
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST1,listbox,1352728835
Control3=IDC_READ,button,1342242816
Control4=IDC_ADDFILE,button,1342242816
Control5=IDC_DELFILE,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_ITEM,edit,1350631552
Control11=IDC_TRANSCODE,edit,1350631552
Control12=IDC_TIME,edit,1350631552
Control13=IDC_DELAY,edit,1484849280
Control14=IDC_STATIC,static,1342308352
Control15=IDC_TIME_END,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_LOG,listbox,1353778433
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,button,1342177287

[DLG:IDD_SHOWDATA]
Type=1
Class=CShowData
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT_CELL,edit,1082196096
Control3=IDC_LIST_DATA,SysListView32,1350631425
Control4=IDC_BTN_TIMER,button,1342242816

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_MENUITEM_COPY
Command2=ID_MENUITEM_INFO
CommandCount=2

[DLG:IDD_PROGRESS]
Type=1
Class=CProgressDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PROGRESS1,msctls_progress32,1350565888
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_SIZE,static,1342308352
Control6=IDC_STATIC_READ,static,1342308352

[CLS:CProgressDlg]
Type=0
HeaderFile=ProgressDlg.h
ImplementationFile=ProgressDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CProgressDlg
VirtualFilter=dWC

