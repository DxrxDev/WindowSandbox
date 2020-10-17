; Listing generated by Microsoft (R) Optimizing Compiler Version 19.27.29111.0 

	TITLE	f:\windowsandbox\windowsandbox\src\window.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB OLDNAMES

PUBLIC	??_C@_07KHIJMDBA@SandBox@			; `string'
PUBLIC	??_C@_0O@BLDOFIJ@?$FLDescription?$FN@		; `string'
PUBLIC	??_C@_0N@CHALHMAE@?$FLError?5Code?$FN@		; `string'
PUBLIC	??_C@_0BB@DAPHOIPK@Window?5Exception@		; `string'
PUBLIC	??_C@_0BF@KPHDJHEI@Undefined?5Error?5Code@	; `string'
PUBLIC	??_R0?AVWindowException@Window@@@8		; Window::WindowException `RTTI Type Descriptor'
PUBLIC	??_R1A@?0A@EA@WindowException@Window@@8		; Window::WindowException::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_7WindowException@Window@@6B@			; Window::WindowException::`vftable'
PUBLIC	??_R4WindowException@Window@@6B@		; Window::WindowException::`RTTI Complete Object Locator'
PUBLIC	??_R2WindowException@Window@@8			; Window::WindowException::`RTTI Base Class Array'
PUBLIC	??_R3WindowException@Window@@8			; Window::WindowException::`RTTI Class Hierarchy Descriptor'
PUBLIC	?wndClassName@WindowClass@Window@@0QBDB		; Window::WindowClass::wndClassName
EXTRN	__imp__FormatMessageA@28:PROC
EXTRN	__imp__DestroyWindow@4:PROC
EXTRN	__imp__LocalFree@4:PROC
EXTRN	__imp__ShowWindow@8:PROC
EXTRN	__imp__SetWindowLongA@12:PROC
EXTRN	__imp__GetWindowLongA@8:PROC
EXTRN	_atexit:PROC
EXTRN	__imp__GetModuleHandleA@4:PROC
EXTRN	__imp__DefWindowProcA@16:PROC
EXTRN	__imp__CreateWindowExA@48:PROC
EXTRN	__imp__UnregisterClassA@8:PROC
EXTRN	__imp__PostQuitMessage@4:PROC
EXTRN	__imp__RegisterClassExA@4:PROC
;	COMDAT ?wndClassName@WindowClass@Window@@0QBDB
CONST	SEGMENT
?wndClassName@WindowClass@Window@@0QBDB DD FLAT:??_C@_07KHIJMDBA@SandBox@ ; Window::WindowClass::wndClassName
CONST	ENDS
;	COMDAT ??_R3WindowException@Window@@8
rdata$r	SEGMENT
??_R3WindowException@Window@@8 DD 00H			; Window::WindowException::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	03H
	DD	FLAT:??_R2WindowException@Window@@8
rdata$r	ENDS
;	COMDAT ??_R2WindowException@Window@@8
rdata$r	SEGMENT
??_R2WindowException@Window@@8 DD FLAT:??_R1A@?0A@EA@WindowException@Window@@8 ; Window::WindowException::`RTTI Base Class Array'
	DD	FLAT:??_R1A@?0A@EA@Exception@@8
	DD	FLAT:??_R1A@?0A@EA@exception@std@@8
rdata$r	ENDS
;	COMDAT ??_R4WindowException@Window@@6B@
rdata$r	SEGMENT
??_R4WindowException@Window@@6B@ DD 00H			; Window::WindowException::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	FLAT:??_R0?AVWindowException@Window@@@8
	DD	FLAT:??_R3WindowException@Window@@8
rdata$r	ENDS
;	COMDAT ??_7WindowException@Window@@6B@
CONST	SEGMENT
??_7WindowException@Window@@6B@ DD FLAT:??_R4WindowException@Window@@6B@ ; Window::WindowException::`vftable'
	DD	FLAT:??_EWindowException@Window@@UAEPAXI@Z
	DD	FLAT:?what@WindowException@Window@@UBEPBDXZ
	DD	FLAT:?GetType@WindowException@Window@@UBEPBDXZ
CONST	ENDS
;	COMDAT ??_R1A@?0A@EA@WindowException@Window@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@WindowException@Window@@8 DD FLAT:??_R0?AVWindowException@Window@@@8 ; Window::WindowException::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	02H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3WindowException@Window@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVWindowException@Window@@@8
data$r	SEGMENT
??_R0?AVWindowException@Window@@@8 DD FLAT:??_7type_info@@6B@ ; Window::WindowException `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVWindowException@Window@@', 00H
data$r	ENDS
;	COMDAT ??_C@_0BF@KPHDJHEI@Undefined?5Error?5Code@
CONST	SEGMENT
??_C@_0BF@KPHDJHEI@Undefined?5Error?5Code@ DB 'Undefined Error Code', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0BB@DAPHOIPK@Window?5Exception@
CONST	SEGMENT
??_C@_0BB@DAPHOIPK@Window?5Exception@ DB 'Window Exception', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0N@CHALHMAE@?$FLError?5Code?$FN@
CONST	SEGMENT
??_C@_0N@CHALHMAE@?$FLError?5Code?$FN@ DB '[Error Code]', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0O@BLDOFIJ@?$FLDescription?$FN@
CONST	SEGMENT
??_C@_0O@BLDOFIJ@?$FLDescription?$FN@ DB '[Description]', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_07KHIJMDBA@SandBox@
CONST	SEGMENT
??_C@_07KHIJMDBA@SandBox@ DB 'SandBox', 00H		; `string'
PUBLIC	??1WindowException@Window@@UAE@XZ		; Window::WindowException::~WindowException
PUBLIC	??_GWindowException@Window@@UAEPAXI@Z		; Window::WindowException::`scalar deleting destructor'
PUBLIC	?HandleMsg@Window@@AAEJPAUHWND__@@IIJ@Z		; Window::HandleMsg
PUBLIC	?HandleMsgThunk@Window@@CGJPAUHWND__@@IIJ@Z	; Window::HandleMsgThunk
PUBLIC	?HandleMsgSetup@Window@@CGJPAUHWND__@@IIJ@Z	; Window::HandleMsgSetup
PUBLIC	?GetInstance@WindowClass@Window@@SAPAUHINSTANCE__@@XZ ; Window::WindowClass::GetInstance
PUBLIC	?GetName@WindowClass@Window@@SAPBDXZ		; Window::WindowClass::GetName
PUBLIC	??1WindowClass@Window@@AAE@XZ			; Window::WindowClass::~WindowClass
PUBLIC	??0WindowClass@Window@@AAE@XZ			; Window::WindowClass::WindowClass
PUBLIC	?__autoclassinit2@WindowClass@Window@@QAEXI@Z	; Window::WindowClass::__autoclassinit2
PUBLIC	??1Window@@QAE@XZ				; Window::~Window
PUBLIC	??0Window@@QAE@HHPBD@Z				; Window::Window
PUBLIC	?wndClass@WindowClass@Window@@0V12@A		; Window::WindowClass::wndClass
EXTRN	??_EWindowException@Window@@UAEPAXI@Z:PROC	; Window::WindowException::`vector deleting destructor'
?wndClass@WindowClass@Window@@0V12@A DD 01H DUP (?)	; Window::WindowClass::wndClass
_BSS	ENDS
??wndClass$initializer$@WindowClass@Window@@0P6AXXZA@@3P6AXXZA DD FLAT:??__E?wndClass@WindowClass@Window@@0V12@A@@YAXXZ ; ??wndClass$initializer$@WindowClass@Window@@0P6AXXZA@@3P6AXXZA
; Function compile flags: /Ogtp
;	COMDAT ??__E?wndClass@WindowClass@Window@@0V12@A@@YAXXZ
text$di	SEGMENT
_wc$1 = -52						; size = 48
__$ArrayPad$ = -4					; size = 4
??__E?wndClass@WindowClass@Window@@0V12@A@@YAXXZ PROC	; `dynamic initializer for 'Window::WindowClass::wndClass'', COMDAT
; File F:\WindowSandbox\WindowSandbox\src\Window.cpp
; Line 5
	push	ebp
	mov	ebp, esp
	and	esp, -8					; fffffff8H
	sub	esp, 56					; 00000038H
	mov	eax, DWORD PTR ___security_cookie
	xor	eax, esp
	mov	DWORD PTR __$ArrayPad$[esp+56], eax
; Line 7
	push	0
	call	DWORD PTR __imp__GetModuleHandleA@4
	mov	DWORD PTR ?wndClass@WindowClass@Window@@0V12@A, eax ; Window::WindowClass::wndClass
	xorps	xmm0, xmm0
; Line 33
	mov	DWORD PTR _wc$1[esp+76], eax
; Line 21
	lea	eax, DWORD PTR _wc$1[esp+56]
	push	eax
	mov	DWORD PTR _wc$1[esp+60], 48		; 00000030H
	mov	DWORD PTR _wc$1[esp+64], 32		; 00000020H
	mov	DWORD PTR _wc$1[esp+68], OFFSET ?HandleMsgSetup@Window@@CGJPAUHWND__@@IIJ@Z ; Window::HandleMsgSetup
	mov	DWORD PTR _wc$1[esp+72], 0
	mov	DWORD PTR _wc$1[esp+76], 0
	movups	XMMWORD PTR _wc$1[esp+84], xmm0
	mov	DWORD PTR _wc$1[esp+100], OFFSET ??_C@_07KHIJMDBA@SandBox@
	mov	DWORD PTR _wc$1[esp+104], 0
	call	DWORD PTR __imp__RegisterClassExA@4
; Line 5
	push	OFFSET ??__F?wndClass@WindowClass@Window@@0V12@A@@YAXXZ ; `dynamic atexit destructor for 'Window::WindowClass::wndClass''
	call	_atexit
	mov	ecx, DWORD PTR __$ArrayPad$[esp+60]
	add	esp, 4
	xor	ecx, esp
	call	@__security_check_cookie@4
	mov	esp, ebp
	pop	ebp
	ret	0
??__E?wndClass@WindowClass@Window@@0V12@A@@YAXXZ ENDP	; `dynamic initializer for 'Window::WindowClass::wndClass''
text$di	ENDS
; Function compile flags: /Ogtp
;	COMDAT ??__F?wndClass@WindowClass@Window@@0V12@A@@YAXXZ
text$yd	SEGMENT
??__F?wndClass@WindowClass@Window@@0V12@A@@YAXXZ PROC	; `dynamic atexit destructor for 'Window::WindowClass::wndClass'', COMDAT
; File F:\WindowSandbox\WindowSandbox\src\Window.cpp
; Line 25
	push	DWORD PTR ?wndClass@WindowClass@Window@@0V12@A ; Window::WindowClass::wndClass
	push	OFFSET ??_C@_07KHIJMDBA@SandBox@
	call	DWORD PTR __imp__UnregisterClassA@8
	ret	0
??__F?wndClass@WindowClass@Window@@0V12@A@@YAXXZ ENDP	; `dynamic atexit destructor for 'Window::WindowClass::wndClass''
text$yd	ENDS
; Function compile flags: /Ogtp
;	COMDAT ??_GWindowException@Window@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8						; size = 4
??_GWindowException@Window@@UAEPAXI@Z PROC		; Window::WindowException::`scalar deleting destructor', COMDAT
; _this$ = ecx
	push	ebp
	mov	ebp, esp
	push	esi
	mov	esi, ecx
	call	??1Exception@@UAE@XZ
	test	BYTE PTR ___flags$[ebp], 1
	je	SHORT $LN7@scalar
	push	68					; 00000044H
	push	esi
	call	??3@YAXPAXI@Z				; operator delete
	add	esp, 8
$LN7@scalar:
	mov	eax, esi
	pop	esi
	pop	ebp
	ret	4
??_GWindowException@Window@@UAEPAXI@Z ENDP		; Window::WindowException::`scalar deleting destructor'
_TEXT	ENDS
; Function compile flags: /Ogtp
;	COMDAT ??1WindowException@Window@@UAE@XZ
_TEXT	SEGMENT
??1WindowException@Window@@UAE@XZ PROC			; Window::WindowException::~WindowException, COMDAT
; _this$ = ecx
	jmp	??1Exception@@UAE@XZ
??1WindowException@Window@@UAE@XZ ENDP			; Window::WindowException::~WindowException
_TEXT	ENDS
END
