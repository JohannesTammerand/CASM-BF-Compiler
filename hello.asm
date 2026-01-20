global main
extern printf
section .text
main:

    ; teeme kordust 10 korda.
    mov ecx, 12

    tsykkel_algus:

        ; Salvestame exc väärtuse
        push ecx

        ; Lisame 32-bitise väärtuse magasini (4 baiti)
        push ecx

        push hello

        call printf

        ; Eemaldame magasinist 8 baiti (aadress ja väärtus 17)
        add esp, 8

        ; Taastame exc väärtuse
        pop ecx

        ; Vähenda ecx väärtust ja kui see pole 0, siis mine tagasi
        ; tsükli algusesse.
        loop tsykkel_algus

    mov eax, 0 

    ret

section .data
hello: db 'Loendur: %d', 10, 0