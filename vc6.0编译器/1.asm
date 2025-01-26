	TITLE	1.c
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_add
_TEXT	SEGMENT
_a$ = 8
_b$ = 12
_add	PROC NEAR
; File 1.c
; Line 3
	push	ebp
	mov	ebp, esp
; Line 4
	mov	eax, DWORD PTR _a$[ebp]
	add	eax, DWORD PTR _b$[ebp]
; Line 5
	pop	ebp
	ret	0
_add	ENDP
_TEXT	ENDS
PUBLIC	_main
EXTRN	_printf:NEAR
_DATA	SEGMENT
$SG344	DB	'hello world %d', 00H
_DATA	ENDS
_TEXT	SEGMENT
_c$ = -4
_main	PROC NEAR
; Line 8
	push	ebp
	mov	ebp, esp
	push	ecx
	push	ebx
	push	esi
	push	edi
; Line 9
	push	2
	push	1
	call	_add
	add	esp, 8
	mov	DWORD PTR _c$[ebp], eax
; Line 11
	push	eax
; Line 12
	mov	eax, DWORD PTR _c$[ebp]
; Line 13
	inc	eax
; Line 14
	mov	DWORD PTR _c$[ebp], eax
; Line 15
	pop	eax
; Line 17
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	push	OFFSET FLAT:$SG344
	call	_printf
	add	esp, 8
; Line 18
	xor	eax, eax
; Line 19
	pop	edi
	pop	esi
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	0
_main	ENDP
_TEXT	ENDS
END
