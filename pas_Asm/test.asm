.586
.model flat, stdcall

include <\masm32\include\msvcrt.inc>
include <\masm32\include\kernel32.inc>
includelib <\masm32\lib\msvcrt.lib>
includelib <\masm32\lib\kernel32.lib>

data segment

   division_1 dd 0
   division_2 dd 0
   print_format db "%d ", 0

data ends

text segment

value = -8
j = -12
i = -16

print PROC
   enter 0, 0
   mov eax, [ebp + 8]
   invoke crt_printf, offset print_format, eax
   leave
   ret 4
print ENDP

__start:

   enter 16, 0
   push 999
   pop eax
   push eax
   call print
   push 100
   push 54
   push 4
   push 2
   pop ebx
   pop eax
   mov division_1, eax
   mov division_2, ebx
   finit
   fild division_2
   fild division_1
   fdiv st(0), st(1)
   fist division_1
   push division_1
   pop eax
   pop ebx
   imul eax, ebx
   push eax
   pop eax
   pop ebx
   add eax, ebx
   push eax
   pop eax
   mov value[ebp], eax
   push value[ebp]
   pop eax
   push eax
   call print
   push 0
   pop eax
   mov i[ebp], eax
LS1:
   push i[ebp]
   pop ecx
   push 10
   pop edx
   cmp ecx, edx
   jge LN2
   push 1
   jmp LCE2
LN2:
   push 0
LCE2:
   pop eax
   cmp eax, 0
   je LE1
   push i[ebp]
   pop eax
   push eax
   call print
LS3:
LA3:
   push value[ebp]
   pop ecx
   push 100
   pop edx
   cmp ecx, edx
   je LN4
   push 1
   jmp LCE4
LN4:
   push 0
LCE4:
   pop eax
   cmp eax, 0
   je LE3
   push value[ebp]
   push 1
   pop ebx
   pop eax
   sub eax, ebx
   push eax
   pop eax
   mov value[ebp], eax
   push value[ebp]
   pop eax
   push eax
   call print
   push 100
   pop eax
   mov j[ebp], eax
   push j[ebp]
   pop ecx
   push 10
   pop edx
   cmp ecx, edx
   je LN6
   push 1
   jmp LCE6
LN6:
   push 0
LCE6:
   pop eax
   cmp eax, 0
   je LE5
LS5:
   push j[ebp]
   pop eax
   push eax
   call print
   push j[ebp]
   pop ecx
   push 10
   pop edx
   cmp ecx, edx
   jne LN8
   push 1
   jmp LCE8
LN8:
   push 0
LCE8:
   pop eax
   cmp eax, 0
   je LE7
LS7:
   push 1
   pop eax
   push eax
   call print
   push 0
   pop eax
   push eax
   call print
LE7:
LE5:
   jmp LS3
LE3:
LA1:
   push i[ebp]
   push 1
   pop eax
   pop ebx
   add eax, ebx
   push eax
   pop eax
   mov i[ebp], eax
   jmp LS1
LE1:

   leave

   ret

text ends

end __start

