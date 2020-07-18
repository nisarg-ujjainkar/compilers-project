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
ifElse1
	MOV r3, #0
	CMP r11,r3
	BLE else1
	MOV r3, #1
	SUB r0, r11, r3
	ADD r11, r0, #0
	B else1
end_ifElse1
	MOV r2, #1
	SUB r0, r10, r2
	ADD r10, r0, #0
end_ifElse1
	B while1
end_while1
	END
