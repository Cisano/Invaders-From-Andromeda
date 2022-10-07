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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO finds the hit invader
; The lenght of the bullet is important for the code created
    MAC FIND_HIT_INVADER_MACRO
    lda CollisionInvader ;resetted in subroutine ResetBallMissileGRP0
    and #%01000000 ;filter
    sta BooleanHitInvader

    beq .Exit ;no invader hit
;;;;;;;;;;;;
    dec TotalInvaders
    ;manage the last invader
    lda TotalInvaders
    cmp #1 ;because it is 30+1 for the sound
    bne .continue
    lda #0
    sta BooleanAtBeginOfEveryLevel
    inc LevelNumber
    lda LevelNumber
    sta BooleanVictorySongCh0
    sta BooleanVictorySongCh1
    ;;;;;;;;;;;;

.continue
    lda #%01000000
    sta Tmp

    lda CoordXBallSpaceShip
    sbc CoordXInvaders
    adc #8 ;offset
    sta TempX
    lda #0
.loopX
    lsr Tmp
    adc #16 ;distance in x between 2 invaders
    cmp TempX
    bmi .loopX

    ldy #$ff
    lda CursorYBallSpaceShip
    sbc CoordYInvaders
    sta TempX
    lda #0
.loopY
    iny
    adc #17 ;distance in Y between 2 invaders
    cmp TempX
    bmi .loopY

    lda InvadersLine0,y
    eor Tmp
    sta InvadersLine0,y

    jsr ResetBallMissileGRP0
    ;set timer for the invader lane goes down
    lda #$f5 ;240
    sta TimerShiftInvadersColumn
    sta BooleanShiftInvadersColumn

    lda Tmp
    sta CurosorMaskInvaders

.Exit
    ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO Draw GRP0 ;
    MAC DRAW_GRP0_SPACESHIP_MACRO
    lda (PointerSpaceShipLow),y
    sta WSYNC
    sta GRP0
    lda ColorSpaceShip,y
    sta COLUP0
    iny
    ENDM
; End MACRO Draw GRP0 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO Draw GRP1 ;
    MAC DRAW_GRP1_UFO_MACRO
    sta WSYNC
    sta ENABL
    lda (PointerGRP1UfoLow),y
    sta GRP1
    beq .Exit
    lda ColorUfo,y
    sta COLUP1
    beq .Exit    ;when zero GRP1 is complete
    iny
.Exit
    ENDM
; End MACRO Draw GRP1 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO Draw GRP0+GRP1 ;
    MAC DRAW_GRP0_GRP1_MACRO
    sta WSYNC
    sta ENABL
    lda (PointerGRP0Low),y
    sta GRP0
    lda (PointerGRP1Low),y
    sta GRP1
    lda (PntrColorGRPLow),y
    sta COLUP0
    sta COLUP1
    iny
.exit
    ENDM
; End MACRO Draw GRP0+GRP1 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Draw the ball
    MAC DRAWBALL_MACRO2
    lsr Bullet
    lda Bullet
    cpx CursorYBallSpaceShip
    bne .setBall
    lda #%00001110
    sta Bullet
.setBall
    sta ENABL
    ENDM
; End MACRO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Draw the ball without STA
    MAC DRAWBALL_WITHOUT_STA_MACRO2
    lsr Bullet
    lda Bullet
    cpx CursorYBallSpaceShip
    bne .setBall
    lda #%00001110
    sta Bullet
.setBall
    ENDM
; End MACRO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Draw the ball without STA
    MAC DRAWBALL_WITHOUT_STA_MACRO3
    lsr Bullet
    cpx CursorYBallSpaceShip
    bne .setBall
    lda #%00001110
    sta Bullet
.setBall
    ENDM
; End MACRO
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Position GRP0
    MAC POSITIONING_GRP0_MACRO2
    sec
    sta WSYNC ;start a new line
    ; one cycle of .DivideLoopGRP0 is saved
    sbc #30;sty ENABL
    ;;;;;;;;;;;;;
    ;and the saved cycle is used for these
    ldx Tmp
    stx ENAM1
    ldx Temp
    stx GRP1
    ;;;;;;;;;;;;;
.DivideLoopGRP0
    sbc #15		;subtract 15
    bcs .DivideLoopGRP0	;branch until negative
    eor #7		;calculate fine offset
    asl
    asl
    asl
    asl
    sta RESP0	;fix coarse position
    sta HMP0
    ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO
    MAC LOAD_POINTERS_INVADERS

    ;define pointer ghost invader
    ldy #10 ;index for ghost invader pointer
    lda TablePntrInvadersFrame0Low,y
    sta Tmp ;stored pointer ghost invader
    ;set animation frame for the invaders
    ldy #4 ;4,3,2,1,0 = 5 elements
timerAnimationInvaders
    sty TempX;*
    ldy LevelNumber; from 0 to 9 - 10 waves of invaders
    ldx TablePntrInvadersFrame0Low,y

    lda AlternateFrameInvader
    bne saveInvaderPointer
    ldx TablePntrInvadersFrame1Low,y

saveInvaderPointer
    ldy TempX;*

    stx Temp
    lda InvadersLine0,y
    and #%00111000
    bne thereAreSomeGRP0
    ldx Tmp ;pointer at ghost invader
thereAreSomeGRP0
    stx ArrayPointerGRP0Line0,y

    ldx Temp
    lda InvadersLine0,y
    and #%00000111
    bne thereAreSomeGRP1 ;if no more invaders
    ldx Tmp ;load pointer at ghost invader
thereAreSomeGRP1
    stx ArrayPointerGRP1Line0,y
    dey

    bpl timerAnimationInvaders
    ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MACRO sound generator
    MAC PLAY_SOUND_MACRO
    lda #Volume0
    sta AUDV0 ;volume march
    lda #Volume1
    sta AUDV1 ;volume sfx

    lda ShieldNumber ;when zero, game over
    bne .noEndGame
    ;end game, close the audio chanel
    lda #0
    sta AUDC0
    sta AUDF0

    jmp .chanel1

.noEndGame
;;;;;;;;;
;CHANEL 0
.codeForVictorySong
    lda LevelNumber
    cmp #LastLevelEndGame
    bne .codeForTheMarch

    lda TempoCH0 ;sound until the tempo not reach the zero
    beq .chanel1
    dec TempoCH0 ;end of any sound effect
    bne .chanel1
    jsr SoundEffectCH0
    lda BooleanVictorySongCh0
    beq .chanel1
    lda #0
    sta BooleanVictorySongCh0
    sta CursorSound0
    ldy #5 ;VictorySongCh0
    lda TableSoundsLow,y
    sta SoundMarchDataLow
    lda TableSoundsHi,y
    sta SoundMarchDataHi

    jsr SoundEffectCH0
    jmp .chanel1
;;;;;;;;;;
.codeForTheMarch
    ldy #0
    lda TableSoundsLow,y
    sta SoundMarchDataLow
    lda TableSoundsHi,y
    sta SoundMarchDataHi

    lda TempoCH0 ;sound until the tempo not reach the zero
    beq .march

    lda TimerMask
    bne .chanel1

    ;for loop invaders march
    lda CursorSound0
    cmp #24 ;lenght data sound for the march
    bne .march
.preMarch
    ;begin the loop again
    sty CursorSound0 ;Y=0 above
.march
    jsr SoundEffectCH0

;;;;;;;;;
;CHANEL 1
.chanel1
    ;ldx #1 ;chanel
    lda TempoCH1 ;sound until the tempo not reach the zero
    beq .shieldlost
    dec TempoCH1 ;end of any sound effect
    bne .end
.play
    jsr SoundEffectCH1
    ;jmp .end

;inizialize one of the sounds
.shieldlost
    lda BooleanShieldLost
    beq .fire
    ;lda #0
    ;sta BooleanShieldLost
    ldy #4 ;SpaceShipHit
    jmp .playIt
.fire
    lda BooleanFirePressed
    beq .hit
    lda #0
    sta BooleanFirePressed
    ldy #1 ;Fire
    jmp .playIt
.hit
    lda BooleanHitInvader
    beq .ufo
    lda #0
    sta BooleanHitInvader
    ldy #2 ;InvaderExplosion
    jmp .playIt
.ufo
    lda UfoCollision
    beq .victory
    ldy #3 ;UfoHit
    jmp .playIt
.victory
    lda LevelNumber
    cmp #LastLevelEndGame
    bne .end
    lda BooleanVictorySongCh1
    beq .end
    lda #0
    sta BooleanVictorySongCh1
    ldy #6 ;VictorySongCh1

.playIt
    lda TableSoundsLow,y
    sta SoundDataLow
    lda TableSoundsHi,y
    sta SoundDataHi
    lda #0
    sta CursorSound1
    jsr SoundEffectCH1
.end
    ENDM
