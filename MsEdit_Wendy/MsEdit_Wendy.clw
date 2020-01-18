; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMsEdit_WendyDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MsEdit_Wendy.h"

ClassCount=5
Class1=CMsEdit_WendyApp
Class2=CMsEdit_WendyDlg
Class3=CAboutDlg

ResourceCount=6
Resource1=IDD_READMDS
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CShowData
Resource4=IDD_E_INFO
Resource5=IDD_SHOWDATA
Class5=CEINFO
Resource6=IDR_MENU1

[CLS:CMsEdit_WendyApp]
Type=0
HeaderFile=MsEdit_Wendy.h
ImplementationFile=MsEdit_Wendy.cpp
Filter=N

[CLS:CMsEdit_WendyDlg]
Type=0
HeaderFile=MsEdit_WendyDlg.h
ImplementationFile=MsEdit_WendyDlg.cpp
Filter=D
LastObject=IDC_DELFILE
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=MsEdit_WendyDlg.h
ImplementationFile=MsEdit_WendyDlg.cpp
Filter=D
LastObject=CAboutDlg

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
ControlCount=16
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

[DLG:IDD_SHOWDATA]
Type=1
Class=CShowData
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT_CELL,edit,1082196096
Control3=IDC_LIST_DATA,SysListView32,1350631425
Control4=IDC_BTN_TIMER,button,1342242816

[CLS:CShowData]
Type=0
HeaderFile=ShowData.h
ImplementationFile=ShowData.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=IDC_LIST_DATA

[MNU:IDR_MENU1]
Type=1
Class=CShowData
Command1=ID_MENUITEM_COPY
Command2=ID_MENUITEM_INFO
CommandCount=2

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

[CLS:CEINFO]
Type=0
HeaderFile=EINFO.h
ImplementationFile=EINFO.cpp
BaseClass=CDialog
Filter=D
LastObject=CEINFO
VirtualFilter=dWC

