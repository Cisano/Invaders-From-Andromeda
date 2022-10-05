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

		MAC JOYSTICK_MACRO
		lda BooleanGameOver
		beq EndJoy0

CheckJoy0Sx
		lda #%01000000 ;left?     
		bit SWCHA
		bne CheckJoy0Dx
		lda CoordXSpaceShip
		cmp #LeftLimitSpaceShip
		beq CheckFire
		dec CoordXSpaceShip
		jmp CheckFire

CheckJoy0Dx
		lda #%10000000 ;right?
		bit SWCHA
		bne CheckFire
		lda CoordXSpaceShip
		cmp #RightLimitSpaceShip
		beq CheckFire
		inc CoordXSpaceShip

CheckFire
		lda BooleanNoBulletInGame
		bne EndJoy0
		;at the start game, can shot only when BooleanNoBulletInGame is 0
		lda #%10000000 ;Fire?
		bit INPT4
		bne EndJoy0

		lda BooleanFireSpaceShip
		bne EndJoy0
		;set true
		lda #1
		sta BooleanFireSpaceShip
		sta BooleanFirePressed

		;offset SpaceShip and ball/missile
		lda CoordXSpaceShip
		clc
		adc #1
		sta CoordXBallSpaceShip

EndJoy0
		ENDM
