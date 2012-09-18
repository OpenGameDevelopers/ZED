; Provide all of the SIMD x64 functions

; void NormaliseMMX( Vector3 &p_Vec )
PUBLIC ?NormaliseMMX@Arithmetic@ZED@@YAXAEAVVector3@12@@Z
.CODE
ALIGN 8
?NormaliseMMX@Arithmetic@ZED@@YAXAEAVVector3@12@@Z PROC FRAME
MOV QWORD PTR [RSP+8], RCX
SUB	RSP,	32
.ALLOCSTACK 32
PUSH	RBX
.PUSHREG	RBX
.ENDPROLOG

POP	RBX
ADD	RSP,	32
RET
ALIGN 8
?NormaliseMMX@Arithmetic@ZED@@YAXAEAVVector3@12@@Z ENDP

_TEXT ENDS

END