/*
 * This file is part of qkNppReverseLines plugin for Notepad++
 * Copyright (C)2019 Query Kuma <querykuma@gmail.com>
 * Copyright (C)2014 Themistokle Benetatos <info@mrt-prodz.com>
 * Copyright (C)2003 Don HO <donho@altern.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginDefinition.h"
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /*hModule*/)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
	setCommand(0, TEXT("Selection"), reverseSelection, NULL, false);
	setCommand(1, TEXT("Document"), reverseDocument, NULL, false);
	setCommand(2, TEXT("About"), aboutDlg, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

// This function returns the current scintilla
//
HWND getCurScintilla()
{
	// Return the current scintilla or NULL if it fails
	int which = -1;
	sendScintilla(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)& which);
	if (which == -1)
		return NULL;
	return (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

// This function sends message to scintilla
//
inline LRESULT sendScintilla(HWND& hCurrScintilla, UINT Msg, WPARAM wParam, LPARAM lParam)
//inline LRESULT sendScintilla(sptr_t  hCurrScintilla, unsigned int Msg, uptr_t wParam, sptr_t lParam)
{
	//	uptr_t wParam;
	return ::SendMessage(hCurrScintilla, Msg, wParam, lParam);
}

// Reverse lines of the text provided by selectedText and save it in reversedText
// eol_mode = {0: CRLF(\r\n), 1: CR(\r), 2: LF(\n)}
void reverseLines(size_t bufLength, char* selectedText, char* reversedText, size_t eol_mode)
{
	size_t cpy_len = 0;

	char eol;
	eol = (eol_mode == 1) ? '\r' : '\n';

	for (size_t i = 0; i <= bufLength - 1; i++)
	{
		size_t j = bufLength - 1 - i;
		size_t i_move = i;

		char c = selectedText[i];

		if (c == eol)
		{
			i_move -= cpy_len;

			if (eol_mode == 0)
			{
				reversedText[j++] = '\r';
				cpy_len -= 1;
			}
			reversedText[j++] = eol;

			strncpy(reversedText + j, selectedText + i_move, cpy_len);

			cpy_len = 0;
		}
		else
		{
			cpy_len++;
		}
	}

	if (cpy_len != 0)
	{
		strncpy(reversedText, selectedText + bufLength - cpy_len, cpy_len);
	}
}

// Reverse the text selection
//
void reverseSelection()
{
	HWND hCurrScintilla = getCurScintilla();
	if (!hCurrScintilla)
		return;

	// SC_EOL_CRLF (0), SC_EOL_CR (1), or SC_EOL_LF (2).
	size_t eol_mode = sendScintilla(hCurrScintilla, SCI_GETEOLMODE);

	// Get buffer length based on text selection
	size_t bufLength = sendScintilla(hCurrScintilla, SCI_GETSELTEXT);
	if (bufLength < 2)
		return;

	// Store text selection in selectedText
	char* selectedText = new char[bufLength];
	char* reversedText = new char[bufLength];
	sendScintilla(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)selectedText);

	// Reverse text and store result in reversedText
	bufLength = strlen(selectedText);
	reverseLines(bufLength, selectedText, reversedText, eol_mode);

	// Replace content and restore selection
	size_t selStart = sendScintilla(hCurrScintilla, SCI_GETSELECTIONSTART);
	size_t selEnd = sendScintilla(hCurrScintilla, SCI_GETSELECTIONEND);
	if (selEnd < selStart)
	{
		size_t tmp = selStart;
		selStart = selEnd;
		selEnd = tmp;
	}
	sendScintilla(hCurrScintilla, SCI_SETTARGETSTART, selStart);
	sendScintilla(hCurrScintilla, SCI_SETTARGETEND, selEnd);
	sendScintilla(hCurrScintilla, SCI_REPLACETARGET, bufLength, (LPARAM)reversedText);
	sendScintilla(hCurrScintilla, SCI_SETSEL, selStart, selStart + bufLength);

	delete[] reversedText;
	delete[] selectedText;
}

// Reverse each single line in the document
//
void reverseDocument()
{
	HWND hCurrScintilla = getCurScintilla();
	if (!hCurrScintilla)
		return;

	size_t curPos = sendScintilla(hCurrScintilla, SCI_GETCURRENTPOS);
	size_t text_length = sendScintilla(hCurrScintilla, SCI_GETTEXTLENGTH);

	sendScintilla(hCurrScintilla, SCI_SELECTALL);
	reverseSelection();

	sendScintilla(hCurrScintilla, SCI_SETSEL, (WPARAM)-1, text_length - curPos);
	return;
}

// Dialog About
//
void aboutDlg()
{
	::MessageBox(NULL, TEXT("Author: Query Kuma\nVersion: 1.1.0.0"), TEXT("About Reverse Lines"), MB_OK);
}
