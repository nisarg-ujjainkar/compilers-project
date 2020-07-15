	MOV r3, #5.000000
	MOV r1, #0x3e8
	STR r3, [r1]
	MOV r3, #6.000000
	MOV r1, #0x3ec
	STR r3, [r1]
while1
	MOV r1, #0x3e8
	LDR r2, [r1]
	MOV r1, #0x3ec
	LDR r2, [r1]
	MOV r3, #5.000000
	SUB r0, r2, r3
	CMP r2,r0
	BLT end_while1
	MOV r1, #0x3e8
	LDR r2, [r1]
	MOV r3, #1.000000
	SUB r0, r2, r3
	MOV r3, #0x3e8
	STR r0, [r3]
	MOV r1, #0x3e8
	LDR r2, [r1]
	MOV r3, #0x3ec
	STR r2, [r3]
	B while1
end_while1
