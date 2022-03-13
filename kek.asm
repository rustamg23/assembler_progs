sseg segment stack
    db 256 dup (?) ; определяем размер стека
sseg ends
;
data segment
    s db 'a1234a wwvbla abbl bllla alfelfa ava ava wdw aaaava ava $'
    t db ' $'
    len dw $ - s - 3 ; длина строки
    endl db 10,13,'$'
    res	db	80 dup (?) ; стек для результата
data ends
;
code segment
assume cs:code, ds:data, ss:sseg
start: 
	mov ax,data ;установка указателя на сегмент данных
    mov ds,ax ;надо делать
    mov cx,len ; cx-регистр счетчик
    xor bx,bx ; bx=0 (bx="H")

strt:
	cmp bx, 0
		je newWord0

	cmp s[bx], ' '
    	je newWord ; если проверка не срабатывает идем дальше по циклу и прыгаем в newWord
	
    cmp s[bx],'$' ; сравниваем s[bx]
    	je e ; если s[bx] конец строки - завершаем программу
	jmp standard
	
newWord0:
    cmp s[bx], 'a'
    	je truePush
    mov dl, [bx]
    mov ah, 2
    int 21h 
	inc bx 
    jmp strt

newWord:
	mov dl, [bx]
    mov ah, 2
    int 21h 
	inc bx;
	cmp s[bx], 'a'
    	je truePush
    mov dl, [bx]
    mov ah, 2
    int 21h 
	inc bx 
    jmp strt

standard:
	mov dl, [bx]
	mov ah, 2
	int 21h
	inc bx
	jmp strt

truePush:
	push bx
	jmp true

true:
	inc bx
	cmp s[bx], ' '
    	je acmp
	jmp true

acmp:
	dec bx
	cmp s[bx], 'a'
		je apop
	jmp nepop

nepop:
	pop bx
	jmp strt

apop:
	inc bx
	jmp strt


e:
    mov ax,04C00h ;функция завершения программы
    int 21h
code ends
end start
