; ModuleName    : a.bin
; entry Address :   000000
; Base of code  :   000000
; Base of data  :   000000
; ImageBase     : 00000000

                        code
00000000 eb04               jmp     short 0006

00000002 af                 scasd
00000003 c2bfa3             ret     a3bf

00000006 81ec00010000       sub     esp,00000100
0000000c 31c9               xor     ecx,ecx
0000000e 880c0c             mov     [esp+ecx],cl
00000011 fec1               inc     cl
00000013 75f9               jnz     000e
00000015 31c0               xor     eax,eax
00000017 baefbeadde         mov     edx,deadbeef
0000001c 02040c             add     al,[esp+ecx]
0000001f 00d0               add     al,dl
00000021 c1ca08             ror     edx,08
00000024 8a1c0c             mov     bl,[esp+ecx]
00000027 8a3c04             mov     bh,[esp+eax]
0000002a 881c04             mov     [esp+eax],bl
0000002d 883c0c             mov     [esp+ecx],bh
00000030 fec1               inc     cl
00000032 75e8               jnz     001c
00000034 e95c000000         jmp     00000095

00000039 89e3               mov     ebx,esp
0000003b 81c304000000       add     ebx,00000004
00000041 5c                 pop     esp
00000042 58                 pop     eax
00000043 3d41414141         cmp     eax,41414141
00000048 7543               jnz     008d
0000004a 58                 pop     eax
0000004b 3d42424242         cmp     eax,42424242
00000050 753b               jnz     008d
00000052 5a                 pop     edx
00000053 89d1               mov     ecx,edx
00000055 89e6               mov     esi,esp
00000057 89df               mov     edi,ebx
00000059 29cf               sub     edi,ecx
0000005b f3a4               rep     movsb
0000005d 89de               mov     esi,ebx
0000005f 89d1               mov     ecx,edx
00000061 89df               mov     edi,ebx
00000063 29cf               sub     edi,ecx
00000065 31c0               xor     eax,eax
00000067 31db               xor     ebx,ebx
00000069 31d2               xor     edx,edx
0000006b fec0               inc     al
0000006d 021c06             add     bl,[esi+eax]
00000070 8a1406             mov     dl,[esi+eax]
00000073 8a341e             mov     dh,[esi+ebx]
00000076 883406             mov     [esi+eax],dh
00000079 88141e             mov     [esi+ebx],dl
0000007c 00f2               add     dl,dh
0000007e 30f6               xor     dh,dh
00000080 8a1c16             mov     bl,[esi+edx]
00000083 8a17               mov     dl,[edi]
00000085 30da               xor     dl,bl
00000087 8817               mov     [edi],dl
00000089 47                 inc     edi
0000008a 49                 dec     ecx
0000008b 75de               jnz     006b
0000008d 31db               xor     ebx,ebx
0000008f 89d8               mov     eax,ebx
00000091 fec0               inc     al
00000093 cd80               int     80
00000095 90                 nop
00000096 90                 nop
00000097 e89dffffff         call    00000039
0000009c 41                 inc     ecx
0000009d 41                 inc     ecx
0000009e 41                 inc     ecx
0000009f 41                 inc     ecx
