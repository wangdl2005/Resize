; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CResizeView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Resize.h"
LastPage=0

ClassCount=6
Class1=CResizeApp
Class2=CResizeDoc
Class3=CResizeView
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CInputDlg
Resource3=IDD_SIZEDLG

[CLS:CResizeApp]
Type=0
HeaderFile=Resize.h
ImplementationFile=Resize.cpp
Filter=N

[CLS:CResizeDoc]
Type=0
HeaderFile=ResizeDoc.h
ImplementationFile=ResizeDoc.cpp
Filter=N

[CLS:CResizeView]
Type=0
HeaderFile=ResizeView.h
ImplementationFile=ResizeView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_PIC_SEAMCARVE2


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_PIC_SEAMCARVE_WEIGHT




[CLS:CAboutDlg]
Type=0
HeaderFile=Resize.cpp
ImplementationFile=Resize.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_PIC_FOR_TEST
Command15=ID_PIC_RESIZE_INSIDE
Command16=ID_PIC_SEAMCARVE_WEIGHT
Command17=ID_PIC_SEAMCARVE
Command18=ID_PIC_SEAMCARVE2
Command19=ID_PIC_SEAMCARVE_SOBEL
Command20=ID_PIC_SEAMCARVE_LAPLACE
Command21=ID_PIC_SEAMCARVE_CANNY
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_SIZEDLG]
Type=1
Class=CInputDlg
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_HTO,edit,1350631553
Control4=IDC_WIDTO,edit,1350631553
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_WIDORIGIN,static,1350701057
Control12=IDC_HORIGIN,static,1350701057

[CLS:CInputDlg]
Type=0
HeaderFile=InputDlg.h
ImplementationFile=InputDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LENTO

