/* Raw - Another World Interpreter
 * Copyright (C) 2004 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "vm.h"
#include "resource.h"
#include "video.h"


const VirtualMachine::OpcodeStub VirtualMachine::opcodeTable[] = {
	/* 0x00 */
	&VirtualMachine::op_movConst,
	&VirtualMachine::op_mov,
	&VirtualMachine::op_add,
	&VirtualMachine::op_addConst,
	/* 0x04 */
	&VirtualMachine::op_call,
	&VirtualMachine::op_ret,
	&VirtualMachine::op_break,
	&VirtualMachine::op_jmp,
	/* 0x08 */
	&VirtualMachine::op_setSetVect,
	&VirtualMachine::op_jnz,
	&VirtualMachine::op_condJmp,
	&VirtualMachine::op_setPalette,
	/* 0x0C */
	&VirtualMachine::op_resetThread,
	&VirtualMachine::op_selectPage,
	&VirtualMachine::op_fillPage,
	&VirtualMachine::op_copyPage,
	/* 0x10 */
	&VirtualMachine::op_blitFramebuffer,
	&VirtualMachine::op_halt,
	&VirtualMachine::op_drawString,
	&VirtualMachine::op_sub,
	/* 0x14 */
	&VirtualMachine::op_and,
	&VirtualMachine::op_or,
	&VirtualMachine::op_shl,
	&VirtualMachine::op_shr,
	/* 0x18 */
	&VirtualMachine::op_playSound,
	&VirtualMachine::op_updateMemList,
	&VirtualMachine::op_playMusic
};

const uint16 VirtualMachine::frequenceTable[] = {
	0x0CFF, 0x0DC3, 0x0E91, 0x0F6F, 0x1056, 0x114E, 0x1259, 0x136C, 
	0x149F, 0x15D9, 0x1726, 0x1888, 0x19FD, 0x1B86, 0x1D21, 0x1EDE, 
	0x20AB, 0x229C, 0x24B3, 0x26D7, 0x293F, 0x2BB2, 0x2E4C, 0x3110, 
	0x33FB, 0x370D, 0x3A43, 0x3DDF, 0x4157, 0x4538, 0x4998, 0x4DAE, 
	0x5240, 0x5764, 0x5C9A, 0x61C8, 0x6793, 0x6E19, 0x7485, 0x7BBD
};

const uint16 Resource::_memListParts[][4] = {
	{ 0x14, 0x15, 0x16, 0x00 }, // protection screens
	{ 0x17, 0x18, 0x19, 0x00 }, // introduction
	{ 0x1A, 0x1B, 0x1C, 0x11 },
	{ 0x1D, 0x1E, 0x1F, 0x11 },
	{ 0x20, 0x21, 0x22, 0x11 },
	{ 0x23, 0x24, 0x25, 0x00 },
	{ 0x26, 0x27, 0x28, 0x11 },
	{ 0x29, 0x2A, 0x2B, 0x11 },
	{ 0x7D, 0x7E, 0x7F, 0x00 },
	{ 0x7D, 0x7E, 0x7F, 0x00 }  // password screen
};

const uint8 Video::_font[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00,
	0x28, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x00,
	0x08, 0x3E, 0x48, 0x3C, 0x12, 0x7C, 0x10, 0x00, 0x42, 0xA4, 0x48, 0x10, 0x24, 0x4A, 0x84, 0x00,
	0x60, 0x90, 0x90, 0x70, 0x8A, 0x84, 0x7A, 0x00, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x06, 0x08, 0x10, 0x10, 0x10, 0x08, 0x06, 0x00, 0xC0, 0x20, 0x10, 0x10, 0x10, 0x20, 0xC0, 0x00,
	0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x28, 0x10, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
	0x78, 0x84, 0x8C, 0x94, 0xA4, 0xC4, 0x78, 0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7C, 0x00,
	0x78, 0x84, 0x04, 0x08, 0x30, 0x40, 0xFC, 0x00, 0x78, 0x84, 0x04, 0x38, 0x04, 0x84, 0x78, 0x00,
	0x08, 0x18, 0x28, 0x48, 0xFC, 0x08, 0x08, 0x00, 0xFC, 0x80, 0xF8, 0x04, 0x04, 0x84, 0x78, 0x00,
	0x38, 0x40, 0x80, 0xF8, 0x84, 0x84, 0x78, 0x00, 0xFC, 0x04, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00,
	0x78, 0x84, 0x84, 0x78, 0x84, 0x84, 0x78, 0x00, 0x78, 0x84, 0x84, 0x7C, 0x04, 0x08, 0x70, 0x00,
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x10, 0x60,
	0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0xFE, 0x00, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x00, 0x7C, 0x82, 0x02, 0x0C, 0x10, 0x00, 0x10, 0x00,
	0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x78, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x00,
	0xF8, 0x84, 0x84, 0xF8, 0x84, 0x84, 0xF8, 0x00, 0x78, 0x84, 0x80, 0x80, 0x80, 0x84, 0x78, 0x00,
	0xF8, 0x84, 0x84, 0x84, 0x84, 0x84, 0xF8, 0x00, 0x7C, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7C, 0x00,
	0xFC, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x00, 0x7C, 0x80, 0x80, 0x8C, 0x84, 0x84, 0x7C, 0x00,
	0x84, 0x84, 0x84, 0xFC, 0x84, 0x84, 0x84, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00,
	0x04, 0x04, 0x04, 0x04, 0x84, 0x84, 0x78, 0x00, 0x8C, 0x90, 0xA0, 0xE0, 0x90, 0x88, 0x84, 0x00,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x00, 0x82, 0xC6, 0xAA, 0x92, 0x82, 0x82, 0x82, 0x00,
	0x84, 0xC4, 0xA4, 0x94, 0x8C, 0x84, 0x84, 0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00,
	0xF8, 0x84, 0x84, 0xF8, 0x80, 0x80, 0x80, 0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x8C, 0x7C, 0x03,
	0xF8, 0x84, 0x84, 0xF8, 0x90, 0x88, 0x84, 0x00, 0x78, 0x84, 0x80, 0x78, 0x04, 0x84, 0x78, 0x00,
	0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00,
	0x84, 0x84, 0x84, 0x84, 0x84, 0x48, 0x30, 0x00, 0x82, 0x82, 0x82, 0x82, 0x92, 0xAA, 0xC6, 0x00,
	0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, 0x00, 0x82, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x00,
	0xFC, 0x04, 0x08, 0x10, 0x20, 0x40, 0xFC, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00,
	0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00,
	0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE,
	0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00, 0x38, 0x04, 0x3C, 0x44, 0x3C, 0x00,
	0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0x78, 0x00, 0x00, 0x00, 0x3C, 0x40, 0x40, 0x40, 0x3C, 0x00,
	0x04, 0x04, 0x3C, 0x44, 0x44, 0x44, 0x3C, 0x00, 0x00, 0x00, 0x38, 0x44, 0x7C, 0x40, 0x3C, 0x00,
	0x38, 0x44, 0x40, 0x60, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x78,
	0x40, 0x40, 0x58, 0x64, 0x44, 0x44, 0x44, 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00,
	0x02, 0x00, 0x02, 0x02, 0x02, 0x02, 0x42, 0x3C, 0x40, 0x40, 0x46, 0x48, 0x70, 0x48, 0x46, 0x00,
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0xEC, 0x92, 0x92, 0x92, 0x92, 0x00,
	0x00, 0x00, 0x78, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00,
	0x00, 0x00, 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x00, 0x00, 0x3C, 0x44, 0x44, 0x3C, 0x04, 0x04,
	0x00, 0x00, 0x4C, 0x70, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00, 0x3C, 0x40, 0x38, 0x04, 0x78, 0x00,
	0x10, 0x10, 0x3C, 0x10, 0x10, 0x10, 0x0C, 0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x44, 0x78, 0x00,
	0x00, 0x00, 0x44, 0x44, 0x44, 0x28, 0x10, 0x00, 0x00, 0x00, 0x82, 0x82, 0x92, 0xAA, 0xC6, 0x00,
	0x00, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x42, 0x22, 0x24, 0x18, 0x08, 0x30,
	0x00, 0x00, 0x7C, 0x08, 0x10, 0x20, 0x7C, 0x00, 0x60, 0x90, 0x20, 0x40, 0xF0, 0x00, 0x00, 0x00,
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x38, 0x44, 0xBA, 0xA2, 0xBA, 0x44, 0x38, 0x00,
	0x38, 0x44, 0x82, 0x82, 0x44, 0x28, 0xEE, 0x00, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA
};


const StrEntry Video::_stringsTableEng[] = {
	{ 0x001, "P E A N U T  3000" },
	{ 0x002, "Copyright  } 1990 Peanut Computer, Inc.\nAll rights reserved.\n\nCDOS Version 5.01" },
	{ 0x003, "2" },
	{ 0x004, "3" },
	{ 0x005, "." },
	{ 0x006, "A" },
	{ 0x007, "@" },
	{ 0x008, "PEANUT 3000" },
	{ 0x00A, "R" },
	{ 0x00B, "U" },
	{ 0x00C, "N" },
	{ 0x00D, "P" },
	{ 0x00E, "R" },
	{ 0x00F, "O" },
	{ 0x010, "J" },
	{ 0x011, "E" },
	{ 0x012, "C" },
	{ 0x013, "T" },
	{ 0x014, "Shield 9A.5f Ok" },
	{ 0x015, "Flux % 5.0177 Ok" },
	{ 0x016, "CDI Vector ok" },
	{ 0x017, " %%%ddd ok" },
	{ 0x018, "Race-Track ok" },
	{ 0x019, "SYNCHROTRON" },
	{ 0x01A, "E: 23%\ng: .005\n\nRK: 77.2L\n\nopt: g+\n\n Shield:\n1: OFF\n2: ON\n3: ON\n\nP~: 1\n" },
	{ 0x01B, "ON" },
	{ 0x01C, "-" },
	{ 0x021, "|" },
	{ 0x022, "--- Theoretical study ---" },
	{ 0x023, " THE EXPERIMENT WILL BEGIN IN    SECONDS" },
	{ 0x024, "  20" },
	{ 0x025, "  19" },
	{ 0x026, "  18" },
	{ 0x027, "  4" },
	{ 0x028, "  3" },
	{ 0x029, "  2" },
	{ 0x02A, "  1" },
	{ 0x02B, "  0" },
	{ 0x02C, "L E T ' S   G O" },
	{ 0x031, "- Phase 0:\nINJECTION of particles\ninto synchrotron" },
	{ 0x032, "- Phase 1:\nParticle ACCELERATION." },
	{ 0x033, "- Phase 2:\nEJECTION of particles\non the shield." },
	{ 0x034, "A  N  A  L  Y  S  I  S" },
	{ 0x035, "- RESULT:\nProbability of creating:\n ANTIMATTER: 91.V %\n NEUTRINO 27:  0.04 %\n NEUTRINO 424: 18 %\n" },
	{ 0x036, "   Practical verification Y/N ?" },
	{ 0x037, "SURE ?" },
	{ 0x038, "MODIFICATION OF PARAMETERS\nRELATING TO PARTICLE\nACCELERATOR (SYNCHROTRON)." },
	{ 0x039, "       RUN EXPERIMENT ?" },
	{ 0x03C, "t---t" },
	{ 0x03D, "000 ~" },
	{ 0x03E, ".20x14dd" },
	{ 0x03F, "gj5r5r" },
	{ 0x040, "tilgor 25%" },
	{ 0x041, "12% 33% checked" },
	{ 0x042, "D=4.2158005584" },
	{ 0x043, "d=10.00001" },
	{ 0x044, "+" },
	{ 0x045, "*" },
	{ 0x046, "% 304" },
	{ 0x047, "gurgle 21" },
	{ 0x048, "{{{{" },
	{ 0x049, "Delphine Software" },
	{ 0x04A, "By Eric Chahi" },
	{ 0x04B, "  5" },
	{ 0x04C, "  17" },
	{ 0x12C, "0" },
	{ 0x12D, "1" },
	{ 0x12E, "2" },
	{ 0x12F, "3" },
	{ 0x130, "4" },
	{ 0x131, "5" },
	{ 0x132, "6" },
	{ 0x133, "7" },
	{ 0x134, "8" },
	{ 0x135, "9" },
	{ 0x136, "A" },
	{ 0x137, "B" },
	{ 0x138, "C" },
	{ 0x139, "D" },
	{ 0x13A, "E" },
	{ 0x13B, "F" },
	{ 0x13C, "        ACCESS CODE:" },
	{ 0x13D, "PRESS BUTTON OR RETURN TO CONTINUE" },
	{ 0x13E, "   ENTER ACCESS CODE" },
	{ 0x13F, "   INVALID PASSWORD !" },
	{ 0x140, "ANNULER" },
	{ 0x141, "      INSERT DISK ?\n\n\n\n\n\n\n\n\nPRESS ANY KEY TO CONTINUE" },
	{ 0x142, " SELECT SYMBOLS CORRESPONDING TO\n THE POSITION\n ON THE CODE WHEEL" },
	{ 0x143, "    LOADING..." },
	{ 0x144, "              ERROR" },
	{ 0x15E, "LDKD" },
	{ 0x15F, "HTDC" },
	{ 0x160, "CLLD" },
	{ 0x161, "FXLC" },
	{ 0x162, "KRFK" },
	{ 0x163, "XDDJ" },
	{ 0x164, "LBKG" },
	{ 0x165, "KLFB" },
	{ 0x166, "TTCT" },
	{ 0x167, "DDRX" },
	{ 0x168, "TBHK" },
	{ 0x169, "BRTD" },
	{ 0x16A, "CKJL" },
	{ 0x16B, "LFCK" },
	{ 0x16C, "BFLX" },
	{ 0x16D, "XJRT" },
	{ 0x16E, "HRTB" },
	{ 0x16F, "HBHK" },
	{ 0x170, "JCGB" },
	{ 0x171, "HHFL" },
	{ 0x172, "TFBB" },
	{ 0x173, "TXHF" },
	{ 0x174, "JHJL" },
	{ 0x181, " BY" },
	{ 0x182, "ERIC CHAHI" },
	{ 0x183, "         MUSIC AND SOUND EFFECTS" },
	{ 0x184, " " },
	{ 0x185, "JEAN-FRANCOIS FREITAS" },
	{ 0x186, "IBM PC VERSION" },
	{ 0x187, "      BY" },
	{ 0x188, " DANIEL MORAIS" },
	{ 0x18B, "       THEN PRESS FIRE" },
	{ 0x18C, " PUT THE PADDLE ON THE UPPER LEFT CORNER" },
	{ 0x18D, "PUT THE PADDLE IN CENTRAL POSITION" },
	{ 0x18E, "PUT THE PADDLE ON THE LOWER RIGHT CORNER" },
	{ 0x258, "      Designed by ..... Eric Chahi" },
	{ 0x259, "    Programmed by...... Eric Chahi" },
	{ 0x25A, "      Artwork ......... Eric Chahi" },
	{ 0x25B, "Music by ........ Jean-francois Freitas" },
	{ 0x25C, "            Sound effects" },
	{ 0x25D, "        Jean-Francois Freitas\n             Eric Chahi" },
	{ 0x263, "              Thanks To" },
	{ 0x264, "           Jesus Martinez\n\n          Daniel Morais\n\n        Frederic Savoir\n\n      Cecile Chahi\n\n    Philippe Delamarre\n\n  Philippe Ulrich\n\nSebastien Berthet\n\nPierre Gousseau" },
	{ 0x265, "Now Go Out Of This World" },
	{ 0x190, "Good evening professor." },
	{ 0x191, "I see you have driven here in your\nFerrari." },
	{ 0x192, "IDENTIFICATION" },
	{ 0x193, "Monsieur est en parfaite sante." },
	{ 0x194, "Y\n" },
	{ 0x193, "AU BOULOT !!!\n" },
	{ END_OF_STRING_DICTIONARY, "" }
};

const StrEntry Video::_stringsTableDemo[] = {
	{ 0x001, "P E A N U T  3000" },
	{ 0x002, "Copyright  } 1990 Peanut Computer, Inc.\nAll rights reserved.\n\nCDOS Version 5.01" },
	{ 0x003, "2" },
	{ 0x004, "3" },
	{ 0x005, "." },
	{ 0x006, "A" },
	{ 0x007, "@" },
	{ 0x008, "PEANUT 3000" },
	{ 0x00A, "R" },
	{ 0x00B, "U" },
	{ 0x00C, "N" },
	{ 0x00D, "P" },
	{ 0x00E, "R" },
	{ 0x00F, "O" },
	{ 0x010, "J" },
	{ 0x011, "E" },
	{ 0x012, "C" },
	{ 0x013, "T" },
	{ 0x014, "Shield 9A.5f Ok" },
	{ 0x015, "Flux % 5.0177 Ok" },
	{ 0x016, "CDI Vector ok" },
	{ 0x017, " %%%ddd ok" },
	{ 0x018, "Race-Track ok" },
	{ 0x019, "SYNCHROTRON" },
	{ 0x01A, "E: 23%\ng: .005\n\nRK: 77.2L\n\nopt: g+\n\n Shield:\n1: OFF\n2: ON\n3: ON\n\nP~: 1\n" },
	{ 0x01B, "ON" },
	{ 0x01C, "-" },
	{ 0x021, "|" },
	{ 0x022, "--- Theoretical study ---" },
	{ 0x023, " THE EXPERIMENT WILL BEGIN IN    SECONDS" },
	{ 0x024, "  20" },
	{ 0x025, "  19" },
	{ 0x026, "  18" },
	{ 0x027, "  4" },
	{ 0x028, "  3" },
	{ 0x029, "  2" },
	{ 0x02A, "  1" },
	{ 0x02B, "  0" },
	{ 0x02C, "L E T ' S   G O" },
	{ 0x031, "- Phase 0:\nINJECTION of particles\ninto synchrotron" },
	{ 0x032, "- Phase 1:\nParticle ACCELERATION." },
	{ 0x033, "- Phase 2:\nEJECTION of particles\non the shield." },
	{ 0x034, "A  N  A  L  Y  S  I  S" },
	{ 0x035, "- RESULT:\nProbability of creating:\n ANTIMATTER: 91.V %\n NEUTRINO 27:  0.04 %\n NEUTRINO 424: 18 %\n" },
	{ 0x036, "   Practical verification Y/N ?" },
	{ 0x037, "SURE ?" },
	{ 0x038, "MODIFICATION OF PARAMETERS\nRELATING TO PARTICLE\nACCELERATOR (SYNCHROTRON)." },
	{ 0x039, "       RUN EXPERIMENT ?" },
	{ 0x03C, "t---t" },
	{ 0x03D, "000 ~" },
	{ 0x03E, ".20x14dd" },
	{ 0x03F, "gj5r5r" },
	{ 0x040, "tilgor 25%" },
	{ 0x041, "12% 33% checked" },
	{ 0x042, "D=4.2158005584" },
	{ 0x043, "d=10.00001" },
	{ 0x044, "+" },
	{ 0x045, "*" },
	{ 0x046, "% 304" },
	{ 0x047, "gurgle 21" },
	{ 0x048, "{{{{" },
	{ 0x049, "Delphine Software" },
	{ 0x04A, "By Eric Chahi" },
	{ 0x04B, "  5" },
	{ 0x04C, "  17" },
	{ 0x12C, "0" },
	{ 0x12D, "1" },
	{ 0x12E, "2" },
	{ 0x12F, "3" },
	{ 0x130, "4" },
	{ 0x131, "5" },
	{ 0x132, "6" },
	{ 0x133, "7" },
	{ 0x134, "8" },
	{ 0x135, "9" },
	{ 0x136, "A" },
	{ 0x137, "B" },
	{ 0x138, "C" },
	{ 0x139, "D" },
	{ 0x13A, "E" },
	{ 0x13B, "F" },
	{ 0x13D, "PRESS BUTTON OR RETURN TO CONTINUE" },
	{ 0x13E, "   ENTER ACCESS CODE" },
	{ 0x13F, "   INVALID PASSWORD !" },
	{ 0x140, "ANNULER" },
	{ 0x141, "          INSERT DISK ?" },
	{ 0x142, " SELECT SYMBOLS CORRESPONDING TO\n THE POSITION\n ON THE CODE WHEEL" },
	{ 0x143, "    LOADING..." },
	{ 0x144, "              ERROR" },
	{ 0x181, " BY" },
	{ 0x182, "ERIC CHAHI" },
	{ 0x183, "         MUSIC AND SOUND EFFECTS" },
	{ 0x184, " " },
	{ 0x185, "JEAN-FRANCOIS FREITAS" },
	{ 0x186, "IBM PC VERSION" },
	{ 0x187, "      BY" },
	{ 0x188, " DANIEL MORAIS" },
	{ 0x18B, "       THEN PRESS FIRE" },
	{ 0x18C, " PUT THE PADDLE ON THE UPPER LEFT CORNER" },
	{ 0x18D, "PUT THE PADDLE IN CENTRAL POSITION" },
	{ 0x18E, "PUT THE PADDLE ON THE LOWER RIGHT CORNER" },
	{ 0x1F4, "Over Two Years in the Making" },
	{ 0x1F5, "   A New, State\nof the Art, Polygon\n  Graphics System" },
	{ 0x1F6, "   Comes to the\nComputer With Full\n Screen Graphics" },
	{ 0x1F7, "While conducting a nuclear fission\nexperiment at your local\nparticle accelerator ..." },
	{ 0x1F8, "Nature decides to put a little\n    extra spin on the ball" },
	{ 0x1F9, "And sends you ..." },
	{ 0x1FA, "     Out of this World\nA Cinematic Action Adventure\n Coming soon to a computer\n      screen near you\n from Interplay Productions\n   coming soon to the IBM" },
	{ 0x258, "      Designed by ..... Eric Chahi" },
	{ 0x259, "    Programmed by...... Eric Chahi" },
	{ 0x25A, "      Artwork ......... Eric Chahi" },
	{ 0x25B, "Music by ........ Jean-francois Freitas" },
	{ 0x25C, "            Sound effects" },
	{ 0x25D, "        Jean-Francois Freitas\n             Eric Chahi" },
	{ 0x263, "              Thanks To" },
	{ 0x264, "           Jesus Martinez\n\n          Daniel Morais\n\n        Frederic Savoir\n\n      Cecile Chahi\n\n    Philippe Delamarre\n\n  Philippe Ulrich\n\nSebastien Berthet\n\nPierre Gousseau" },
	{ 0x265, "Now Go Out Of This World" },
	{ 0x190, "Good evening professor." },
	{ 0x191, "I see you have driven here in your\nFerrari." },
	{ 0x192, "IDENTIFICATION" },
	{ 0x193, "Monsieur est en parfaite sante." },
	{ 0x194, "Y\n" },
	{ 0x193, "AU BOULOT !!!\n" },
};
