// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#if !defined(AFX_PROJECTPROPERTIES_H__54C0E731_BB34_4ACE_9963_5A09A553BDF2__INCLUDED_)
#define AFX_PROJECTPROPERTIES_H__54C0E731_BB34_4ACE_9963_5A09A553BDF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectProperties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectProperties dialog

class CProjectProperties : public CDialog
{
// Construction
public:
	CProjectProperties(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectProperties)
	enum { IDD = IDD_PROJECT_PROPERTIES };
	BOOL	m_bGenerateListing;
	CString	m_strOutputDir;
	CString	m_strOutputPrefix;
	CString	m_strIntermediateDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectProperties)
	afx_msg void OnBrowseFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTPROPERTIES_H__54C0E731_BB34_4ACE_9963_5A09A553BDF2__INCLUDED_)
