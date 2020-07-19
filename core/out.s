 AREA ASDF, CODE, READONLY
	ENTRY
	MOV r2, #10
	ADD r10, r2, #0
	MOV r3, #0
	ADD r11, r3, #0
while1
	MOV r3, #0
	CMP r10,r3
	BLE end_while1
	MUL r0, r10, r10
	ADD r12, r0, #0
	ADD r0, r11, r12
	ADD r11, r0, #0
	MOV r2, #1
	SUB r0, r10, r2
	ADD r10, r0, #0
	B while1
end_while1
	END
