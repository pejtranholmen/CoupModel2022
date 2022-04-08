// CoupModelDoc.h : interface of the CCoupModelDoc class
//
/////////////////////////////////////////////////////////////////////////////


#pragma once

//typedef CTypedPtrArray<CPtrArray,float*> CFloatArray;
//typedef CTypedPtrArray<CPtrArray,CBase*> CBasePtrArray;

#include ".\CoupModel.h"
#include ".\NewBase\Doc.h"



class CCoupModelDoc : public CDocument
{
	friend class MyCug;
	friend class Doc;
	friend class ModelBase;
	friend class CFortranMap;
protected: // create from serialization only



	CCoupModelDoc();
	virtual ~CCoupModelDoc();
	DECLARE_DYNCREATE(CCoupModelDoc)
	
	//DocFunction m_Func;


// Attributes
public:
   // Operations
public:
	Doc m_Doc;
	CCoupModelApp* m_pApp;
	
	BOOL m_Closing;
	BOOL m_Ready;

	std::vector <void*> m_X_Annimate;
	std::vector <void*> m_T_Annimate;
	std::vector <void*> m_G_Annimate;
	std::vector <void*> m_D_Annimate;

	BOOL AddPGFile(string filename);
	BOOL SaveAndOpenDocument();
	void UpdateIniFileWithDocPath(LPCTSTR lpszPathName);
	BOOL m_ReReadMStorageFile;
	int GetDocumentType() {return m_Doc.m_DocumentType;};
	Doc* GetSimDocPointer() {return &m_Doc;};

	SimB* GetPtr(CString Name, CString Type, CString group);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoupModelDoc)
	public:

	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	//}}AFX_VIRTUAL

// Implementation
protected:


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// Generated message map functions
//{{AFX_MSG(CCoupModelDoc)
	public:
	afx_msg void OnCloseDocument();
	afx_msg BOOL OnNewDocument();
	protected:
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

