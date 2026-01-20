# CASM Brainfuck kompilaator
### Autor: Johannes Tammerand

#### Kood jookseb kolmes programmis:

1. Täidab käske jooksvalt
2. Loob instruktsioonide pinu ja seejärel käib instruktsioonid läbi
3. Loob instruktsioonide pinu, kirjutab instruktsioonid üle assembly faili ning seejärel käivitab assembly

#### Käsud käivitamiseks:

* make clean - Puhastab kõik loodud failid
* make all - Kompileerib kõik koodi bfi ja bfiAsm failidesse
* make run - Kompileerib ning käivitab kõik koodi
* make bfi - Kompileerib bfi C interpretaatori, failinimi bfi
* make asm - Kompileerib ASM programmi antud BF programmile
* Kiiresti käivitamiseks saab kasutada "make clean all" ja "make clean run"

### Käivitamise parameetrid
* ./bfi --asm {programm} > kompileerib bfiAsm.asm assembly faili

### Kompileerimise parameetrid
* make
    * CFLAGS - programm lisas vaid -DDEBUG
        * ASM kompilaator eirab seda, kuna selle väljund satuks ASM faili
    * PROGRAM - täpsustab BF programmi, vaikimis "Hello World!" programm
        * nt. PROGRAM='+++.'