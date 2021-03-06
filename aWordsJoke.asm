sseg segment stack
    db 256 dup (?) ; определяем размер стека
sseg ends
;
data segment
    s db 'val aea kele $'
    t db ' $'
    len dw $ - s - 3 ; длина строки
    endl db 10,13,'$'
    res	db	80 dup (?) ; стек для результата
data ends
;
code segment
assume cs:code, ds:data, ss:sseg
start: mov ax,data ;установка указателя на сегмент данных
    mov ds,ax ;надо делать
;
    mov cx,len ; cx-регистр счетчик
    xor bx,bx ; bx=0 (bx="H")
for:
    cmp s[bx],'$' ; сравниваем s[bx]
    je e ; если s[bx] конец строки - завершаем программу
    jmp prin ; если проверка не срабатывает идем дальше по циклу и прыгаем в prin
prin:
    cmp s[bx], 'a'
    je true
    mov dl, [bx] 
    inc bx 
    mov ah, 2
    int 21h 
    jmp for 
true:
    mov dl, ' '
    inc bx;
    mov ah,2
    int 21h
    jmp for
e:
    mov ax,04C00h ;функция завершения программы
    int 21h
code ends
end start
;jnz
