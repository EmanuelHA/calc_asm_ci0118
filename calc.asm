section .text
    global _main
    global _process_data
    global _mem_reset

; Parámetros en EDI param#1, ESI param#2
_process_data:
    push rbx
    mov dword[val1], edi
    mov dword[val2], esi
    mov dword[opcode], edx
    call _op_calc
    pop rbx
    ret

_mem_reset:
    mov dword[val1], 0
    mov dword[val2], 0
    ret

_main:
    ; Solicitud de op
    mov ecx, msg_start
    mov edx, msg_start_len
    call _print
    call _read
    call _str_to_int
    mov [opcode], eax

    ; Solicitud de #1
    mov ecx, msg_a_in
    mov edx, msg_a_in_len
    call _print
    call _read
    call _str_to_int
    mov [val1], eax

    ; Solicitud de #2
    mov ecx, msg_b_in
    mov edx, msg_b_in_len
    call _print
    call _read
    call _str_to_int
    mov [val2], eax

    ; Resolver operacion
    call _op_calc
    mov [result], eax   ; Guarda el resultado

    ; Salida del resultado
    mov ecx, msg_res
    mov edx, msg_res_len
    call _print
    mov eax, [result]               ; N -> EAX
    call _int_to_str                ; RESULT -> ECX, STR_LEN -> ESI
    mov byte [ecx + esi], 0x0A      ; Concatena "\n" al final
    inc esi                         ; buffer legth++
    mov byte [ecx + esi], 0         ; NUL al final del buffer
    mov edx, esi                    ; STR_LEN calculado en _int_to_str
    call _print

    ; Llamada al sistema para salir con normalidad
    mov eax, 0x01   ; exit
    xor ebx, ebx    ; Codigo salida (0x0)
    int 0x80        ; Llamada al sistema


; LLamada al sistema para imprimir (REQ: ecx = msg, edx = length_msg)
_print:
    mov eax, 0x04   ; write
    mov ebx, 0x01   ; stdout
    int 0x80
    ret

; Llamada al sistema para entrada de datos (RET: buffer = STDIN_INPUT)
_read:
    mov eax, 0x03   ; read
    mov ebx, 0x00   ; stdin
    mov ecx, buffer ; buffer_dir
    mov edx, 0x10   ; ancho del buffer
    int 0x80
    ret

; Convierte los caracteres del buffer a numero entero (REQ: eax = 0, ecx = buffer | RET: eax = result)
_str_to_int:
    xor eax, eax            ; Limpia eax antes de comenzar la conversion
loop_s_t_i:
    movzx ebx, byte [ecx]   ; Carga el caracter actual (ECX) en EBX
    cmp bl, 0x0A            ; Compara el caracter con '\n' (0x0A)
    je  eol                 ; Salta al retorno en caso de '\n'
    movzx ebx, byte [ecx]   ; Carga el caracter actual (ECX) en EBX
    cmp bl, 0x0A            ; Compara el caracter con '\n' (0x0A)
    je  eol                 ; Salta al retorno en caso de '\n'
    sub ebx, '0'            ; Resta '0' (0x30) al caracter para obtener el valor numerico
    ; Concatenar resultado (eax = eax * 10 + ebx)
    imul eax, eax, 0x0A     ; Multiplicar EAX por 10
    add eax, ebx            ; Suma el ultimo digito extraido de la cadena 
    inc ecx                 ; Avanza al siguiente caracter
    jmp loop_s_t_i

eol:
    ret

;; Convierte un entero a una cadena de caracteres (REQ: eax = n, ecx = buffer | RET: esi = STR_LEN) ---> (invierte el orden XD)
;_int_to_str:
;    mov edi, 0x0A           ; Divisor x10 para extraer digitos del numero
;    xor esi, esi            ; index = 0
;concat_loop:
;    xor edx, edx            ; Limpiar el residuo
;    div edi                 ; EAX = EAX/EDI (n/10), EDX = residuo (digito extraido)
;    add dl, '0'             ; Agrega '0' (0x30) al digito para convertirlo en caracter
;    mov [ecx + esi], dl     ; Guardar el caracter en el buffer
;    inc esi                 ; index++
;    test eax, eax           ; Verificar si el cociente = 0
;    jnz concat_loop         ; Si no, sigue extrayendo digitos
;    mov byte [ecx + esi], 0 ; Encola NUL al buffer
;    ret

; Convierte un entero a una cadena de caracteres (REQ: eax = n, ecx = buffer | RET: esi = STR_LEN) --> *Clap* NICE!
_int_to_str:
    mov edi, 0x0A           ; Divisor x10 para extraer digitos del numero
    lea ebx, [ecx + 15]     ; Apunta ESI al final del buffer (16 bytes)
    mov byte [ebx], 0       ; Termina la cadena con NUL
    dec ebx                 ; Ajusta EBX para empezar a transcribir los digitos
    xor esi, esi            ; indice = 0

loop_i_t_s:
    xor edx, edx            ; Limpiar EDX antes de la division
    div edi                 ; Divide EAX entre 10, EAX = cociente, EDX = residuo
    add dl, '0'             ; Agrega '0' (0x30) al digito para convertirlo en caracter
    mov [ebx], dl           ; Guardar el caracter en el buffer
    dec ebx                 ; base--
    inc esi                 ; indice++ (STR_LEN)
    test eax, eax           ; Verifica si el cociente es 0
    jnz loop_i_t_s          ; De lo contrario continua el loop
    lea ecx, [ebx + 1]      ; Ajusta ECX para que apunte al inicio de la cadena
    ret

; Verifica el codigo de operacion y hace el salto acorde
_op_calc:
    mov eax, [opcode]
    cmp eax, 0x01
    je _add
    cmp eax, 0x02
    je _sub
    cmp eax, 0x03
    je _mul
    cmp eax, 0x04
    je _div
    ret

; Suma los numeros (RET: eax = result)
_add:
    mov eax, [val1]
    mov ebx, [val2]
    add eax, ebx
    ret

; Resta los numeros (RET: eax = result)
_sub:
    mov eax, [val1]
    mov ebx, [val2]
    sub eax, ebx
    ret

; Multiplica los numeros (RET: eax = result)
_mul:
    mov eax, [val1]
    mov ebx, [val2]
    mul ebx
    ret

; Divide los numeros (RET: eax = result)
_div:
    mov eax, [val1]
    mov ebx, [val2]
    xor edx, edx
    div ebx
    ret

section .data
    msg_start db 'Ingrese el numero con la operacion que desea realizar', 0x0A, \
                '1. SUMAR',         0x0A,  \
                '2. RESTAR',        0x0A,  \
                '3. MULTIPLICAR',   0x0A,  \
                '4. DIVIDIR',       0x0A
    msg_start_len equ $ - msg_start
    msg_a_in db 'Ingrese el primer numero', 0x0A
    msg_a_in_len equ $ - msg_a_in
    msg_b_in db 'Ingrese el segundo numero', 0x0A
    msg_b_in_len equ $ - msg_b_in
    msg_res db 'El resultado de la operacion es: '
    msg_res_len equ $ - msg_res

section .bss
    buffer  resb 0x10   ; Reserva 16 bytes para la entrada de datos
    opcode  resb 0x04   ; Reserva 4 bytes para guardar el # de operacion
    val1    resb 0x04   ; Reserva 4 bytes para guardar el primer operando
    val2    resb 0x04   ; Reserva 4 bytes para guardar el segundo operando
    result  resb 0x04   ; Reserva 4 bytes para guardar el resultado