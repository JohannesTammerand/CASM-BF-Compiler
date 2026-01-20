global collaz
section .text

collaz:
    ;;;;; Funktsiooni proloog.
    push ebp
    mov ebp, esp

    ;;;;; Funktsiooni keha

    ; Kopeerime funktsiooni parameerti (4-baidise) väärtuse
    ; registrisse eax

    ; Liidame ebp'le 8, sest proloogis lisasime vana ebp väärtuse
    ; magasini (4-baiti), seega esimene 4-baidine parameeter asub
    ; 8 baidi kaugusel.
    mov eax, [ebp + 8]

    ; Lisame jagaja ecx registrisse
    mov ecx, 2

    ; Jagame edx:eax väärtuse 2'ga, jääk pannakse edx registrisse.
    mov edx, 0
    div ecx

    ; Kontrollime, kas jääk on 0
    cmp edx, 0

    ; Kui jääk on 0, siis eax juba sisaldab õiget väärtust 
    ; ja hüppame funktsiooni lõppu
    je lopp

    ; Hetkel teame, et parameeter on paaritu arv.
    ; Loeme parameetri väärtuse uuesti
    mov eax, [ebp + 8]

    ; Korrutame edx:eax väärtust kolmega
    mov ecx, 3
    mul ecx

    ; Ignoreerime teadlikult kõrgemaid bitte korrutisest, mis asuvad edx registris
    ; ja suurendame eax väärtust ühe võrra.
    add eax, 1


lopp:
    ;;;;; Funktsiooni epiloog

    ; Taastame esp väärtuse
    mov esp, ebp

    ; Taastame ebp väärtuse magasinist
    pop ebp

    ; Väljume funktsioonist
    ret