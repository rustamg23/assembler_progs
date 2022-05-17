 use16
org 100h

first:
        cmp bx, cx
        je second
        cmp al,cl
        je third
        cmp cx, [bx]
        jne third
        cmp cl, [bx]
        je third
second:
        cmp cx, [si]
        je third
third:
        cmp ax, [di]
        je first
        cmp dx, [bx+si]
        je first
        cmp dx, [bx+di]
        jne forth
forth:
        sub bx, cx
        sub al,cl
        cmp bx, cx
        je fivth
fivth:
        sub cx, [bx]
        sub cl, [bx]
        sub cx, [si]
        cmp cx, [si]
        jne forth

