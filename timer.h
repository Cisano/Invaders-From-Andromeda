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
    MAC TIMER_INVADERS_BOMBING
;MiSSILE1
    lda InvadersLine4 ;if empty..
    beq .EndTimer ;..avoid the problem when the invader wants strike and the column is shifting down
    lda BooleanNoBulletInGame
    beq .manageMissile1  
    jmp .Exit

.manageMissile1    
    lda TimerWhenInvaderStrike
    and #%01111111
    sta TimerWhenInvaderStrike
    bne .EndTimer

.setTimer
    ldx TimerForAll
    lda final_0,x ;it used only for start from somewhere
    and #%00000011
    tax
    lda TableTimerBombing,x
    sta TimerWhenInvaderStrike ;set Timing for Bombing
    lda #68
    sta CoordYMissile1

    ldy #0
    lda #1
    sta Tmp ;temporary cursor
    jmp .checkIt ;avoid to modify the variables at the first loop
.checkLoop  
    iny
    cpy #5
    beq .out
    asl Tmp
.checkIt
    lda Tmp
    and InvadersLine4 ;check if alien is in
    beq .checkLoop

    lda TableCoordXMissile,y
    adc CoordXInvaders
    sbc CoordXSpaceShip
    sbc #8 ;delta
    bpl .checkLoop

.out
    lda TableCoordXMissile,y
    adc CoordXInvaders
    sta CoordXMissile1

.EndTimer
    dec TimerWhenInvaderStrike
    lda TimerForAll
    and #%00000001
    beq .stepOneM1
    jmp .stepTwoM1
.stepOneM1
    inc CoordYMissile1
.stepTwoM1
    inc CoordYMissile1

    lda CoordYMissile1
    cmp #140
    bmi .manageMissile2
    lda #140
    sta CoordYMissile1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;MISSILE2 - semi random bombing
.manageMissile2
    lda TimerForAll
    and #%00000001
    beq .stepOneM2
    jmp .stepTwoM2
.stepOneM2
    inc CoordYMissile2
.stepTwoM2
    inc CoordYMissile2

    lda CoordYMissile2
    cmp #140
    bmi .Exit
    lda #68
    sta CoordYMissile2

    lda InvadersLine4 ;if empty..
    beq .determinatingTheXPosition ;..avoid the problem when the invader wants strike and the column is shifting down
                                   ;the missile 2 use the old X
    ldx TimerForAll
    lda logo_0,x ;it used only for start from somewhere
    and #%00000111
    tax

.selectWhoBombs
    asl CursorWhoBombs
    lda CursorWhoBombs
    and #%01000000 ;complete the loop #%01000000=#%00000001
    beq .continue
    lda #%00000001
    sta CursorWhoBombs
.continue
    dex
    bpl .selectWhoBombs

.checkIfInvaderThereIs
    lda CursorWhoBombs
    and InvadersLine4
    beq .selectWhoBombs ;if not found, search it!

.determinatingTheXPosition
    ldy #6
    lda #%01000000
    sta Tmp ;temporary cursor
.loop
    lsr Tmp
    dey
    lda Tmp
    and CursorWhoBombs
    beq .loop

    lda TableCoordXMissile,y
    adc CoordXInvaders
    sta CoordXMissile2

.Exit
    ENDM
