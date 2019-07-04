# Notepad++ Plugin: Qk Reverse Lines

#### Reverse lines in the selection or complete document.

Qk Reverse Lines for Notepad++ is a very simple plugin made to help you reverse lines.

## Features:

So far this plugin will:

- reverse lines in the selected text content
- reverse all lines in your document

## Usage:

##### Reverse Lines in the Selection

Select some text in your document and click on Plugins -> Reverse Lines -> Selection.

##### Reverse All Lines in the Document

You don't need to make any selection or positioning for the caret, just click on Plugins -> Reverse Lines -> Document.

## Screenshot:

![Notepad++ Plugin: Qk Reverse Lines](https://raw.githubusercontent.com/querykuma/qkNppReverseLines/master/screenshot.png)

## Installation:

To install the plugin manually follow the instructions below.

1. Stop Notepad++.
2. Create `qkNppReverseLines` folder in Notepad++'s plugins installation folder (`%Notepad++_program_folder%\plugins`).
3. Copy `qkNppReverseLines.dll` into the newly created folder.
The folder name and the dll name should be same.
Please select the version, x86(32bit) or x64(64bit).
4. Restart Notepad++.

## Development

This plugin has been developed using the Visual Studio 2019.
Available platforms are x86 win32 and x64 for Unicode Release and Debug.

## Reference:

[Scintilla API](http://www.scintilla.org/ScintillaDoc.html)

[Notepad++ Plugin Template](http://notepad-plus-plus.org/contribute/plugin-howto.html)

## Thanks

[NPP-Reverse-Text](https://github.com/mrt-prodz/NPP-Reverse-Text) is a very useful template to start developing this plugin.

[Notepad++ Plugin Template](https://notepad-plus-plus.org/contribute/plugin-howto.html) is a official template to start developing this plugin by the Visual Studio.
