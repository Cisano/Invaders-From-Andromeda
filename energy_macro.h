;Copyright 2022 Cisano Carmelo
;
;This file is part of Invaders From Andromeda
;
;    Invaders From Andromeda is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    Invaders From Andromeda is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with Invaders From Andromeda.  If not, see <http://www.gnu.org/licenses/>.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO contatore energia
    MAC POSITIONING_ENERGY_COUNTER_MACRO
;Positioning energy counter
    lda #XPosGRP0Energia
       
    sta WSYNC
    sec
.DivideLoop
    sbc #15
    bcs .DivideLoop
    eor #7
    asl
    asl
    asl
    asl
    sta RESP0
    sta RESP1
    sta HMP0
    lda #$60
    sta HMP1
    ENDM

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MAC DEF_NUMBERS_POINTERS_MACRO
    sta WSYNC
    sta HMOVE

    lda #$01
    sta NUSIZ1      ;2 copies
    lda #0
    sta NUSIZ0      ;1 copy

    ldy ShieldNumber
    lda TablePtrNumbLow,y
    sta PointerNumberLow
    lda TablePtrNumbHi,y
    sta PointerNumberHi
    ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    MAC SHOW_ENERGY_MACRO
    lda #7
    sta Tmp
    ldy #0

.cycle
    ldx PointerGFXPlanet
    lda PlayfieldPlanet,x
    sta WSYNC

    sta PF0
    inx
    lda PlayfieldPlanet,x
    sta PF1
    inx
    txa
    and #%01111111
    sta PointerGFXPlanet

    lda Shield0,y
    sta GRP0
    lda Shield1,y
    sta GRP1

    lda (PointerNumberLow),y
    iny    
    nop
    nop
    sta GRP1
    dec Tmp
    bne .cycle ;if 0 not draw the energy counter
    lda #0
    sta GRP0
    sta GRP1

    ldx #176 ;scanline on exit
    ENDM
