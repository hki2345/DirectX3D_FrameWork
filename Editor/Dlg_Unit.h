#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_Unit ��ȭ �����Դϴ�.

class Renderer_BonAni;
class Force_Unit;
class Dlg_Unit : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Unit)

public:
	Dlg_Unit(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dlg_Unit();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITDLG };
#endif

protected:
	void Update_RscTree();
	void Update_AddModel();
	void Update_DelModel();
	void Update_SelectList();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_KM3List;
	CListBox m_SelectList;

	KPtr<TheOne>						m_CurOne;
	KPtr<Force_Unit>					m_CurUnit;
	std::wstring						m_CurStr;

	std::list<KPtr<Renderer_BonAni>>	m_RenderList;
	std::list<KPtr<Renderer_BonAni>>::iterator m_SRI;
	std::list<KPtr<Renderer_BonAni>>::iterator m_ERI;


	CEdit InfoValue[6];
	CButton WeaponType[3];


	virtual BOOL OnInitDialog();
	void Init_Dlg();

	afx_msg void OnBnClickedReninsertbtn();
	afx_msg void OnBnClickedRendeletebtn();
	afx_msg void OnBnClickedUnitloadbtn();
	afx_msg void OnBnClickedUnitsavebtn();
	afx_msg void OnLbnSelchangeUnitkm3list();
	afx_msg void OnBnClickedRenrestartbtn();
	afx_msg void UnitInfoSelchange(UINT _Id);
	afx_msg void UnitWeaponSelchange(UINT _Id);
};