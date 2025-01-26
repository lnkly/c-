	TITLE	temp.c
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
PUBLIC	_a1
PUBLIC	_b1
PUBLIC	_str
_DATA	SEGMENT
_a1	DD	0aH
_b1	DD	014H
_str	DB	'hello world', 00H
	ORG $+8
_DATA	ENDS
PUBLIC	_add
_TEXT	SEGMENT
_a$ = 8
_b$ = 12
_add	PROC NEAR
; File temp.c
; Line 4
	push	ebp
	mov	ebp, esp
; Line 5
	mov	eax, DWORD PTR _a$[ebp]
	add	eax, DWORD PTR _b$[ebp]
; Line 6
	pop	ebp
	ret	0
_add	ENDP
_TEXT	ENDS
PUBLIC	_sub
_TEXT	SEGMENT
_a$ = 8
_b$ = 12
_sub	PROC NEAR
; Line 8
	push	ebp
	mov	ebp, esp
; Line 9
	mov	eax, DWORD PTR _a$[ebp]
	sub	eax, DWORD PTR _b$[ebp]
; Line 10
	pop	ebp
	ret	0
_sub	ENDP
_TEXT	ENDS
PUBLIC	_main
_TEXT	SEGMENT
_num$ = -12
_d$ = -4
_i$ = -8
_main	PROC NEAR
; Line 11
	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
; Line 12
	push	2
	push	1
	call	_add
	add	esp, 8
	mov	DWORD PTR _num$[ebp], eax
; Line 13
	push	1
	push	3
	call	_sub
	add	esp, 8
	mov	DWORD PTR _d$[ebp], eax
; Line 15
	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L51
$L52:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L51:
	mov	ecx, DWORD PTR _i$[ebp]
	cmp	ecx, DWORD PTR _num$[ebp]
	jge	SHORT $L53
; Line 16
	mov	edx, DWORD PTR _d$[ebp]
	add	edx, 1
	mov	DWORD PTR _d$[ebp], edx
; Line 17
	jmp	SHORT $L52
$L53:
; Line 18
	xor	eax, eax
; Line 19
	mov	esp, ebp
	pop	ebp
	ret	0
_main	ENDP
_TEXT	ENDS
END
