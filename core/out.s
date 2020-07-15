	MOV r2, #5.000000
	ADD r10, r2, #0
	MOV r3, #6.000000
	ADD r11, r3, #0
while1
	MOV r3, #5.000000
	SUB r0, r11, r3
	CMP r10,r0
	BLT end_while1
	ADD r11, r10, #0
if1
	MOV r2, #6.000000
	CMP r11,r2
	BLT end_if1
	MOV r3, #1.000000
	SUB r0, r10, r3
	ADD r10, r0, #0
	B while1
end_while1
end_if1
