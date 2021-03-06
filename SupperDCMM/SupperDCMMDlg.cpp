// SupperDCMMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SupperDCMM.h"
#include "SupperDCMMDlg.h"
#include "helping.h"
#include "ModeChooseDlg.h"
#include "exitDlg.h"
#include "mainStrShowDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSupperDCMMDlg 对话框




CSupperDCMMDlg::CSupperDCMMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSupperDCMMDlg::IDD, pParent)
	, m_strMainString(_T(""))
	, m_strSubString(_T(""))
	, m_Kernel(this )
	, m_bIsShowTree(FALSE)
	, m_nRadius(15)
	, m_bStepShow(FALSE)
	, m_bIsStepShow(FALSE)
	, m_bHaveMainStr(FALSE)
	, m_cReserveChar(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_eMode = STANDARD ;
	
}

void CSupperDCMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAINSTRING, m_strMainString);
	DDX_Text(pDX, IDC_EDIT_SUBSTRING, m_strSubString);
	DDX_Control(pDX, IDC_LIST_SUBSTRING, m_listSubString);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_CHECK_CIRCLE, m_ctrlCheck);
}

BEGIN_MESSAGE_MAP(CSupperDCMMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CSupperDCMMDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_MAINSTRING, &CSupperDCMMDlg::OnBnClickedButtonInputMainstring)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CSupperDCMMDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SUBSTRING, &CSupperDCMMDlg::OnBnClickedButtonAddSubstring)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CSupperDCMMDlg::OnBnClickedButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_TREE, &CSupperDCMMDlg::OnBnClickedButtonShowTree)
	ON_BN_CLICKED(IDC_BUTTON_STEP, &CSupperDCMMDlg::OnBnClickedButtonStep)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SUBSTRING, &CSupperDCMMDlg::OnBnClickedButtonDeleteSubstring)
	ON_BN_CLICKED(IDC_BUTTON_HELPING, &CSupperDCMMDlg::OnBnClickedButtonHelping)
	ON_BN_CLICKED(IDC_BUTTON_SET_MODE, &CSupperDCMMDlg::OnBnClickedButtonSetMode)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_NEW_TREE, &CSupperDCMMDlg::OnBnClickedButtonShowNewTree)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CSupperDCMMDlg::OnBnClickedButtonFile)
END_MESSAGE_MAP()


// CSupperDCMMDlg 消息处理程序

BOOL CSupperDCMMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_listSubString.InsertColumn(0,_T("ID"),LVCFMT_CENTER,40);
	m_listSubString.InsertColumn(1,_T("模式串"),LVCFMT_CENTER,250);

	m_listResult.InsertColumn(0,_T("起始位置"),LVCFMT_CENTER,80);
	m_listResult.InsertColumn(1,_T("所匹配的模式串"),LVCFMT_CENTER,210);

	m_listSubString.SetExtendedStyle(LVS_EX_FULLROWSELECT );
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT );

	m_listSubString.SetTextBkColor(RGB(255,255,0));
	m_listResult.SetTextBkColor(RGB(255,0,0) );

	m_listSubString.SetBkColor(RGB(255,0,255) );
	m_listResult.SetBkColor(RGB(0,255,0) );

	m_ctrlCheck.SetCheck(BST_CHECKED);

	ModeChooseDlg modeDlg ;
	if ( IDOK == modeDlg.DoModal() )
	{
		m_eMode = STANDARD ;
	}
	else
	{
		m_eMode = EXTEND ;
	}
	m_cReserveChar = modeDlg.reserveChar ;
	m_Kernel.setReserveChar( m_cReserveChar ) ;
	//GetDlgItem(IDC_STATIC_RESERVE)->
	CString tmp ;
	tmp.Format(_T("%s:%c"),_T("系统保留字符"),m_cReserveChar );
	( GetDlgItem( IDC_STATIC_RESERVE ) )->SetWindowText( tmp );


	/*CBitmap bitmap;
	bitmap.LoadBitmapW( IDB_BITMAP_COMPANY );
	bitmap.ge
	m_listSubString.SetBkImage(*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSupperDCMMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSupperDCMMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDC *dc = GetDC() ;
		CPaintDC pDC(this);

		CBitmap bitmap;
		bitmap.LoadBitmapW( IDB_BITMAP_COMPANY );
		BITMAP bmp ;
		bitmap.GetBitmap( &bmp );

		CDC dcCompatible ;
		dcCompatible.CreateCompatibleDC(dc );
		dcCompatible.SelectObject( &bitmap );
		pDC.StretchBlt(15,445,550,bmp.bmHeight/2,&dcCompatible,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY );
		dcCompatible.DeleteDC( );

		pDC.SetBkMode( TRANSPARENT ) ;

		CFont font ;
		font.CreatePointFont(150,_T("楷体_GB2312"),NULL );
		CFont *oldfont = pDC.SelectObject( &font );
		pDC.SetTextColor( RGB(255,0,0) );
		//cdc.TextOutW(0 ,0 ,_T("------------------------------------"),34),
		CString str( _T("主串：") ) ;
		if( TRUE ==  m_bHaveMainStr && m_strMainString.GetLength() < 100 )
		{
			
			str += m_strMainString ;
			pDC.TextOutW(90,50,str,str.GetLength() ) ;
		}
		//pDC->SelectObject(oldfont);
		//------------------------------------------------------------------------
		//CFont font ;
		font.DeleteObject();
		font.CreatePointFont(190,_T("楷体_GB2312"),NULL );
		oldfont = pDC.SelectObject( &font );
		pDC.SetTextColor( RGB(152,55,176) );
		//cdc.TextOutW(0 ,0 ,_T("------------------------------------"),34),
		str = _T("基于多重优化的双链多模式匹配算法演示程序") ;
		pDC.TextOutW(5,270,str,str.GetLength() ) ;
		
		if( STANDARD == m_eMode )
		{
			str.Format( _T("当前状态：标准模式匹配") ) ;
		}
		else
		{
			str.Format( _T("当前状态：扩展模式匹配") ) ;
		}
		pDC.SetTextColor( RGB(255,0,0) );
		pDC.TextOutW(270,480,str,str.GetLength() ) ;

		pDC.SelectObject(oldfont);

		if ( TRUE == m_bIsShowTree )
		{
			drawTree( &pDC );
		}
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSupperDCMMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSupperDCMMDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	exitDlg exit ;
	
	if( IDOK != exit.DoModal()  )
	{
		OnCancel();
	}
}

void CSupperDCMMDlg::OnBnClickedButtonInputMainstring()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_bHaveMainStr = TRUE ;
	int sum = m_strMainString.GetLength() ;//.GetBuffer(m_strMainString.GetLength());//.GetBuffer(0);
	wchar_t  *str = new wchar_t[ sum ] ;
	if( 0 == sum )
	{
		MessageBox(_T("对不起，请不要输入空字符串！"),_T("操作错误提示"));
		return ;
	}
	
	int i = 0;
	for( i=0;i < sum ; i++ )
	{
		str[i] = m_strMainString.GetAt(i) ;
		if( m_cReserveChar == str[i] )
		{
			CString str ;
			str.Format(_T("%s %c %s"),_T("请不要输入带有保留字符："),m_cReserveChar,_T("的字符串！！！") ) ;
			MessageBox( str ,_T("输入错误提示") );
			return ;
		}
	}
	str[i] = '\0' ;

	if( IDNO == MessageBox( _T("请问是否确认设置“")+m_strMainString+ _T("”为主串？？"),_T("添加主串提示"),MB_YESNO ) )
	{
		m_strMainString.Format( _T("") );
		UpdateData(FALSE);
		return ;
	}

	Invalidate(FALSE);
	m_Kernel.inputMainString( str ,sum ) ;
}

void CSupperDCMMDlg::OnBnClickedButtonInit()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if( IDYES != MessageBox( _T("请问是否确认初始化"),_T("系统初始化提示"), MB_YESNO ) )
	{
		return ;
	}
	m_Kernel.init( ) ;

	m_listSubString.DeleteAllItems();
	m_listResult.DeleteAllItems() ;
	
	m_bHaveMainStr = FALSE ;

	m_strMainString.Format( _T("") );
	m_strSubString.Format( _T("") );

	m_bIsShowTree = FALSE ;
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CSupperDCMMDlg::OnBnClickedButtonAddSubstring()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( TRUE ) ;

	
	int sum = m_strSubString.GetLength() ;//.GetBuffer(m_strMainString.GetLength());//.GetBuffer(0);
	wchar_t   *str = new wchar_t[sum] ;
	if(  0 == sum )
	{
		MessageBox(_T("对不起，请不要输入空字符串！"),_T("操作错误提示"));
		return ;
	}

	int i = 0;
	for( i=0;i < sum ; i++ )
	{
		str[i] = m_strSubString.GetAt( i ) ;
		if( m_cReserveChar == str[i] )
		{
			CString str ;
			str.Format(_T("%s %c %s"),_T("请不要输入带有保留字符："),m_cReserveChar,_T("的字符串！！！") ) ;
			MessageBox( str,_T("错误提示") );
			return ;
		}
	}
	str[i] = '\0' ;

	if( IDNO == MessageBox( _T("请问是否确认添加“")+m_strSubString+ _T("”为模式串？"),_T("操作错误提示"),MB_YESNO ) )
	{
		return ;
	}

	int num = m_Kernel.addSubString( str );
	CString tmp ;
	tmp.Format( _T("%d"),num );
	m_listSubString.InsertItem( 0,tmp );
	m_listSubString.SetItemText( 0,1,m_strSubString );
	m_strSubString.Format( _T("") );
	UpdateData(FALSE);

}

void CSupperDCMMDlg::OnBnClickedButtonBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	if( FALSE == m_bHaveMainStr )
	{
		MessageBox(_T("请先输入主串！！") );
		return ;
	}
	m_listResult.DeleteAllItems();
	m_Kernel.initTree();
	if( STANDARD == m_eMode )
	{
		if ( FALSE == m_Kernel.StandardBuildTree( FALSE ) )
		{
			MessageBox(_T("对不起，请不要输入相互包含的字符串！"),_T("输入错误提示"));
		}
		if ( FALSE == m_Kernel.StandardProcessTree( FALSE ) )
		{
			MessageBox(_T("对不起，请不要输入相互包含的字符串！"),_T("输入错误提示"));
		}
  		if ( FALSE == m_Kernel.StandardDoubleChainMatcher( FALSE ) )
		{
			MessageBox(_T("root 节点竟然为空，请联系 hw_herny2008@126.com ！"),_T("输入错误提示"));
		}
	}
	else
	{
		//MessageBox(_T("扩展模式匹配"),_T("ye !11"));
		m_Kernel.ExtendBuildTree(   ) ;
		m_Kernel.ExtendProcessTree(   ) ;
		m_Kernel.ExtendDoubleChainMatcher( 0  ) ;
	}
	return ;
}

void CSupperDCMMDlg::OnBnClickedButtonShowTree( )
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsStepShow = FALSE ;

	m_Kernel.initTree() ;
	if( STANDARD == m_eMode )
	{
		m_Kernel.StandardBuildTree( TRUE );
		m_Kernel.StandardProcessTree( TRUE );
	}
	else
	{
		//MessageBox(_T("扩展模式匹配！"),_T("ye"));
		m_Kernel.ExtendBuildTree(  );
		m_Kernel.ExtendProcessTree(  );
	}
	m_Kernel.setPosition( );

	m_nRadius = (m_Kernel.m_nDownLen < m_Kernel.m_nRightLen) ? m_Kernel.m_nDownLen : m_Kernel.m_nRightLen ;
	m_nRadius = m_nRadius/4 ;
	
	if ( FALSE == m_bIsShowTree )
	{
		m_bIsShowTree = TRUE ;
		Invalidate(FALSE);
		GetDlgItem(IDC_BUTTON_SHOW_TREE)->SetWindowTextW( _T("隐藏双链树") ) ;
	}
	else
	{
		m_bIsShowTree = FALSE ;
		Invalidate(TRUE ) ;
		GetDlgItem(IDC_BUTTON_SHOW_TREE)->SetWindowTextW( _T("显示双链树") ) ;
	}
	//Invalidate( true ) ;
	
	//UpdateWindow();
	return ;
}

void CSupperDCMMDlg::OnBnClickedButtonStep()
{
	// TODO: 在此添加控件通知处理程序代码
//'췍'
	//	str = str.Left(dwFileLen);
	MessageBox(_T("不好意思，由于本算法的预期用户为编程人员，所以逐步演示功能暂时没有必要实现····" ) );
	/*m_bIsShowTree = FALSE ;
	m_bStepShow = TRUE ;
	m_bIsStepShow = TRUE ;

	int i = 0 ;
	if( 0 == i )
	{
		i = 1 ;
		CDC *pDC = GetDC() ;
		drawTree( pDC );
	}*/
}

void CSupperDCMMDlg::OnBnClickedButtonDeleteSubstring()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CRect showRect ;
	GetDlgItem(IDC_STATIC_SHOW)->GetClientRect(showRect);
	CString str;
	str.Format( _T("lefttop=%d :%d   bottomright= %d :%d ") ,
					showRect.TopLeft().x ,showRect.TopLeft().y , showRect.BottomRight().x , showRect.BottomRight().y );
					
	MessageBox(str);*/

	CString str ;
	
	int i = m_listSubString.GetSelectionMark() ;
	if( -1 == i )
	{
		MessageBox(_T("对不起，请在列表框中选择要删除的列！"),_T("操作错误提示"));
		return ;
	}
	str=m_listSubString.GetItemText(i,1);
	int Len = str.GetLength();
	wchar_t *tmp =  new wchar_t[Len] ;
	
	 if(  IDYES == MessageBox(_T("请问是否确认删除：“")+str+_T("”字符串？"),_T("确认询问"),MB_YESNO) )
	 {
		 m_listSubString.DeleteItem(i);
		int i = 0;
		int sum = str.GetLength() ;
		for( i=0;i < sum ; i++ )
		{
			tmp[i] = str.GetAt( i ) ;
		}
		tmp[i] = '\0' ;
			
		m_Kernel.deleteString(tmp);
	}

}

int CSupperDCMMDlg::drawTree(CDC * pDC)
{
	pDC->SetBkMode(TRANSPARENT);

	CPen pen =  CPen( PS_SOLID,2,RGB(255,0,0) ) ;
	CPen * lpOldPen = pDC->SelectObject( &pen );

	CFont font ;
	font.CreatePointFont(200,_T("楷体_GB2312"),NULL );
	CFont *oldfont = pDC->SelectObject( &font );
	pDC->SetTextColor( RGB(0,0,255) );

	HGDIOBJ lpOldBrush = pDC->SelectObject(GetStockObject(NULL_BRUSH)) ;
	pDC->Rectangle( 20,55,80,80 );
	pDC->TextOutW(22,57,_T("root"),4) ;
	pDC->MoveTo(50,80);
	pDC->LineTo(50,90);
	
	PreOrderDraw( m_Kernel.root->child , pDC );
	Invalidate(FALSE);
	pDC->SelectObject(lpOldPen);
	pDC->SelectObject(oldfont) ;
	return 0;
}

void CSupperDCMMDlg::PreOrderDraw( LPBtree tree , CDC * pDC )
{
	if ( tree != NULL )
    {
        if( tree->data == m_cReserveChar )
        {//我不管啦！
            return ;
        }
		else
		{//if the tree is not NULL
			//  go to the son ,then the postion is number of son * step
			CString str ;
			str.Format(_T("%c"),tree->data ) ;

			
			if( BST_CHECKED == m_ctrlCheck.GetCheck() )//->IsDlgButtonChecked(IDC_CHECK_CIRCLE)  )
			{
				pDC->Ellipse(tree->point.x- m_nRadius ,tree->point.y- m_nRadius 
							,tree->point.x+ m_nRadius ,tree->point.y+ m_nRadius );
			}
			pDC->TextOutW(tree->point.x - m_nRadius+2 ,tree->point.y - m_nRadius + 2 ,str,1);
			
			if( NULL != tree->child  && m_cReserveChar != tree->child->data )
			{//if have child , then draw the line 
				CPen pen =  CPen( PS_SOLID,1,RGB(0,255,0) ) ;
				CPen * lpOldPen = pDC->SelectObject( &pen );
				CPoint tmp = tree->point ;
				tmp.y = tmp.y +  m_nRadius  ;
				pDC->MoveTo( tmp );
				tmp = tree->child->point ;
				tmp.y = tmp.y -  m_nRadius  ;
				pDC->LineTo( tmp );
				pDC->SelectObject( lpOldPen );
			}
			if ( NULL != tree->brother &&  m_cReserveChar != tree->brother->data)
			{//if have the brother ,then draw the line 
				CPen pen =  CPen( PS_SOLID,1,RGB(0,255,0) ) ;
				CPen * lpOldPen = pDC->SelectObject( &pen );
				CPoint tmp = tree->point ;
				tmp.x = tmp.x +  m_nRadius  ;
				pDC->MoveTo( tmp );
				tmp = tree->brother->point ;
				tmp.x = tmp.x -  m_nRadius  ;
				pDC->LineTo( tmp );
				pDC->SelectObject( lpOldPen );
			}
			if(  NULL != tree->brother && m_cReserveChar == tree->brother->data )
			{//if have special aid point 
				CPen pen =  CPen( PS_SOLID,2,RGB( 0,255,0 ) ) ;
				CPen * lpOldPen = pDC->SelectObject( &pen );
				CPoint tmp = tree->point ;
				tmp.x = tmp.x +  m_nRadius  ;
				pDC->MoveTo( tmp );
				tmp = tree->brother->brother->point ;
				tmp.x = tmp.x -  m_nRadius  ;
				pDC->LineTo( tmp );
				pDC->Ellipse(tmp.x-5,tmp.y-5,tmp.x+5,tmp.y+5 );
				pDC->SelectObject( lpOldPen );
			}
			
			if( TRUE == m_bIsStepShow )
			{
				while( m_bStepShow == FALSE )
				{
					::Sleep(10);
				}
				m_bStepShow = FALSE ;
				//Invalidate(FALSE );
			}
			
			PreOrderDraw( tree->child ,pDC );//seatch the left child of the tree .
			PreOrderDraw( tree->brother ,pDC );//search the right child of the tree
		}
    }
}

void CSupperDCMMDlg::OnBnClickedButtonHelping()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile file(_T("../SupperDCMM/res/Helping.txt"), CFile::modeRead  );
	wchar_t *pBuf ;
	int Len = (int)file.GetLength();
	pBuf = new wchar_t[Len+1] ;
	file.Read(pBuf,Len);
	pBuf[Len/2] = '\0' ;
	file.Close();
	//MessageBox(pBuf);

	CString str ;
	str.Format(_T("%s"),pBuf);

	helping helpDlg;
	helpDlg.setText( str );
	helpDlg.DoModal() ;

}

void CSupperDCMMDlg::OnBnClickedButtonSetMode()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	ModeChooseDlg modeDlg ;
	if ( IDOK == modeDlg.DoModal() )
	{
		m_eMode = STANDARD ;
	}
	else
	{
		m_eMode = EXTEND ;
	}

	m_cReserveChar = modeDlg.reserveChar ;
	m_Kernel.setReserveChar( m_cReserveChar ) ;

	CString tmp ;
	tmp.Format(_T("%s:%c"),_T("系统保留字符"),m_cReserveChar );
	( GetDlgItem( IDC_STATIC_RESERVE ) )->SetWindowText( tmp );


	CRect rect(270,480,550,500);
	InvalidateRect(&rect,FALSE );
}

void CSupperDCMMDlg::OnBnClickedButtonShowNewTree()
{
	// TODO: 在此添加控件通知处理程序代码

	m_Kernel.initTree() ;
	if( STANDARD == m_eMode )
	{
		m_Kernel.StandardBuildTree( TRUE );
		//m_Kernel.StandardProcessTree( FALSE );
	}
	else
	{
		m_Kernel.ExtendBuildTree(  );
		//m_Kernel.ExtendProcessTree(  );
	}
	m_Kernel.setPosition( );

	m_nRadius = (m_Kernel.m_nDownLen < m_Kernel.m_nRightLen) ? m_Kernel.m_nDownLen : m_Kernel.m_nRightLen ;
	m_nRadius = m_nRadius/4 ;
	
	if ( FALSE == m_bIsShowTree )
	{
		m_bIsShowTree = TRUE ;
		Invalidate(FALSE);
		GetDlgItem(IDC_BUTTON_SHOW_NEW_TREE)->SetWindowTextW( _T("隐藏原始双链树") ) ;
	}
	else
	{
		m_bIsShowTree = FALSE ;
		Invalidate(TRUE ) ;
		GetDlgItem(IDC_BUTTON_SHOW_NEW_TREE)->SetWindowTextW( _T("显示原始双链树") ) ;
	}
}

void CSupperDCMMDlg::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(TRUE,_T("TXT"),NULL,NULL,_T("TXT(*.txt)|*.txt"),this); 
	fileDlg.m_pOFN->lpstrTitle = _T("请选择文件的位置") ;
	fileDlg.m_ofn.lpstrInitialDir = _T("..\\SupperDCMM");
	m_bHaveMainStr = TRUE ;
	if( fileDlg.DoModal() == IDOK )
	{
		CString path = fileDlg.GetFileName() ;

		CFile file(path, CFile::modeRead  );
		wchar_t *pBuf ;
		int dwFileLen ;
		dwFileLen = (int)file.GetLength();
		pBuf = new wchar_t[dwFileLen+1] ;
		file.Read(pBuf,dwFileLen);
		//pBuf[dwFileLen/2] = '\0' ;
		file.Close();
		//MessageBox(pBuf);
		
		m_Kernel.inputMainString( pBuf , dwFileLen/2 );
		
		pBuf[dwFileLen/2] = '\0' ;
		m_strMainString.Format(_T("%s"),pBuf);
		UpdateData(FALSE );
		Invalidate(FALSE);
		
		if( m_strMainString.GetLength() >= 100 )
		{
			mainstrdlg.DestroyWindow();
			mainstrdlg.Create(IDD_DIALOG_MAINSTRING,NULL);
			mainstrdlg.setText( m_strMainString );
			mainstrdlg.ShowWindow( SW_SHOW );
		}

	}
}
