 AREA ASDF, CODE, READONLY
	ENTRY
	MOV r2, #5
	ADD r10, r2, #0
	MOV r3, #6
	ADD r11, r3, #0
while1
	MOV r3, #0
	CMP r10,r3
	BLE end_while1
while2
	MOV r3, #0
	CMP r11,r3
	BLE end_while2
	MOV r3, #1
	SUB r0, r11, r3
	ADD r11, r0, #0
	B while2
end_while2
	MOV r2, #1
	SUB r0, r10, r2
	ADD r10, r0, #0
	B while1
end_while1
	END
