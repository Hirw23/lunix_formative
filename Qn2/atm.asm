; ==========================================
; ATM Simulation in NASM Assembly (Linux 32-bit)
; ==========================================
; Functions:
;   1. Deposit
;   2. Withdraw
;   3. Check Balance
;   4. Exit
; ==========================================

section .data
menu db 10, "==== ATM MENU ====", 10, \
     "1. Deposit", 10, \
     "2. Withdraw", 10, \
     "3. Check Balance", 10, \
     "4. Exit", 10, \
     "Choose an option: ", 0
menu_len equ $ - menu

depositMsg db "Enter amount to deposit: ", 0
depositMsg_len equ $ - depositMsg

withdrawMsg db "Enter amount to withdraw: ", 0
withdrawMsg_len equ $ - withdrawMsg

balanceMsg db "Your current balance is: ", 0
balanceMsg_len equ $ - balanceMsg

depositSuccess db "Deposit successful!", 10, 0
depositSuccess_len equ $ - depositSuccess

withdrawSuccess db "Withdrawal successful!", 10, 0
withdrawSuccess_len equ $ - withdrawSuccess

insufficientMsg db "Insufficient funds!", 10, 0
insufficientMsg_len equ $ - insufficientMsg

invalidMsg db "Invalid option, try again.", 10, 0
invalidMsg_len equ $ - invalidMsg

exitMsg db "Exiting ATM... Goodbye!", 10, 0
exitMsg_len equ $ - exitMsg

newline db 10, 0
newline_len equ $ - newline


section .bss
choice resb 2
amount resb 6
balance resd 1


section .text
global _start

_start:
    mov dword [balance], 1000          ; initial balance = 1000

main_menu:
    ; Display menu
    mov eax, 4
    mov ebx, 1
    mov ecx, menu
    mov edx, menu_len
    int 0x80

    ; Read user choice
    mov eax, 3
    mov ebx, 0
    mov ecx, choice
    mov edx, 2
    int 0x80

    mov al, [choice]
    cmp al, '1'
    je deposit
    cmp al, '2'
    je withdraw
    cmp al, '3'
    je check_balance
    cmp al, '4'
    je exit_program

    ; Invalid choice
    mov eax, 4
    mov ebx, 1
    mov ecx, invalidMsg
    mov edx, invalidMsg_len
    int 0x80
    jmp main_menu


; ---------------------------
; Deposit
; ---------------------------
deposit:
    mov eax, 4
    mov ebx, 1
    mov ecx, depositMsg
    mov edx, depositMsg_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, amount
    mov edx, 6
    int 0x80

    mov ecx, amount
    call atoi
    mov ebx, eax
    add [balance], ebx

    ; show deposit success message
    mov eax, 4
    mov ebx, 1
    mov ecx, depositSuccess
    mov edx, depositSuccess_len
    int 0x80

    jmp main_menu


; ---------------------------
; Withdraw
; ---------------------------
withdraw:
    mov eax, 4
    mov ebx, 1
    mov ecx, withdrawMsg
    mov edx, withdrawMsg_len
    int 0x80

    mov eax, 3
    mov ebx, 0
    mov ecx, amount
    mov edx, 6
    int 0x80

    mov ecx, amount
    call atoi
    mov ebx, eax
    mov eax, [balance]
    cmp eax, ebx
    jl insufficient_funds
    sub eax, ebx
    mov [balance], eax

    ; show withdrawal success message
    mov eax, 4
    mov ebx, 1
    mov ecx, withdrawSuccess
    mov edx, withdrawSuccess_len
    int 0x80

    jmp main_menu

insufficient_funds:
    mov eax, 4
    mov ebx, 1
    mov ecx, insufficientMsg
    mov edx, insufficientMsg_len
    int 0x80
    jmp main_menu


; ---------------------------
; Check Balance
; ---------------------------
check_balance:
    mov eax, 4
    mov ebx, 1
    mov ecx, balanceMsg
    mov edx, balanceMsg_len
    int 0x80

    mov eax, [balance]
    call print_num

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, newline_len
    int 0x80
    jmp main_menu


; ---------------------------
; Exit
; ---------------------------
exit_program:
    mov eax, 4
    mov ebx, 1
    mov ecx, exitMsg
    mov edx, exitMsg_len
    int 0x80

    mov eax, 1
    xor ebx, ebx
    int 0x80


; ---------------------------
; Convert ASCII string → integer (atoi)
; IN: ecx -> string
; OUT: eax = integer value
; ---------------------------
atoi:
    xor eax, eax
    xor ebx, ebx
.next:
    mov bl, [ecx]
    cmp bl, 10
    je .done
    cmp bl, 0
    je .done
    sub bl, '0'
    imul eax, eax, 10
    add eax, ebx
    inc ecx
    jmp .next
.done:
    ret


; ---------------------------
; Print integer in eax (print_num)
; ---------------------------
print_num:
    mov ebx, 10
    mov ecx, amount + 5
    mov byte [ecx], 0
.convert:
    dec ecx
    xor edx, edx
    div ebx
    add dl, '0'
    mov [ecx], dl
    test eax, eax
    jnz .convert

    mov eax, 4
    mov ebx, 1
    mov edx, amount + 6
    sub edx, ecx
    mov esi, ecx
    mov ecx, esi
    int 0x80
    ret
